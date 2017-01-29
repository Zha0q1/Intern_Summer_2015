#include <python2.7/Python.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <string>
#include "helperFunctions.h"

using namespace std;

double getVolume(const char filename[])
{
	return getSinglePara(filename, 'v');
}

double getArea(const char filename[])
{
	return getSinglePara(filename, 'a');
}

int getNumFacets(const char filename[])
{
	int type = checker(filename);
	if (type==-1){return -1;}
	if (type==0){getANum(filename);}
	else{getBNum(filename);}
}

PyObject* getAll(const char filename[])
{
	return getAllHelper(filename);
}

PyObject *wrap_getVolume(PyObject *self, PyObject *args) { 
	const char* f;
	double result;
	if (!PyArg_ParseTuple(args,"s",&f)) 
		return NULL;
	result = getVolume(f);
	return Py_BuildValue("d",result);
}

PyObject *wrap_getArea(PyObject *self, PyObject *args) { 
	const char* f;
	double result;
	if (!PyArg_ParseTuple(args,"s",&f)) 
		return NULL;
	result = getArea(f);
	return Py_BuildValue("d",result);
}

PyObject *wrap_getNumFacets(PyObject *self, PyObject *args) { 
	const char* f;
	int result;
	if (!PyArg_ParseTuple(args,"s",&f)) 
		return NULL;
	result = getNumFacets(f);
	return Py_BuildValue("i",result);
}

PyObject *wrap_getAll(PyObject *self, PyObject *args) { 
	const char* f;
	PyObject* result;
	if (!PyArg_ParseTuple(args,"s",&f)) 
		return NULL;
	result = getAll(f);
	return Py_BuildValue("O",result);
}

static PyMethodDef parsingMethods[] = { 
		{ "getVolume", wrap_getVolume, 1 },
		{ "getArea",wrap_getArea,1},
		{ "getNumFacets", wrap_getNumFacets, 1},
		{ "getAll", wrap_getAll, 1},
		{ NULL, NULL }
};


PyMODINIT_FUNC initparsingModule() { 
		PyObject *m;
		m = Py_InitModule("parsingModule", parsingMethods);
}