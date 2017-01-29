#include <python2.7/Python.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <string>
#include <cfloat>
//#include <cstdlib>
#include <vector>
#include <algorithm> 

#include <GL/glut.h>

using namespace std;



class Line2D {
public:
	Line2D(double x1, double y1, double x2, double y2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
	double get_x1(){return x1;}
	double get_y1(){return y1;}
	double get_x2(){return x2;}
	double get_y2(){return y2;}

	~Line2D(){}
private:
	double x1, y1;
	double x2, y2;
};


// GLOBAL VARS

int layer = 1;
vector<vector<Line2D*> > vec_layers;
float x_min, x_max;
float y_min, y_max;
float x_mid, y_mid;
float x_dif, y_dif;

































void Display(){
    glClear( GL_COLOR_BUFFER_BIT );

    float ratio = 1.6 / max(x_dif,y_dif);

	{
        glColor3f( 0.0f, 0.0f, 1.0f );//指定线的颜色,蓝色
        glBegin( GL_LINES );
        {
        	for (vector<Line2D*>::iterator it = vec_layers[layer].begin() ; 
				it != vec_layers[layer].end(); ++it){

        		double x1,y1,x2,y2;
        		x1 = (*it)->get_x1();
        		y1 = (*it)->get_y1();
        		x2 = (*it)->get_x2();
        		y2 = (*it)->get_y2();

        		//cout << "(" << x1 << "," << y1 << ")->(" << x2 << "," << y2 << ")" << endl;

        		glVertex3f(ratio*((*it)->get_x1()-x_mid), ratio*((*it)->get_y1()-y_mid), 0.0f);
        		glVertex3f(ratio*((*it)->get_x2()-x_mid), ratio*((*it)->get_y2()-y_mid), 0.0f);

        	}

        }
        glEnd();

    }
    cout << "CURRENT LAYER :  " << layer << endl;

	glutSwapBuffers();

}

void SetupRC()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1 );//以RGB(0,0,0)即黑色来清空颜色缓冲区

    glColor3f( 1.0f, 0.0f, 0.0f );//以RGB(1,0,0)即红色来绘制图形

}




void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
    	if (layer == vec_layers.size()-1)
    		layer += 0;
    	else
        	layer += 1.0f;

    if(key == GLUT_KEY_DOWN)
    	if (layer == 0)
    		layer += 0;
    	else
        	layer -= 1.0f;

    if(key == GLUT_KEY_LEFT)
		layer -= vec_layers.size()/20;
		if (layer<0)
			layer = 0;

    if(key == GLUT_KEY_RIGHT)
    	layer += vec_layers.size()/20;
    	if (layer>vec_layers.size()-1)
    		layer = vec_layers.size()-1;




    glutPostRedisplay();// this will refresh the window, so, it works the same to call RenderScene() directly 

}







































void calculate_binary_dimensions(const char filename[], 
	float& z_max, float& z_min)
{

	ifstream inFile(filename,ios::binary|ios::in);

	inFile.ignore(84);

	float tmp;

	x_min = y_min = z_min = FLT_MAX;
	x_max = y_max = z_max = FLT_MIN;

	while ((!inFile.eof()) || (!inFile.fail())){
		inFile.ignore(12);
		for (int i=0;i<3;i++){
			inFile.read(reinterpret_cast<char*>(&tmp), sizeof(float));
			if (tmp>x_max) x_max = tmp;
			if (tmp<x_min) x_min = tmp;
			inFile.read(reinterpret_cast<char*>(&tmp), sizeof(float));
			if (tmp>y_max) y_max = tmp;
			if (tmp<y_min) y_min = tmp;
			inFile.read(reinterpret_cast<char*>(&tmp), sizeof(float));
			if (tmp>z_max) z_max = tmp;
			if (tmp<z_min) z_min = tmp;
		}
		inFile.ignore(2);
	}
 	inFile.close();
 	x_mid = (x_max+x_min)/2;
 	y_mid = (y_max+y_min)/2;
 	x_dif = x_max - x_min;
 	y_dif = y_max - y_min;
}


