#include <python2.7/Python.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <string>

using namespace std;

int checker(const char filename[]);
void calculateAreaAndVolume(float**& v, double& area, 
	double& volume, char what);
int getANum(const char filename[]);
int getBNum(const char filename[]);
void calculateAscii(const char filename[], int& num, 
	double& surfaceArea, double& volume, char what);
void calculateBinary(const char filename[], int& num, 
	double& surfaceArea, double& volume, char what);
double getSinglePara(const char filename[], char what);
PyObject* getAllHelper(const char filename[]);