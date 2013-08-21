#include "faceclusters.h"
#include <memory.h>
#include <stdio.h>
#include <math.h>


//-------------------------------------------------------------------------------------------------
CFaceClusters::CFaceClusters(int num, int dim, float *dat)
{
	this->num = num;
	this->dim = dim;
	this->dat = dat;
}


//-------------------------------------------------------------------------------------------------
void CFaceClusters::PrintMsg(int n, float *fmat, float *fvec, int *ivec)
{
	for(int i=0; i<n; i++)
	{
		for(int j=n-20; j<n; j++)
		{
			printf("%.2f ", fmat[i*n+j]);
		}
		//if((i+1)%10==0) printf("= %5.2f .%2d\n", fvec[i], ivec[i]);
		if(i==13 || i==24 || i==34) printf("= %5.2f .%2d\n", fvec[i], ivec[i]);
		else printf("= %5.2f  %2d\n", fvec[i], ivec[i]);
	}
}


//-------------------------------------------------------------------------------------------------
int CFaceClusters::FaceClusters(int *iface, float th, float tl, int metric)
{
	float *fmat = new float[num*num];
	float *fvec = new float[num];
	bool  *bvec = new bool[num];
	int   *hist = new int[num];

	int i, j, k;
	for(i=0; i<num; i++)
	{
		float *xi = dat+i*dim;
		for(j=0; j<i; j++)
		{
			float *xj = dat+j*dim;
			float f = GetSimilar(dim, xi, xj, metric);
			fmat[i*num+j] = fmat[j*num+i] = f;//max(0, f-0.5f);
		}
		fmat[i*num+i] = 0;
	}

	for(i=0; i<num; i++)
	{
		float sum = 0;
		for(j=0; j<num; j++) if(fmat[i*num+j]>th) sum++;
		//for(j=0; j<num; j++) sum += fmat[i*num+j]; // 1
		fvec[i] = sum;
		iface[i] = -1;
	}

	k = 1;
	while(true)
	{
		float fmax = -1;
		for(i=0; i<num; i++) if(iface[i]==-1 && fmax<fvec[i]) fmax = fvec[i], j = i;
		if(fmax==-1) break;
		//if(fmax<t) printf("%f\n", fmax);
		//iface[j] = k;
		
		if(fmax>0)
		{
			iface[j] = k;
			
			// find neighbors
			for(i=0; i<num; i++) if(iface[i]==-1 && fmat[j*num+i]>th) iface[i] = k;
			
			// remove unbelievable
			for(i=0; i<num; i++)
			{
				bvec[i] = false;
				if(i!=j && iface[i]==k)
				{
					int num1 = 0, num2 = 0;
					float *p = fmat+i*num;
					for(int l=0; l<num; l++)
					{
						if(p[l]>th)
						{
							num1++;
							if(iface[l]==k) num2++;
						}
					}
					if(num2<0.6*num1) bvec[i] = true;
				}
			}
			for(i=0; i<num; i++) if(bvec[i]) iface[i] = -1;

			int numk = 0;
			for(i=0; i<num; i++) if(iface[i]==k) numk++;

			if(numk>1) k++;
			else iface[j] = 0;
		}
		else iface[j] = 0;
	}

	for(k=0; k<num; k++)
	{
		if(iface[k]==0)
		{
			float fmax = 0, *p = fmat+k*num;
			int i0 = 0;
			for(i=0; i<num; i++)
			{
				if(iface[i]==0) continue;
				if(fmax<p[i]) fmax = p[i], i0 = i;
			}

			if(fmax>tl && fvec[k]<=1) iface[k] = iface[i0];
		}
	}

	int maxid = 0;
	for(i=0; i<num; i++) if(maxid<iface[i]) maxid = iface[i];
	for(i=0; i<num; i++) if(iface[i]==0) iface[i] = maxid++;

	PrintMsg(num, fmat, fvec, iface);

	delete []fmat;
	delete []fvec;
	delete []bvec;
	delete []hist;

	return k-1;
}


//-------------------------------------------------------------------------------------------------
float CFaceClusters::GetSimilar(int n, float *x0, float *x1, int metric)
{
	if(metric==0)
	{
		// Bhattacharyya coefficient
		double f = 0, f0 = 0, f1 = 0;
		for(int i=0; i<n; i++)
		{
			f  += sqrt(x0[i]*x1[i]);
			f0 += x0[i];
			f1 += x1[i];
		}
		f /= sqrt(f0*f1);
		return (float)f;
	}
	else
	{
		// Cosine coefficient
		double f = 0, f0 = 0, f1 = 0;
		for(int i=0; i<n; i++)
		{
			f  += x0[i]*x1[i];
			f0 += x0[i]*x0[i];
			f1 += x1[i]*x1[i];
		}
		f = f/sqrt(f0*f1);
		return (float)f;
	}
}