void calculate_ascii_dimensions(const char filename[], 
	float& z_max, float& z_min)
{
	
	ifstream inFile(filename);

	string line;

	getline(inFile,line);

	char buf[16];
	float tmp;

	x_min = y_min = z_min = FLT_MAX;
	x_max = y_max = z_max = FLT_MIN;

	while ((!inFile.eof())||(!inFile.fail())){
		inFile.ignore(256,'x');
		//x
		inFile.ignore(1);
		inFile.get(buf,16,' ');
		tmp = atof(buf);
		if (tmp>x_max) x_max = tmp;
		if (tmp<x_min) x_min = tmp;
		//y
		inFile.ignore(1);
		inFile.get(buf,16,' ');
		tmp = atof(buf);
		if (tmp>y_max) y_max = tmp;
		if (tmp<y_min) y_min = tmp;
		//z
		inFile.ignore(1);
		inFile.get(buf,16,' ');
		tmp = atof(buf);
		if (tmp>z_max) z_max = tmp;
		if (tmp<z_min) z_min = tmp;
	}
	inFile.close();
	x_mid = (x_max+x_min)/2;
 	y_mid = (y_max+y_min)/2;
 	x_dif = x_max - x_min;
 	y_dif = y_max - y_min;
}


class Triangle {
public:
	Triangle(double x1,double y1, double z1,
		double x2, double y2, double z2,
		double x3, double y3, double z3):
		x1(x1),x2(x2),x3(x3),y1(y1),y2(y2),
		y3(y3),z1(z1),z2(z2),z3(z3)
		{
		z_max = max(max(z1,z2),z3);
		z_min = min(min(z1,z2),z3);
		}
	Triangle(float** v)
	{
		x1 = v[0][0];
		x2 = v[1][0];
		x3 = v[2][0];
		y1 = v[0][1];
		y2 = v[1][1];
		y3 = v[2][1];
		z1 = v[0][2];
		z2 = v[1][2];
		z3 = v[2][2];
		z_max = max(max(z1,z2),z3);
		z_min = min(min(z1,z2),z3);
	}
	~Triangle(){}
	double get_x1(){return x1;}
	double get_x2(){return x2;}
	double get_x3(){return x3;}
	double get_y1(){return y1;}
	double get_y2(){return y2;}
	double get_y3(){return y3;}
	double get_z1(){return z1;}
	double get_z2(){return z2;}
	double get_z3(){return z3;}
	double get_z_max(){return z_max;}
	double get_z_min(){return z_min;}
private:
	double z_max, z_min;
	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;
};

class Line {
public:
	Line(double x1, double y1, double z1, double x2, double y2, double z2){
		this->x1 = x1;
		this->y1 = y1;
		this->z1 = z1;
		this->x2 = x2;
		this->y2 = y2;
		this->z2 = z2;
	}
	~Line(){}

	bool find_intersecting_point_with_plane(double height, double& x, double& y){
		if ((z1>height&&z2<height)||(z1<height&&z2>height)){
			double ratio = abs((height-z1)/(z1-z2));
			//cout << "ratio: " <<ratio << endl;
			x = x1 + ratio * (x2 - x1);
			y = y1 + ratio * (y2 - y1);
			return true;
		}
		return false;
	}


private:
	double x1, y1, z1;
	double x2, y2, z2;
};

double find_line_length(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}


void find_intersection(Triangle* tri, double height, double& x1,
	double& y1, double& x2, double& y2)
{
	Line* l1 = new Line(tri->get_x1(),tri->get_y1(), tri->get_z1(),
		tri->get_x2(), tri->get_y2(), tri->get_z2());
	Line* l2 = new Line(tri->get_x2(),tri->get_y2(), tri->get_z2(),
		tri->get_x3(), tri->get_y3(), tri->get_z3());
	Line* l3 = new Line(tri->get_x3(),tri->get_y3(), tri->get_z3(),
		tri->get_x1(), tri->get_y1(), tri->get_z1());

	if (l1->find_intersecting_point_with_plane(height,x1,y1)){
		if (l2->find_intersecting_point_with_plane(height,x2,y2)){}
		else l3->find_intersecting_point_with_plane(height,x2,y2);
	} else{
		l2->find_intersecting_point_with_plane(height,x1,y1);
		l3->find_intersecting_point_with_plane(height,x2,y2);
	}

	delete l1;
	delete l2;
	delete l3;
}

