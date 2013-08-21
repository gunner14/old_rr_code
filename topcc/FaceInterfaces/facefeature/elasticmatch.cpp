#include "facefeatureimpl.h"
#include <memory.h>
#include <math.h>


//-------------------------------------------------------------------------------------------------
void CFaceFeature::FaceToLBP82(int w, int h, byte *im, byte *lbp)
{
	// find 58 uniform patterns
	int LBP[58];
	int i, j, k = 0;
	for(i=0; i<256; i++)
	{
		int diff = 0;
		if((i&1)!=((i&2)>>1)) diff++;
		if((i&2)!=((i&4)>>1)) diff++;
		if((i&4)!=((i&8)>>1)) diff++;
		if((i&8)!=((i&16)>>1)) diff++;
		if((i&16)!=((i&32)>>1)) diff++;
		if((i&32)!=((i&64)>>1)) diff++;
		if((i&64)!=((i&128)>>1)) diff++;
		if(((i&128)>>7)!=(i&1)) diff++;
		if(diff<=2) LBP[k++] = i;
	}

	// get LBP histogram in each cell
	int i1 = h-2, j1 = w-2, w2 = w*2;
	const double sqrt2 = sqrt(2.0);
	const double c0 = (sqrt2-1)*(sqrt2-1); // far
	const double c1 = (2-sqrt2)*(sqrt2-1); // middle
	const double c2 = (2-sqrt2)*(2-sqrt2); // near
	byte *pim = im+w2, *plbp = lbp+w2;
	memset(lbp, 0, w*h);
	for(i=2; i<i1; i++)
	{
		for(j=2; j<j1; j++)
		{
			// LBP pattern
			byte *p = pim+j;
			byte center = p[0];
			int bit0 = p[-2]>=center ? 1 : 0;

			double temp = c0*p[w2-2]+c1*(p[w2-1]+p[w-2])+c2*p[w-1];
			int bit1 = temp>=center ? 1 : 0;

			int bit2 = p[w2]>=center ? 1 : 0;

			temp = c0*p[w2+2]+c1*(p[w2+1]+p[w+2])+c2*p[w+1];
			int bit3 = temp>=center ? 1 : 0;

			int bit4 = p[2]>=center ? 1 : 0;

			temp = c0*p[-w2+2]+c1*(p[-w2+1]+p[-w+2])+c2*p[-w+1];
			int bit5 = temp>=center ? 1 : 0;

			int bit6 = p[-w2]>=center ? 1 : 0;

			temp = c0*p[-w2-2]+c1*(p[-w2-1]+p[-w-2])+c2*p[-w-1];
			int bit7 = temp>=center ? 1 : 0;

			int lbp = bit0+(bit1<<1)+(bit2<<2)+(bit3<<3)+(bit4<<4)+(bit5<<5)+(bit6<<6)+(bit7<<7);

			// find its index
			for(k=0; k<58; k++){ if(lbp==LBP[k]) break; }

			plbp[j] = k;
		}
		pim += w, plbp += w;
	}
}


//-------------------------------------------------------------------------------------------------
double CFaceFeature::FindPercentile(double p, int n, double *f)
{
	if(p<=0.5)
	{
		int np = int(p*n+0.5);
		for(int i=0; i<np; i++)
		{
			for(int j=i+1; j<n; j++)
			{
				if(f[i]>f[j])
				{
					double temp = f[i];
					f[i] = f[j];
					f[j] = temp;
				}
			}
		}
		return f[np-1];
	}
	else
	{
		int np = int((1-p)*n+1);
		for(int i=0; i<np; i++)
		{
			for(int j=i+1; j<n; j++)
			{
				if(f[i]<f[j])
				{
					double temp = f[i];
					f[i] = f[j];
					f[j] = temp;
				}
			}
		}
		return f[np-1];
	}
}


//-------------------------------------------------------------------------------------------------
bool CFaceFeature::GetLBPHist(int w, int h, byte *lbp, int x, int y, int rw, int *hist)
{
	int x0 = x-rw, x1 = x+rw;
	int y0 = y-rw, y1 = y+rw;
	if(x0<2 || x1>=w-2 || y0<2 || y1>=h-2) return false;

	memset(hist, 0, 59*sizeof(int));
	byte *p = lbp+y0*w;
	for(int i=y0; i<=y1; i++)
	{
		for(int j=x0; j<=x1; j++)
		{
			hist[p[j]]++;
		}
		p += w;
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
double CFaceFeature::HistogramMatch(int n, int *hist1, int *hist2, int sum)
{
	double f = 0;
	for(int i=0; i<n; i++) f += sqrt((double)hist1[i]*(double)hist2[i])/sum;
	return f;
}


//-------------------------------------------------------------------------------------------------
// rw - window radius
// sw - sampling size of window
// rn - neighborhood
// sn - searching step size neighborhoods
// a  - percentile
double CFaceFeature::ElasticMatch(int w, int h, byte *lbp1, byte *lbp2, int rw, int sw, int rn, int sn, double a)
{
	int dw = 2*rw+1, nw = (w-4-dw)/sw, nh = (h-4-dw)/sw, n = 0;
	int j0 = (w-nw*sw)/2, i0 = (h-nh*sw)/2, sum = dw*dw;
	double *f = new double[(nw+1)*(nh+1)];

	for(int i=0; i<=nh; i++)
	{
		for(int j=0; j<=nw; j++)
		{
			//lbp1[(i0+i*sw)*w+(j0+j*sw)] = 255;
			int x = j0+j*sw, y = i0+i*sw;
			int hist1[59], hist2[59];
			GetLBPHist(w, h, lbp1, x, y, rw, hist1);

			// search the best matching in its neighborhoods
			double fmax = 0;
			int u0 = y-rn*sn, u1 = y+rn*sn;
			int v0 = x-rn*sn, v1 = x+rn*sn;
			for(int u=u0; u<=u1; u+=sn)
			{
				for(int v=v0; v<=v1; v+=sn)
				{
					if(GetLBPHist(w, h, lbp2, v, u, rw, hist2))
					{
						double fuv = HistogramMatch(59, hist1, hist2, sum);
						if(fmax<fuv) fmax = fuv;
					}
				}
			}

			f[n] = fmax;
			n++;
		}
	}

	double fa = FindPercentile(a, n, f);

	delete []f;

	return fa;
}
