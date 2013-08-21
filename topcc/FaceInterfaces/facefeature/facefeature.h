#pragma once
#include "../include/public.h"
#include <vector>


//-------------------------------------------------------------------------------------------------
interface IFaceFeature
{
	static IFaceFeature* CreateInstance(char *pathname = "../../data");

	virtual int   GetFeatureLength() = 0;
	virtual bool  ExtractFeature(int w, int h, byte *bgra, int x0, int y0, int x1, int y1, float *x) = 0;
	virtual bool  extractFeature(byte* imageData, int imgWidth, int imgHeight, std::vector<rect> faceRegion, std::vector<float*>& featureBuffer) = 0;
	virtual float GetSimilarity(float *x0, float *x1, float *w, int metric) = 0;

	virtual void  Release() = 0;
};
