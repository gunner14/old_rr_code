#pragma once
#include "../include/public.h"
#include <vector>
using namespace std;


//-------------------------------------------------------------------------------------------------
interface IFaceClassifier
{
	static IFaceClassifier* CreateInstance();

	// multi-class classification: m classes, tag updated flags, xs feature vectors, ys labels
	virtual bool TrainLSVMs(int dim, int m, vector<bool> &tag, vector<float*> &xs, vector<int> &ys, 
		float c, vector<float*> &wbs) = 0;

	// multi-class classification: m classes, m*(m-1)/2 wbs, m sorted labels
	virtual bool TestLSVMs(int dim, float *x, int m, vector<float*> &wbs, vector<int> &l) = 0; // one face
	virtual bool TestLSVMs(int dim, vector<float*> &xs, int m, vector<float*> &wbs, vector<int> &l) = 0; // a group of faces

	virtual void Release() = 0;
};


