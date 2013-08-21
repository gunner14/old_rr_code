#pragma once


//-------------------------------------------------------------------------------------------------
class CFaceClusters
{
public:
	CFaceClusters(int num, int dim, float *dat);
	
	int   FaceClusters(int *iface, float th, float tl, int metric);
	
private:
	float GetSimilar(int n, float *x0, float *x1, int metric);
	void  PrintMsg(int n, float *fmat, float *fvec, int *ivec);
	
	int   num; // face number
	int   dim; // feature dimension
	float*dat; // feature data dat[num*dim]
};
