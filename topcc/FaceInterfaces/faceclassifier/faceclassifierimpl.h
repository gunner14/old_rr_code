#include "faceclassifier.h"


//-------------------------------------------------------------------------------------------------
class CFaceClassifier : public IFaceClassifier
{
public:
	// multi-class classification: m classes, tag updated flags, xs feature vectors, ys labels
	bool  TrainLSVMs(int dim, int m, vector<bool> &tag, vector<float*> &xs, vector<int> &ys, float c, 
		             vector<float*> &wbs);
	// multi-class classification: m classes, m*(m-1)/2 wbs, m sorted labels
	bool  TestLSVMs(int dim, float *x, int m, vector<float*> &wbs, vector<int> &l);
	bool  TestLSVMs(int dim, vector<float*> &xs, int m, vector<float*> &wbs, vector<int> &l);

	void  Release();

protected:
	bool  TrainLSVM(int dim, vector<float*> &xs, vector<int> &ys, float c, float *wb);
	float TestLSVM(int dim, float *x, float *wb);

	float KernelFunction(int n, float *x1, float *x2, int ker, float arg);
	void  KernelMatrix(int n, vector<float*> &xs, vector<int> &ys, int ker, float arg, float *K);
	int   FSMO(int n, float *A, vector<int> &b, float *x, float xh, float tol);
	float Probability(float f, float a);
};
