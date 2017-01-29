#include "helperFunctions.h"

using namespace std;

int checker(const char filename[])
{
	ifstream inFile(filename);

	if (!inFile){
		return -1;
	}

	int i = 0;
	int accuracy = 100;
	int c;
	while ((c=inFile.get())!=EOF && c<=127 && i<accuracy){
		i++;
	}

	inFile.close();

	if (i==accuracy)return 0;
	else return 1;
}


void calculateAreaAndVolume(float**& v, double& area, 
	double& volume, char what)
{

	if (what=='a'||what=='b'){
		float** vec = new float*[3];
		vec[0] = new float[3];
		vec[1] = new float[3];
		vec[2] = new float[3];

		for (int i=0;i<3;i++){
			vec[0][i] = v[1][i] - v[0][i];
			vec[1][i] = v[2][i] - v[1][i];
			vec[2][i] = v[0][i] - v[2][i];
		}

		double deltaA = 0.5 * sqrt(
			pow(vec[0][1]*vec[1][2]-vec[0][2]*vec[1][1],2) +
			pow(vec[0][2]*vec[1][0]-vec[0][0]*vec[1][2],2) +
			pow(vec[0][0]*vec[1][1]-vec[0][1]*vec[1][0],2) );
		area += deltaA;

		delete[] vec[0];
		delete[] vec[1];
		delete[] vec[2];
		delete[] vec;
	}

	if(what=='v'||what=='b'){
		double v321 = v[2][0]*v[1][1]*v[0][2];
		double v231 = v[1][0]*v[2][1]*v[0][2];
		double v312 = v[2][0]*v[0][1]*v[1][2];
		double v132 = v[0][0]*v[2][1]*v[1][2];
		double v213 = v[1][0]*v[0][1]*v[2][2];
		double v123 = v[0][0]*v[1][1]*v[2][2];
		double deltaV = (1.0/6.0)*(-v321 + v231 + v312 - v132 - v213 + v123);
		volume += deltaV;
	}
}

int getANum(const char filename[])
{
	ifstream inFile(filename);

	string line;
	int num = 0;

	getline(inFile, line);;

	while ((!inFile.eof())||(!inFile.fail())){
		inFile.ignore(256,'n');
		num++;
	}
	num--;
	return num/3-1;
}

int getBNum(const char filename[])
{
	ifstream inFile(filename,ios::binary | ios::in);

	int num;

	inFile.ignore(80);
	inFile.read((char*)(&num),4);

	return num;
}


void calculateAscii(const char filename[], int& num, 
	double& surfaceArea, double& volume, char what)
{

	ifstream inFile(filename);

	string line, token;
	stringstream ss;

	getline(inFile,line);
	ss << line;
	ss >> token;
	ss >> token;

	float** v = new float*[3];
	for (int i=0;i<3;i++){
		v[i] = new float[3];
	}
	while ((!inFile.eof())||(!inFile.fail())){
		inFile.ignore(256,'v');
		for (int i=0;i<3;i++){
			getline(inFile,line);
			stringstream sss;
			sss << line;
			sss >> token;
			sss >> v[i][0];
			sss >> v[i][1];
			sss >> v[i][2];
		}
		calculateAreaAndVolume(v,surfaceArea,volume,what);
		num++;
	}

	num--;
	volume = abs(volume);

	delete[] v[0];
	delete[] v[1];
	delete[] v[2];
	delete[] v;
	inFile.close();
}

void calculateBinary(const char filename[], int& num, 
	double& surfaceArea, double& volume, char what)
{
	
	ifstream inFile(filename,ios::binary|ios::in);

	char header[80];

	inFile.read(header, 80);
	inFile.read((char*)(&num), 4);

	float** v = new float*[3];
	for (int i=0;i<3;i++){
		v[i] = new float[3];
	}

	while ((!inFile.eof()) || (!inFile.fail())){
		inFile.ignore(12);
		for (int i=0;i<3;i++){
			inFile.read(reinterpret_cast<char*>(v[i]), 3*sizeof(float));
		}

		inFile.ignore(2);

		calculateAreaAndVolume(v,surfaceArea,volume, what );
	}

	volume = abs(volume);

	delete[] v[0];
	delete[] v[1];
	delete[] v[2];
	delete[] v;

 	inFile.close();

}

double getSinglePara(const char filename[], char what)
{

	int type = checker(filename);
	if (type==-1){return -1.0;}

	double surfaceArea = 0, volume = 0;
	int num = 0, l, w, h;

	switch (what) {
	case 'v':
		if (type==0){calculateAscii(filename,num,surfaceArea,volume,'v');}
		else{calculateBinary(filename,num,surfaceArea,volume,'v');}
		return volume;
		break;
	case 'a':
		if (type==0){calculateAscii(filename,num,surfaceArea,volume,'a');}
		else{calculateBinary(filename,num,surfaceArea,volume,'a');}
		return surfaceArea;
   		break;
	}
}

PyObject* getAllHelper(const char filename[])
{

	clock_t start = clock();

	int type = checker(filename);
	if (type==-1){
		PyObject *rslt = PyTuple_New(0);
		return rslt;
	}

	double surfaceArea = 0, volume = 0;
	int num = 0, l, w, h;

	if (type==0){calculateAscii(filename,num,surfaceArea,volume,'b');}
	else{calculateBinary(filename,num,surfaceArea,volume,'b');}

	double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

	PyObject *rslt = PyTuple_New(4);
	PyTuple_SetItem(rslt, 0, Py_BuildValue("i",num));
	PyTuple_SetItem(rslt, 1, Py_BuildValue("d",surfaceArea));
	PyTuple_SetItem(rslt, 2, Py_BuildValue("d",volume));
	PyTuple_SetItem(rslt, 3, Py_BuildValue("d",duration));
	return rslt;
}