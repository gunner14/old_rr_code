#include "facefeatureimpl.h"
#include <memory.h>
#include <math.h>


//-------------------------------------------------------------------------------------------------
void CFaceFeature::LBP82(int w, int h, byte *im, int d, float *fv, byte *mask)
{
	int wcell = w/d, hcell = h/d, dim = wcell*hcell*59;
	int LBP[58];
	
	// find 58 uniform patterns
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
	memset(fv, 0, dim*sizeof(float));
	int i1 = h-2, j1 = w-2, w2 = w*2, r = d/2;
	const double sqrt2 = sqrt(2.0);
	const double c0 = (sqrt2-1)*(sqrt2-1); // far
	const double c1 = (2-sqrt2)*(sqrt2-1); // middle
	const double c2 = (2-sqrt2)*(2-sqrt2); // near
	byte *pim = im+w2;
	for(i=2; i<i1; i++)
	{
		int ycell = (i-r+d)/d-1, cy = (i-r)-ycell*d;
		for(j=2; j<j1; j++)
		{
			if(mask!=0 && mask[i*w+j]==0) continue;

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
			
			// vote it into the histogram
			int xcell = (j-r+d)/d-1, cx = (j-r)-xcell*d;
			
			if(xcell>=0 && ycell>=0) fv[(ycell*wcell+xcell)*59+k] += (d-cx)*(d-cy);
			if(xcell+1<wcell && ycell>=0) fv[(ycell*wcell+xcell+1)*59+k] += cx*(d-cy);
			if(xcell>=0 && ycell+1<hcell) fv[((ycell+1)*wcell+xcell)*59+k] += (d-cx)*cy;
			if(xcell+1<wcell && ycell+1<hcell) fv[((ycell+1)*wcell+xcell+1)*59+k] += cx*cy;
		}
		pim += w;
	}
	
	// normalize the sum
	double sum = 0;
	for(i=0; i<dim; i++) sum += fv[i];
	//for(i=0; i<dim; i++) fv[i] = float(fv[i]/sum);
	for(i=0; i<dim; i++) fv[i] = (float)sqrt(fv[i]/sum); // modified 20120913
}

