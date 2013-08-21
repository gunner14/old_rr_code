#include "facefeatureimpl.h"
#include "facelandmark.h"
#include <assert.h>
#include <memory.h>
#include <math.h>

//-------------------------------------------------------------------------------------------------
IFaceFeature* IFaceFeature::CreateInstance(char *pathname)
{
	CFaceFeature *fc = new CFaceFeature();
	if(fc->LoadFaceAsm(pathname)) return (IFaceFeature*)fc;
	else
	{
		delete fc;
		return 0;
	}
}


//-------------------------------------------------------------------------------------------------
CFaceFeature::CFaceFeature()
{
	m_pFaceAsm = 0;
}


//-------------------------------------------------------------------------------------------------
CFaceFeature::~CFaceFeature()
{
	if(m_pFaceAsm!=0) FaceAsmRelease(&m_pFaceAsm);
}


//-------------------------------------------------------------------------------------------------
bool CFaceFeature::LoadFaceAsm(char *pathname)
{
	return FaceAsmCreate(&m_pFaceAsm, pathname);
}


//-------------------------------------------------------------------------------------------------
int CFaceFeature::GetFeatureLength()
{
#ifdef GRY_FEATURE 
	return GRY_D;
#endif
#ifdef LBP_FEATURE
	return LBP_D;
#endif
#ifdef HOG_FEATURE
	return HOG_D;
#endif
#ifdef LBP_ELASTIC
	return (STD_W*STD_H);
#endif
}


//-------------------------------------------------------------------------------------------------
void CFaceFeature::BgraToGray(int w, int h, byte *bgra, byte *gray)
{
	const int wh = w*h;
	byte *pbgra = bgra;
	for(int i=0; i<wh; i++)
	{
		gray[i] = (114*pbgra[0]+587*pbgra[1]+299*pbgra[2])/1000;
		pbgra += 4;
	}
}


//-------------------------------------------------------------------------------------------------
bool CFaceFeature::ExtractFeature(int w, int h, byte *gray, int rx0, int ry0, int rx1, int ry1, 
								  float *x)
{

	byte  face[STD_W*STD_H];

#ifdef STD_2P

	//int pt[152], eye[4]; // positions of two eyes
	int  eye[4]; // positions of two eyes
  float pt[152];
	FaceAsmLandmarks(m_pFaceAsm, w, w, h, gray, (rx0+rx1)/2, (ry0+ry1)/2, rx1-rx0+1, pt);
	eye[0] = (pt[54]+pt[56]+pt[58]+pt[60])/4; // 54~60
	eye[1] = (pt[55]+pt[57]+pt[59]+pt[61])/4;
	eye[2] = (pt[64]+pt[66]+pt[68]+pt[70])/4; // 64~70
	eye[3] = (pt[65]+pt[67]+pt[69]+pt[71])/4;

	double dx = eye[2]-eye[0], dy = eye[3]-eye[1];
	double a  = 45*atan(dy/dx)/atan(1.0);
	double s  = 2.4*sqrt(dx*dx+dy*dy)/STD_W;
	int x0 = (eye[0]+eye[2])/2, y0 = (eye[1]+eye[3])/2;
	int x1 = int(0.5*STD_W),  y1 = int(0.25*STD_H);

	RotateRescaleFace(w, h, gray, x0, y0, STD_W, STD_H, face, x1, y1, -a, s);

#else

	int pt[152], pt3[6]; // positions of two eyes
	FaceAsmLandmarks(m_pFaceAsm, w, w, h, gray, (rx0+rx1)/2, (ry0+ry1)/2, rx1-rx0+1, pt);
	pt3[0] = (pt[54]+pt[56]+pt[58]+pt[60])/4; // 54~60
	pt3[1] = (pt[55]+pt[57]+pt[59]+pt[61])/4;
	pt3[2] = (pt[64]+pt[66]+pt[68]+pt[70])/4; // 64~70
	pt3[3] = (pt[65]+pt[67]+pt[69]+pt[71])/4;
	pt3[4] = pt[82]; // 41
	pt3[5] = pt[83];

	double tri0[6] = {pt3[0], pt3[1], pt3[2], pt3[3], pt3[4], pt3[5]};
	double tri1[6] = {0.25*(w-1), 0.2*(h-1), 0.75*(w-1), 0.2*(h-1), 0.5*(w-1), 0.6*(h-1)};

	AffineTransform(w, h, gray, STD_W, STD_H, face, tri0, tri1);

#endif

	GaussianSmooth(STD_W, STD_H, face);

#ifdef GRY_FEATURE	
	byte faceds[GRY_D];
	DownSample(STD_W, STD_H, face, GRY_C, faceds);
	double sum = 0;
	for(int i=0; i<GRY_D; i++) sum += faceds[i];
	for(int i=0; i<GRY_D; i++) x[i] = float(faceds[i]/sum);
#endif

#ifdef LBP_FEATURE
	LBP82(STD_W, STD_H, face, LBP_C, x);
#endif
#ifdef HOG_FEATURE
	HOG8(STD_W, STD_H, face, HOG_C, x);
#endif
#ifdef LBP_ELASTIC
	byte facelbp[STD_W*STD_H];
	FaceToLBP82(STD_W, STD_H, face, facelbp);
	for(int i=0; i<STD_W*STD_H; i++) x[i] = facelbp[i];
#endif

//	delete []gray;

	return true;
}

