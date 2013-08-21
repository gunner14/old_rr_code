#pragma once
#include "../include/faceutils.h"
using namespace std;
class IFaceClustering
{
public:
	static IFaceClustering* createInstance();
	virtual void scanImage(const recognize::Image& img) = 0;
	virtual void setFacePtr(vector<recognize::FacePtr> *pFaces)=0;
	virtual void endScan()=0;
	virtual void release()=0;
	virtual int getFeatureLength()=0;
	virtual bool cluster(vector<float*> featureBuffer, vector<int>& clusterNum, double dist = 0.25)=0;
};
 
