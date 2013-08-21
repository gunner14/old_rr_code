#include "facefeature.h"


//-------------------------------------------------------------------------------------------------
//#define GRY_FEATURE
#define LBP_FEATURE
//#define HOG_FEATURE
//#define LBP_ELASTIC
#define STD_2P
//#define STD_3P


//-------------------------------------------------------------------------------------------------
#define STD_W 88 // standard width after normalization
#define STD_H 88 // standard height after normalization


//-------------------------------------------------------------------------------------------------
#define GRY_C  4
#define GRY_D ((STD_W/GRY_C)*(STD_H/GRY_C)) // dimension


//-------------------------------------------------------------------------------------------------
#define LBP_C 11 // cell size
#define LBP_D (59*(STD_W/LBP_C)*(STD_H/LBP_C)) // dimension


//-------------------------------------------------------------------------------------------------
#define HOG_C 11 // cell size
#define HOG_D (8*(STD_W/HOG_C)*(STD_H/HOG_C)) // dimension


//-------------------------------------------------------------------------------------------------
class CFaceFeature : public IFaceFeature
{
public:
	CFaceFeature();
	~CFaceFeature();
	
	bool  LoadFaceAsm(char *pathname);
	int   GetFeatureLength();
	
	bool  ExtractFeature(int w, int h, byte *bgra, int x0, int y0, int x1, int y1, float *x);
	bool  extractFeature(byte* imageData, int imgWidth, int imgHeight, std::vector<rect> faceRegion, std::vector<float*>& featureBuffer);
	float GetSimilarity(float *x0, float *x1, float *w, int metric);
	void  Release();

protected:
	void  BgraToGray(int w, int h, byte *bgra, byte *gray);
	void  RotateRescaleFace(int w0, int h0, byte *im0, int x0, int y0, 
		                    int w1, int h1, byte *im1, int x1, int y1, 
		                    double a, double s);
	void  AffineTransform(int w0, int h0, byte *im0, int w1, int h1, byte *im1, double *tri0, 
		                  double *tri1);
	bool  SolveLinearSystem2(int n, double *A, double *b, double *x);
	void  GaussianSmooth(int w, int h, byte *im);

	void  LBP82(int w, int h, byte *im, int d, float *fv, byte *mask = 0);
	
	void  SoftVote(int x, int y, double f, double fx, double fy, int wv, int hv, float *fv);
	void  HOG8(int w, int h, byte *im, int dv, float *fv, byte *mask = 0);

	void  DownSample(int w, int h, byte *im0, int d, byte *im1);

	// elasticmatch.cpp
	void  FaceToLBP82(int w, int h, byte *im, byte *lbp);
	bool  GetLBPHist(int w, int h, byte *lbp, int x, int y, int rw, int *hist);
	double HistogramMatch(int n, int *hist1, int *hist2, int sum);
	double FindPercentile(double p, int n, double *f);
	double ElasticMatch(int w, int h, byte *lbp1, byte *lbp2, int rw, int sw, int rn, int sn, double a);

	void *m_pFaceAsm;
};