//-------------------------------------------------------------------------------------------------

//-----------------------------------|

bool  CFaceFeature::extractFeature(byte* imageData, int imgWidth, int imgHeight,
								   std::vector<rect> faceRegion, std::vector<float*>& featureBuffer)
{

	byte  face[STD_W*STD_H];
	int w = imgWidth;
	int h = imgHeight;
	int rx0, ry0, rx1, ry1;

#ifdef STD_2P

	//int pt[152], eye[4]; // positions of two eyes
	int eye[4]; // positions of two eyes
  float pt[152];
	for (int faceNum = 0; faceNum < faceRegion.size(); faceNum++){
		memset(face, 0, sizeof(face));
		float* x = new float[LBP_D];

		rx0 = faceRegion[faceNum].x0;
		ry0 = faceRegion[faceNum].y0;
		rx1 = faceRegion[faceNum].x1;
		ry1 = faceRegion[faceNum].y1;

		FaceAsmLandmarks(m_pFaceAsm, w, w, h, imageData, (rx0+rx1)/2, (ry0+ry1)/2, rx1-rx0+1, pt);

		eye[0] = (pt[54]+pt[56]+pt[58]+pt[60])/4; // 54~60
		eye[1] = (pt[55]+pt[57]+pt[59]+pt[61])/4;
		eye[2] = (pt[64]+pt[66]+pt[68]+pt[70])/4; // 64~70
		eye[3] = (pt[65]+pt[67]+pt[69]+pt[71])/4;

		double dx = eye[2]-eye[0], dy = eye[3]-eye[1];
		double a  = 45*atan(dy/dx)/atan(1.0);
		double s  = 2.4*sqrt(dx*dx+dy*dy)/STD_W;
		int x0 = (eye[0]+eye[2])/2, y0 = (eye[1]+eye[3])/2;
		int x1 = int(0.5*STD_W),  y1 = int(0.25*STD_H);

		RotateRescaleFace(w, h, imageData, x0, y0, STD_W, STD_H, face, x1, y1, -a, s);

		#else

			int pt[152], pt3[6]; // positions of two eyes
			FaceAsmLandmarks(m_pFaceAsm, w, w, h, imageData, (rx0+rx1)/2, (ry0+ry1)/2, rx1-rx0+1, pt);
			pt3[0] = (pt[54]+pt[56]+pt[58]+pt[60])/4; // 54~60
			pt3[1] = (pt[55]+pt[57]+pt[59]+pt[61])/4;
			pt3[2] = (pt[64]+pt[66]+pt[68]+pt[70])/4; // 64~70
			pt3[3] = (pt[65]+pt[67]+pt[69]+pt[71])/4;
			pt3[4] = pt[82]; // 41
			pt3[5] = pt[83];

			double tri0[6] = {pt3[0], pt3[1], pt3[2], pt3[3], pt3[4], pt3[5]};
			double tri1[6] = {0.25*(w-1), 0.2*(h-1), 0.75*(w-1), 0.2*(h-1), 0.5*(w-1), 0.6*(h-1)};

			AffineTransform(w, h, imageData, STD_W, STD_H, face, tri0, tri1);

		#endif

			GaussianSmooth(STD_W, STD_H, face);

		#ifdef GRY_FEATURE	
			byte faceds[GRY_D];
			DownSample(STD_W, STD_H, face, GRY_C, faceds);
			double sum = 0;
			for(int i=0; i<GRY_D; i++) sum += faceds[i];
			for(int i=0; i<GRY_D; i++) x[i] = float(faceds[i]/sum);
		#endif

		#ifdef LBP_FEATURE
			LBP82(STD_W, STD_H, face, LBP_C, x);
		#endif
		#ifdef HOG_FEATURE
			HOG8(STD_W, STD_H, face, HOG_C, x);
		#endif
		#ifdef LBP_ELASTIC
			byte facelbp[STD_W*STD_H];
			FaceToLBP82(STD_W, STD_H, face, facelbp);
			for(int i=0; i<STD_W*STD_H; i++) x[i] = facelbp[i];
		#endif

			//delete []gray;
			featureBuffer.push_back(x);
		}
		return true;
}
//-----------------------------------|

