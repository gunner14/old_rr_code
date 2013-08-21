#include "facefeatureimpl.h"
#include <memory.h>
#include <math.h>


//-------------------------------------------------------------------------------------------------
inline void CFaceFeature::SoftVote(int x, int y, double f, double fx, double fy, int wv, int hv, float *fv)
{
	if(0<=x && 0<=y)     fv[y*wv+x]       += (float)(f*(1-fx)*(1-fy));
	if(x+1<wv && 0<=y)   fv[y*wv+x+1]     += (float)(f*fx*(1-fy));
	if(0<=x && y+1<hv)   fv[(y+1)*wv+x]   += (float)(f*(1-fx)*fy);
	if(x+1<wv && y+1<hv) fv[(y+1)*wv+x+1] += (float)(f*fx*fy);	
}


//-------------------------------------------------------------------------------------------------
void CFaceFeature::HOG8(int w, int h, byte *im, int dv, float *fv, byte *mask)
{
	const int wv = w/dv, hv = h/dv;
	
	int dim = 8*wv*hv;
	memset(fv, 0, dim*sizeof(float));
	
	const double sqrt2 = sqrt(2.0);
	float *fv0 = fv,        *fv1 = fv0+wv*hv, *fv2 = fv1+wv*hv, *fv3 = fv2+wv*hv;
	float *fv4 = fv3+wv*hv, *fv5 = fv4+wv*hv, *fv6 = fv5+wv*hv, *fv7 = fv6+wv*hv;
	int i, i1 = h-1, wl = w-1, wr = w+1;
	for(i=1; i<i1; i++)
	{
		double temp = (i+0.5)/dv-0.5;
		int y = int(temp+1)-1;
		double fy = temp-y;
		
		for(int j=1; j<wl; j++)
		{
			if(mask!=0 && mask[i*w+j]==0) continue;

			temp = (j+0.5)/dv-0.5;
			int x = int(temp+1)-1;
			double fx = temp-x;
			
			byte *pim1 = im+i*w+j;
			int gh = (pim1[-wr]+2*pim1[-1]+pim1[wl])-(pim1[-wl]+2*pim1[ 1]+pim1[ wr]);
			int gv = (pim1[ wl]+2*pim1[ w]+pim1[wr])-(pim1[-wr]+2*pim1[-w]+pim1[-wl]);
			
			if(gh==0)
			{
				if(gv>=0) SoftVote(x, y,  gv, fx, fy, wv, hv, fv2);
				else      SoftVote(x, y, -gv, fx, fy, wv, hv, fv6);
			}
			else if(gv==0)
			{
				if(gh>=0) SoftVote(x, y,  gh, fx, fy, wv, hv, fv0);
				else      SoftVote(x, y, -gh, fx, fy, wv, hv, fv4);
			}
			else if(gh*gv>0)
			{
				bool pn = gh>0 ? true : false; // positive or negative
				if(gh<0) gh = -gh, gv = -gv;
				if(gh>=gv)
				{
					SoftVote(x, y, gh-gv,    fx, fy, wv, hv, pn?fv0:fv4);
					SoftVote(x, y, sqrt2*gv, fx, fy, wv, hv, pn?fv1:fv5);
				}
				else
				{
					SoftVote(x, y, sqrt2*gh, fx, fy, wv, hv, pn?fv1:fv5);
					SoftVote(x, y, gv-gh,    fx, fy, wv, hv, pn?fv2:fv6);
				}
			}
			else
			{
				bool pn = gv>0 ? true : false;
				if(gh<0) gh = -gh;
				else gv = -gv;
				if(gh>=gv)
				{
					SoftVote(x, y, gh-gv,    fx, fy, wv, hv, pn?fv4:fv0);
					SoftVote(x, y, sqrt2*gv, fx, fy, wv, hv, pn?fv3:fv7);
				}
				else
				{
					SoftVote(x, y, sqrt2*gh, fx, fy, wv, hv, pn?fv3:fv7);
					SoftVote(x, y, gv-gh,    fx, fy, wv, hv, pn?fv2:fv6);
				}
			}
		}
	}
	
	float sum = 0;
	for(i=0; i<dim; i++) sum += fv[i];
	if(sum>0){ for(i=0; i<dim; i++) fv[i] /= sum; }	
}