void slice(vector<Triangle*>& vec, double height, int& n, double& exterior)
{
	vector<Line2D*> vec_lines;

	//cout << "++++++++++++++++++++++++\n height: " << height << endl;

	for (vector<Triangle*>::iterator it = vec.begin() ; 
		it != vec.end(); ++it){

		if ((*it)->get_z_min() < height){
			if ((*it)->get_z_max() > height){
				double x1, y1, x2, y2;
				find_intersection((*it),height,x1,y1,x2,y2);

				//cout << "(" << x1 << "," << y1 << ")->(" << x2 << "," << y2 << ")" << endl;


				exterior += find_line_length(x1,y1,x2,y2)*0.2*3*0.2;
				n++;
				vec_lines.push_back(new Line2D(x1,y1,x2,y2));
			}
		}
	}
	vec_layers.push_back(vec_lines);
}

void load_ascii(vector<Triangle*>& vec, const char filename[])
{

	ifstream inFile(filename);
	string line;
	char buf[16];

	getline(inFile,line);

	float** v = new float*[3];
	for (int i=0;i<3;i++){
		v[i] = new float[3];
	}

	while ((!inFile.eof())||(!inFile.fail())){
		for(int j=0;j<3;j++){
			inFile.ignore(256,'x');
			for (int i=0;i<3;i++){
				inFile.ignore(1);
				inFile.get(buf,16,' ');
				v[j][i] = atof(buf);
			}
		}
		vec.push_back(new Triangle(v));
	}

	inFile.close();
	delete[] v[0];
	delete[] v[1];
	delete[] v[2];
	delete[] v;
}

void slice_ascii(vector<Triangle*>& vec, double thickness, const char filename[])
{
	float z_max, z_min;
	calculate_ascii_dimensions(filename, z_max, z_min);
	
	int num_layers = (z_max-z_min+0.5)/thickness + 1;
	int n = 0;
	double exterior = 0;
	for (int i=0;i<num_layers;i++){
		slice(vec,thickness*i+z_min,n,exterior);
	}

	cout << "n: " << n << endl;
	cout << "exterior: " << exterior/1000 << endl;
}

void load_binary(vector<Triangle*>& vec, const char filename[])
{
	ifstream inFile(filename,ios::binary|ios::in);

	inFile.ignore(84);

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
		vec.push_back(new Triangle(v));
	}

 	inFile.close();

 	delete[] v[0];
	delete[] v[1];
	delete[] v[2];
	delete[] v;
}

void slice_binary(vector<Triangle*>& vec, double thickness,  const char filename[]){
	float z_max, z_min;
	calculate_binary_dimensions(filename, z_max, z_min);
	
	int num_layers = (z_max-z_min+0.5)/thickness + 1;
	int n = 0;
	double exterior = 0;
	for (int i=0;i<num_layers;i++){
		slice(vec,thickness*i+z_min,n,exterior);
	}

	cout << "n: " << n << endl;
	cout << "exterior: " << exterior/1000 << endl;
}








int main(int argc, char *argv[])
{

	clock_t start = clock();

	vector<Triangle*> vec;

	//load_ascii(vec,"knob.stl");
	//slice_ascii(vec,0.2,"knob.stl");
	load_binary(vec,"robot.stl");
	slice_binary(vec,0.1,"robot.stl");

	double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << "Time Taken:" << duration << endl;

	cout << "count " << vec_layers.size() << endl;

	// while (1){
	// 	int instruction;
	// 	cin >> instruction;
	// 	if (instruction == 1){
	// 		cout << "1" << endl;
	// 	}
	// 	if (instruction == 0){
	// 		break;
	// 	}
	// }



	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (10, 10);
    glutCreateWindow( "Point examples" );
    glutDisplayFunc( Display );
    //glutReshapeFunc( ChangeSize );
    glutSpecialFunc( SpecialKeys );
    SetupRC();
    glutMainLoop();
}