//-------------------------------------------------------------------------------------------------
void CFaceFeature::DownSample(int w, int h, byte *im0, int d, byte *im1)
{
	int w1 = w/d, h1 = h/d, dd = d*d;
	for(int i=0; i<h1; i++)
	{
		for(int j=0; j<w1; j++)
		{
			int temp = 0;
			int u0 = i*d, u1 = u0+d, v0 = j*d, v1 = v0+d;
			for(int u=u0; u<u1; u++)
			{
				for(int v=v0; v<v1; v++) temp += im0[u*w+v];
			}
			im1[i*w1+j] = temp/dd;
		}
	}
}


//-------------------------------------------------------------------------------------------------
void CFaceFeature::RotateRescaleFace(int w0, int h0, byte *im0, int x0, int y0, 
					                 int w1, int h1, byte *im1, int x1, int y1, 
					                 double a, double s)
{
	// x' = x0+(x-x1)*s*cosa+(y-y1)*s*sina
	// y' = y0-(x-x1)*s*sina+(y-y1)*s*cosa	
	// angle to radian
	a = -a*atan(1.0)/45;
	double cosa = s*cos(a), sina = s*sin(a);

	// rotate the image
	const int w00 = w0-1, w01 = w0+1, h00 = h0-1;
	byte *pim1 = im1;
	for(int i=0; i<h1; i++)
	{
		double x = x0-x1*cosa+(i-y1)*sina;
		double y = y0+x1*sina+(i-y1)*cosa;
		for(int j=0; j<w1; j++)
		{
			// do bilinear interpolation
			if(0<=x && x<w00 && 0<=y && y<h00)
			{
				byte *pim0 = im0+w0*int(y)+int(x);
				double dx = x-int(x), dy = y-int(y);
				pim1[j] = byte((1-dy)*((1-dx)*pim0[0]+dx*pim0[1])+dy*((1-dx)*pim0[w0]+dx*pim0[w01]));
			}
			//else pim1[j] = 128;
			else
			{
				double xm = x, ym = y;
				if(xm<0) xm = -xm;
				else if(xm>=w00) xm = 2*w00-xm-0.0001;
				if(ym<0) ym = -ym;
				else if(ym>=h00) ym = 2*h00-ym-0.0001;

				byte *pim0 = im0+w0*int(ym)+int(xm);
				double dx = xm-int(xm), dy = ym-int(ym);
				pim1[j] = byte((1-dy)*((1-dx)*pim0[0]+dx*pim0[1])+dy*((1-dx)*pim0[w0]+dx*pim0[w01]));
			}

			// update (x,y)
			x += cosa, y -= sina;
		}
		pim1 += w1;
	}
}


