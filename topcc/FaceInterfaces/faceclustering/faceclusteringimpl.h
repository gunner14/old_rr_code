#pragma once
#include "faceclustering.h"
#include "../facefeature/facefeature.h"
#include "../facedetection/facedetector.h"
class FaceClustering : public IFaceClustering
{
public:
	FaceClustering(void);
	~FaceClustering(void);

public:
	 void scanImage(const recognize::Image& img);
	 void setFacePtr(vector<recognize::FacePtr> *pFaces);
	 void endScan();
	 void release();
	 int getFeatureLength();
	 bool cluster(vector<float*> featureBuffer, vector<int>& clusterNum, double dist = 0.25);
private:
	void _cluster(double);
private:
	IFaceDetector* m_pFaceDetector;
	IFaceFeature* m_pFaceDescriptor;
	vector<recognize::FacePtr> * m_pVecFaces;
	double m_dDistT;
};
