#include "faceclassifierimpl.h"
#include <stdio.h>
#include <math.h>
#include <algorithm>
#pragma warning(disable: 4018)

//-------------------------------------------------------------------------------------------------
IFaceClassifier* IFaceClassifier::CreateInstance()
{
	CFaceClassifier *fc = new CFaceClassifier();
	return (IFaceClassifier*)fc;
}


//-------------------------------------------------------------------------------------------------
// multi-class classification: m classes, tag updated flags, xs feature vectors, ys labels
bool CFaceClassifier::TrainLSVMs(int dim, int m, vector<bool> &tag, vector<float*> &xs, vector<int> &ys, 
								 float c, vector<float*> &wbs)
{
	if(tag.size()!=m || xs.size()<m || xs.size()!=ys.size() || wbs.size()!=m*(m-1)/2) return false;

	const int n = ys.size();
	for(int i=0, iwb=0; i<m-1; i++)
	{
		for(int j=i+1; j<m; j++)
		{
			if(!tag[i] && !tag[j])
			{
				iwb++;
				continue;
			}

			vector<float*> xs_ij;
			vector<int> ys_ij;
			int ni = 0, nj = 0;
			for(int k=0; k<n; k++)
			{
				if(ys[k]==i)
				{
					xs_ij.push_back(xs[k]);
					ys_ij.push_back(1);
					ni++;
				}
				else if(ys[k]==j)
				{
					xs_ij.push_back(xs[k]);
					ys_ij.push_back(-1);
					nj++;
				}
			}
			if(ni==0 || nj==0) return false;

			// update the pair classifier (i,j)
			TrainLSVM(dim, xs_ij, ys_ij, c, wbs[iwb]);
			iwb++;
		}
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
// multi-class classification: m classes, m*(m-1)/2 wbs, m sorted labels
bool CFaceClassifier::TestLSVMs(int dim, float *x, int m, vector<float*> &wbs, vector<int> &l)
{
	if(wbs.size()!=m*(m-1)/2) return false;

	l.resize(m);
	if(m==2)
	{
		float f = TestLSVM(dim, x, wbs[0]);
		if(f>=0) l[0] = 0, l[1] = 1;
		else l[0] = 1, l[1] = 0;
	}
	else
	{
		const float a = 3; // smooth coefficient
		float *f = new float[m*m];
		bool  *b = new bool[m];

		// probability matrix
		memset(f, 0, m*m*sizeof(float));
		for(int i=0, k=0; i<m; i++)
		{
			for(int j=i+1; j<m; j++)
			{				
				float fk = TestLSVM(dim, x, wbs[k]);
				fk = Probability(fk, a);
				f[i*m+j] = fk;
				f[j*m+i] = 1-fk;
				k++;
			}
		}

		// sort all candidates
		for(int k=0; k<m; k++) b[k] = true;
		for(int k=m-1; k>=0; k--)
		{
			// find the row with the smallest sum
			int ik = 0;
			float fmin = (float)m, *pf = f;
			for(int i=0; i<m; i++)
			{
				if(b[i])
				{
					float fi = 0;
					for(int j=0; j<m; j++) if(b[j]) fi += pf[j];
					if(fmin>fi) fmin = fi, ik = i;
				}
				pf += m;
			}
			b[ik] = false;
			l[k] = ik;
		}

		delete []f;
		delete []b;
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
// multi-class classification: m classes, m*(m-1)/2 wbs, m sorted labels
bool CFaceClassifier::TestLSVMs(int dim, vector<float*> &xs, int m, vector<float*> &wbs, vector<int> &l)
{
	if(wbs.size()!=m*(m-1)/2 || xs.size()==0) return false;
	const int n = xs.size();

	l.resize(m);
	if(m==2)
	{
		float f = 0;
		for(int i=0; i<n; i++) f += TestLSVM(dim, xs[i], wbs[0]);
		if(f>=0) l[0] = 0, l[1] = 1;
		else l[0] = 1, l[1] = 0;
	}
	else
	{
		const float a = 3; // smooth coefficient
		float *f = new float[m*m];
		bool  *b = new bool[m];

		// probability matrix
		memset(f, 0, m*m*sizeof(float));
		for(int i=0, k=0; i<m; i++)
		{
			for(int j=i+1; j<m; j++)
			{	
				float fk = 0;
				for(int t=0; t<n; t++)
				{
					float temp = TestLSVM(dim, xs[t], wbs[k]);
					fk += Probability(temp, a);
				}
				f[i*m+j] = fk;
				f[j*m+i] = 1-fk;
				k++;
			}
		}

		// sort all candidates
		for(int k=0; k<m; k++) b[k] = true;
		for(int k=m-1; k>=0; k--)
		{
			// find the row with the smallest sum
			int ik = 0;
			float fmin = (float)(m*n), *pf = f;
			for(int i=0; i<m; i++)
			{
				if(b[i])
				{
					float fi = 0;
					for(int j=0; j<m; j++) if(b[j]) fi += pf[j];
					if(fmin>fi) fmin = fi, ik = i;
				}
				pf += m;
			}
			b[ik] = false;
			l[k] = ik;
		}

		delete []f;
		delete []b;
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
bool CFaceClassifier::TrainLSVM(int n, vector<float*> &xs, vector<int> &ys, float c, float *wb)
{
	if(xs.size()!=ys.size()) return false;

	const int m = ys.size();
	float *A = new float[m*m];
	float *a = new float[m];
	byte  *s = new byte[m];

	KernelMatrix(n, xs, ys, 0, 0, A);
	
	const float tol = 0.001f;
	int t = FSMO(m, A, ys, a, c, tol);

	int nsv = 0, i;
	for(i=0; i<m; i++)
	{
		if(a[i]>tol)
		{
			nsv++;
			if(a[i]<c-tol) s[i] = 2;
			else s[i] = 1;
		}
		else s[i] = 0;
	}

	for(i=0; i<n; i++)
	{
		float temp = 0;
		for(int j=0; j<m; j++) if(s[j]>0) temp += ys[j]*a[j]*xs[j][i];
		wb[i] = temp;
	}
	
	float b = 0, nb = 0;
	for(i=0; i<m; i++)
	{
		if(s[i]!=2) continue;
		float temp = 0;
		for(int j=0; j<n; j++) temp += wb[j]*xs[i][j];
		b += ys[i]-temp;
		nb++;
	}
	if(nb>0) b /= nb;
	wb[n] = b;

	delete []A;
	delete []a;
	delete []s;

	printf("linear SVM: %d fsmo iterations, %d support vectors\n", t, nsv);
	return true;
}


//-------------------------------------------------------------------------------------------------
float CFaceClassifier::TestLSVM(int dim, float *x, float *wb)
{
	float f = wb[dim];
	for(int i=0; i<dim; i++) f += wb[i]*x[i];

	return f;
}	


//-------------------------------------------------------------------------------------------------
void CFaceClassifier::Release()
{
	delete this;
}


//-------------------------------------------------------------------------------------------------
// Kernel function
// 0 linear: x'*x2
// 1 polynomial: (1+x1'*x2)^p
// 2 Gaussian: exp(-||x1-x2||^2/(2*r^2))
float CFaceClassifier::KernelFunction(int n, float *x1, float *x2, int ker, float arg)
{
	float f = 0;
	
	switch(ker)
	{
	int i;
	float x12, dx;
    case 0:
        for(i=0; i<n; i++) f += x1[i]*x2[i];
        break;
    
	case 1:
        x12 = 1;
        for(i=0; i<n; i++) x12 += x1[i]*x2[i];
        f = (float)pow(x12, int(arg+0.5));
        break;
    
	case 2:
        dx = 0;
        for(i=0; i<n; i++) dx += (x1[i]-x2[i])*(x1[i]-x2[i]);
        f = (float)exp(-dx/(2*arg*arg));
        break;
    }

    return f;
}


//-------------------------------------------------------------------------------------------------
// Kernel matrix
// Km*m, Xm*n
// kij = yi*yj*k(xi, xj)
void CFaceClassifier::KernelMatrix(int n, vector<float*> &xs, vector<int> &ys, int ker, float arg, float *K)
{
	const int m = ys.size();
	for(int i=0; i<m; i++)
	{
		float *xi = xs[i];
		for(int j=0; j<i; j++)
		{
			float *xj = xs[j];
			K[i*m+j] = K[j*m+i] = (ys[i]*ys[j])*KernelFunction(n, xi, xj, ker, arg); //+/-1
		}
		K[i*m+i] = KernelFunction(n, xi, xi, ker, arg);
	}
}


//-------------------------------------------------------------------------------------------------
// Fast Sequential Minimal Optimization (FSMO)
//    min  Q(x) = 0.5*x'*A*x - 1'*x, 
//    s.t. b'*x = 0, 0 <= x <= xh.
int CFaceClassifier::FSMO(int n, float *A, vector<int> &b, float *x, float xh, float tol)
{
	float *dQ = new float[n];
    bool *b1 = new bool[n];
    bool *b2 = new bool[n];

	//initialize dQ, b1 and b2
	const float ZERO = 0.000001f;
	const float XMAX = xh-ZERO;
	memset(x, 0, n*sizeof(float));
    for(int i=0; i<n; i++)
	{
        dQ[i] = -1;
        for(int j=0; j<n; j++) dQ[i] += A[i*n+j]*x[j];
        if((b[i]<0 && x[i]<XMAX) || (b[i]>0 && x[i]>ZERO)) b1[i] = true;
        else b1[i] = false;
        if((b[i]<0 && x[i]>ZERO) || (b[i]>0 && x[i]<XMAX)) b2[i] = true;
        else b2[i] = false;        
    }

    //begin iterations of x and dQ
	int t;
    for(t=1; t<10000; t++)
	{        
        //find two variables that produce a maximal reduction in Q
        float dQ1 = -1e10, dQ2 = 1e10;
        int i, i1, i2;
        for(i=0; i<n; i++)
		{
            float bdQ = b[i]*dQ[i];
            if(b1[i] && dQ1<bdQ){ dQ1 = bdQ; i1 = i; }
            if(b2[i] && dQ2>bdQ){ dQ2 = bdQ; i2 = i; }
        }
        if(dQ1-dQ2<tol) break;
        
        //optimize x(i1) and x(i2)
        float x1, x2, s;
        s = (float)(b[i1]*b[i2]);
        x2 = x[i2] + b[i2]*(dQ1-dQ2)/(A[i1*n+i1]-2*s*A[i1*n+i2]+A[i2*n+i2]);
        
        //check its bounds
        float L, H;
        if(s<0)
		{
            L = max((float)0,x[i2]-x[i1]);
            H = min(xh,xh+x[i2]-x[i1]);
        }else
		{
            L = max((float)0,x[i2]+x[i1]-xh);
            H = min(xh,x[i2]+x[i1]);
        }
        if(x2<L) x2 = L;
        if(x2>H) x2 = H;
        if(s<0) x1 = x[i1]-x[i2]+x2;
        else x1 = x[i1]+x[i2]-x2;
        
        //update gradient dQ and solution x
        float dx1 = x1-x[i1], dx2 = x2-x[i2];
        for(i=0; i<n; i++) dQ[i] += dx1*A[i*n+i1]+dx2*A[i*n+i2];
        x[i1] = x1;
        x[i2] = x2;
        
        //update b1 and b2
        if((b[i1]<0 && x1<XMAX) || (b[i1]>0 && x1>ZERO)) b1[i1] = true;
        else b1[i1] = false;
        if((b[i1]<0 && x1>ZERO) || (b[i1]>0 && x1<XMAX)) b2[i1] = true;
        else b2[i1] = false;                
        if((b[i2]<0 && x2<XMAX) || (b[i2]>0 && x2>ZERO)) b1[i2] = true;
        else b1[i2] = false;
        if((b[i2]<0 && x2>ZERO) || (b[i2]>0 && x2<XMAX)) b2[i2] = true;
        else b2[i2] = false;        
    }

	delete []b1;
	delete []b2;
	delete []dQ;

	return t;
}


//---------------------------------------------------------------------------------------
// probability function p = 1/(1+exp(-a*f))
float CFaceClassifier::Probability(float f, float a)
{
	if(f>=0) f = float(1/(1+exp(-a*f)));
	else
	{
		f = (float)exp(a*f);
		f = f/(1+f);
	}	
	return f;
}