//-------------------------------------------------------------------------------------------------
// P*A*x = P*b, P*A = L*U
bool CFaceFeature::SolveLinearSystem2(int n, double *A, double *b, double *x)
{
	// P*A = L*U, b' = P*b
	int i, j, k;
	for(i=0; i<n-1; i++)
	{
		// find the maximum |A(i:n, i)|
		j = i;
		for(k=i+1; k<n; k++) if(fabs(A[j*n+i])<fabs(A[k*n+i])) j = k;
		if(fabs(A[j*n+i])<1e-6) return false;

		// swap the two rows A(i,:) and A(j,:) and two elements b(i) and b(j)
		if(j>i)
		{
			for(k=0; k<n; k++)
			{
				double temp = A[i*n+k];
				A[i*n+k] = A[j*n+k];
				A[j*n+k] = temp;
			}
			double temp = b[i];
			b[i] = b[j];
			b[j] = temp;
		}

		// update A(i+1:n,i) /= A(i,i)
		for(k=i+1; k<n; k++) A[k*n+i] /= A[i*n+i];

		// update A(i+1:n,i+1:n) -= A(i+1:n,i)*A(i,i+1:n)
		for(j=i+1; j<n; j++)
		{
			for(k=i+1; k<n; k++) A[j*n+k] -= A[j*n+i]*A[i*n+k];
		}
	}

	// Solve L*y = b'
	for(i=0; i<n; i++)
	{
		x[i] = b[i];
		for(j=0; j<i; j++) x[i] -= x[j]*A[i*n+j];
	}	

	// Solve U*x = y
	for(i=n-1; i>=0; i--)
	{
		for(j=i+1; j<n; j++) x[i] -= x[j]*A[i*n+j];
		x[i] /= A[i*n+i];
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
// tri0 - source triangular
// tri1 - target triangular
void CFaceFeature::AffineTransform(int w0, int h0, byte *im0, int w1, int h1, byte *im1, double *tri0, double *tri1)
{
	// inverse mapping from target to source
	const int n = 6;
	double A[n*n] =
	{
		tri1[0], tri1[1], 1, 0, 0, 0,
		0, 0, 0, tri1[0], tri1[1], 1,
		tri1[2], tri1[3], 1, 0, 0, 0,
		0, 0, 0, tri1[2], tri1[3], 1,
		tri1[4], tri1[5], 1, 0, 0, 0,
		0, 0, 0, tri1[4], tri1[5], 1
	};
	double b[n] = {tri0[0], tri0[1], tri0[2], tri0[3], tri0[4], tri0[5]};
	double x[n];

	// Solve A*x = b
	SolveLinearSystem2(n, A, b, x);
	double a11 = x[0], a12 = x[1], a13 = x[2];
	double a21 = x[3], a22 = x[4], a23 = x[5];

	// Do affine transform
	byte *pim1 = im1;
	for(int i=0; i<h1; i++)
	{
		// x' = a11*x+a12*y+a13
		// y' = a21*x+a22*y+a23
		double x = a12*i+a13;
		double y = a22*i+a23;
		for(int j=0; j<w1; j++)
		{
			if(0<=x && x<w0-1 && 0<=y && y<h0-1)
			{
				byte *pim0 = im0+int(y)*w0+int(x);
				double cx = x-int(x), cy = y-int(y);
				pim1[j] = byte((1-cy)*((1-cx)*pim0[0]+cx*pim0[1])+cy*((1-cx)*pim0[w0]+cx*pim0[w0+1])+0.5);
			}
			else pim1[j] = 0;
			x += a11, y += a21;
		}
		pim1 += w1;
	}
}


//-------------------------------------------------------------------------------------------------
void CFaceFeature::GaussianSmooth(int w, int h, byte *im)
{
	// 1 2 1
	// 2 4 2  =  16
	// 1 2 1
	byte *im0 = new byte[w*h];
	memcpy(im0, im, w*h);

	const int i1 = h-1, j1 = w-1;
	byte *pim = im+w, *pim0 = im0+w;
	for(int i=1; i<i1; i++)
	{
		for(int j=1; j<j1; j++)
		{
			byte *p = pim0+j;
			pim[j] = (4*p[0]+2*(p[-1]+p[1]+p[-w]+p[w])+(p[-w-1]+p[-w+1]+p[w-1]+p[w+1]))/16;
		}
		pim += w, pim0 += w;
	}

	delete []im0;
}


//-------------------------------------------------------------------------------------------------
float CFaceFeature::GetSimilarity(float *x0, float *x1, float *w, int metric)
{
	double temp = 0;

	const int d = GetFeatureLength();
	switch(metric)
	{
	case 0: // Bhattacharyya coefficient
		if(w==0)
		{
			//for(int i=0; i<d; i++) temp += sqrt(x0[i]*x1[i]);
			for(int i=0; i<d; i++) temp += x0[i]*x1[i]; // modified 20120913
		}
		else
		{
			double sum0 = 0, sum1 = 0;
			for(int i=0; i<d; i++)
			{
				if(w[i]>0)
				{
					sum0 += w[i]*x0[i];
					sum1 += w[i]*x1[i];
					temp += w[i]*sqrt(x0[i]*x1[i]);
				}
			}
			temp /= sqrt(sum0*sum1);
		}
		break;

	case 1: // Correlation coefficient
		if(w==0)
		{
			double temp0 = 0, temp1 = 0;
			for(int i=0; i<d; i++)
			{
				temp  += x0[i]*x1[i];
				temp0 += x0[i]*x0[i];
				temp1 += x1[i]*x1[i];
			}
			temp /= sqrt(temp0*temp1);
		}
		else
		{
			double temp0 = 0, temp1 = 0;
			for(int i=0; i<d; i++)
			{
				if(w[i]>0)
				{
					temp  += w[i]*x0[i]*x1[i];
					temp0 += w[i]*x0[i]*x0[i];
					temp1 += w[i]*x1[i]*x1[i];
				}
			}
			temp /= sqrt(temp0*temp1);
		}
		break;

	case 2: // City block distance
		if(w==0)
		{
			for(int i=0; i<d; i++) temp += fabs(x0[i]-x1[i]);
		}
		else
		{
			for(int i=0; i<d; i++)
			{
				if(w[i]>0) temp += w[i]*fabs(x0[i]-x1[i]);
			}
		}
		break;

	case 3: // Euclidean distance
		if(w==0)
		{
			for(int i=0; i<d; i++)
			{
				double tempi = x0[i]-x1[i];
				temp += tempi*tempi;
			}
		}
		else
		{
			for(int i=0; i<d; i++)
			{
				if(w[i]>0)
				{
					double tempi = w[i]*(x0[i]-x1[i]);
					temp += tempi*tempi;
				}
			}
		}
		temp = sqrt(temp);
		break;

	case 4: // robust elastic matching
		assert(d==STD_W*STD_H);
		byte lbp0[STD_W*STD_H], lbp1[STD_W*STD_H];
		for(int i=0; i<d; i++)
		{
			lbp0[i] = (byte)x0[i];
			lbp1[i] = (byte)x1[i];
		}
		double temp0 = ElasticMatch(STD_W, STD_H, lbp0, lbp1, 8, 8, 4, 2, 0.2);
		double temp1 = ElasticMatch(STD_W, STD_H, lbp1, lbp0, 8, 8, 4, 2, 0.2);
		temp = std::min(temp0, temp1);
		break;
	}

	return (float)temp;
}


//-------------------------------------------------------------------------------------------------
void CFaceFeature::Release()
{
	delete this;
}
