#include <windows.h>
#include "stasm.hpp"
#include "imwrite.hpp"
#include "landmarks.h"
#include "../rowley/find.hpp"


//-------------------------------------------------------------------------------------------------
int WINAPI DllMain (HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
extern void InitEye(const char sDataDir[]);


//-------------------------------------------------------------------------------------------------
EXPORT bool API FaceAsmCreate(void **pp)
{
	static const int NMODELS_MAX = 2;  // max number of stacked models
	static ASM_MODEL Models[NMODELS_MAX];

	char sConfFile0[SLEN], sConfFile1[SLEN];
	strcpy(sConfFile0, "landmarks/mu-68-1d.conf");
    strcpy(sConfFile1, "landmarks/mu-76-2d.conf");
 
	const int nInitModels = nInitAsmModels(Models, sConfFile0, sConfFile1);

	InitEye("landmarks");
	
	if(nInitModels==0)
	{
		*pp = 0;
		return false;
	}
	else
	{
		*pp = Models;
		return true;
	}
}


//-------------------------------------------------------------------------------------------------
typedef struct SEARCH_IMAGES    // the images for one pyr level during a search
{
    Image  Img;                 // the input image, scaled to current pyramid level
    Mat    Grads;               // image gradients
}
SEARCH_IMAGES;


//-------------------------------------------------------------------------------------------------
extern void AsmLevSearch (SHAPE &Shape, SEARCH_IMAGES &SearchImgs, const ASM_MODEL &Model, int iLev, 
						  const LANDMARK LandTab[], float *fB = 0);


//-------------------------------------------------------------------------------------------------
// (w,s)     - width and width step, e.g. s = (3*w+3)/4*4
// h         - height
// data[s*h] - image pixels data[s*h]
// (x,y)     - face center
// d         - face size d*d
// point     - output landmark points
EXPORT int  API FaceAsmLandmarks(void *p, int w, int s, int h, byte *data, int x, int y, int d, 
								 int *point, float *b)
{
	// crop the face image
	int x0 = max(0, x-d), x1 = min(x+d, w-1);
	int y0 = max(0, y-d), y1 = min(y+d, h-1);
	int wf = x1-x0+1, hf = y1-y0+1;
	
	Image image(wf, hf);
	byte *buf = image.buf;
	
	if(s>=3*w) // input a color image
	{
		byte *pbgr = data+y0*s+3*x0;
		for(int i=0; i<hf; i++)
		{
			byte *pbgr1 = pbgr;
			for(int j=0; j<wf; j++)
			{
				buf[j] = (114*pbgr1[0]+587*pbgr1[1]+299*pbgr1[2])/1000; // color to gray
				pbgr1 += 3;
			}
			buf+=wf, pbgr+=s;
		}
	}
	else // input a gray image
	{
		byte *pgray = data+y0*s+x0;
		for(int i=0; i<hf; i++)
		{
			memcpy(buf, pgray, wf);
			buf+=wf, pgray+=s;
		}
	}

	ASM_MODEL *Models = (ASM_MODEL*)p;
	const int nModels = 2;
					
    DET_PARAMS DetParams; // for AsmSearch
	DetParams.x = x-(x0+x1)/2;
	DetParams.y = (y0+y1)/2-y;
	DetParams.width  = d;
	DetParams.height = d;
	
	// CombinedShape is created by combining shapes from each model search.
	// It has the same number of points as Models[0].FileMeanShape, so if
	// Models[1] has extra points they are discarded after the Models[1] search.
	SHAPE StartShape, Shape;
	SHAPE CombinedShape;
	
	unsigned DetAttr = FA_ViolaJones;   // specifies which face detector to use
	SHAPE DetAv = Models[0].VjAv;
	if (0)//fRowley)
    {
		DetAttr = FA_Rowley;
		DetAv = Models[0].RowleyAv;
    }
	if(fGetStartShape(StartShape, DetParams, image, Models[0].FileMeanShape, 
	   DetAttr, DetAv, 0, "landmarks", CONF_fStasmSkipIfNotInShapeFile, false))
    {
		Shape = StartShape;
		
		for(int iModel=0; iModel<nModels; iModel++)
        {
			ASM_MODEL *pModel = &Models[iModel];
			if(iModel!=0) GetStartShapeFromPreviousSearch(Shape, CombinedShape, pModel->FileMeanShape);
			
			// Scale Shape and Img, so the face width is nStandardFaceWidth,
			// using the start shape to approximate the face width.
			
			double ImageScale = pModel->nStandardFaceWidth/xShapeExtent(Shape);
			SHAPE Shape1(Shape*ImageScale); // working shape
			Image image1(image); // working Img
			int nNewWidth  = iround(image1.width * ImageScale);
			int nNewHeight = iround(image1.height * ImageScale);
			ScaleImage(image1, nNewWidth, nNewHeight, IM_BILINEAR);
			
			// dimKeep is needed when this model has different number
			// of landmarks from previous model
			Shape1.dimKeep(pModel->nPoints, 2);
			int nStartLev = pModel->nStartLev;
			Shape1 /= GetPyrScale(nStartLev, pModel->PyrRatio);
			for(int iLev = nStartLev; iLev >= 0; iLev--)   // for each lev in image pyr
			{
				double PyrScale = GetPyrScale(iLev, pModel->PyrRatio);
				SEARCH_IMAGES SearchImgs; // the images used during search
				SearchImgs.Img = image1;  // SearchImgs.Img gets scaled to this pyr lev
				ReduceImage(SearchImgs.Img, PyrScale, pModel->PyrReduceMethod);
				InitGradsIfNeeded(SearchImgs.Grads,         // get SearchImgs.Grads
					pModel->AsmLevs[iLev].ProfSpecs, SearchImgs.Img, Shape1.nrows());
				
				//Shape1.print();
				if(iModel==1 && iLev==0) AsmLevSearch(Shape1, SearchImgs, Models[iModel], iLev, gLandTab, b);
				else AsmLevSearch(Shape1, SearchImgs, Models[iModel], iLev, gLandTab);
				
				if(iLev!=0) // use best shape from this iter as starting point for next
					Shape1 *= pModel->PyrRatio;				
			}
			CombinedShape.assign(Shape1);   // use assign not "=" because size may differ
			CombinedShape = CombinedShape/ImageScale; // descale back to original size
		}
	}
	
	Shape = CombinedShape;
	//Shape.print();

	int npoint = 0;
	if(Shape.nrows())  // successfully located landmarks?
	{
        // convert from stasm coords to OpenCV coords
        Shape.col(VX) += image.width/2;
        Shape.col(VY) = image.height/2-Shape.col(VY);
		
        npoint = Shape.nrows();
        for(int iPoint=0; iPoint<npoint; iPoint++)
		{
            point[2*iPoint]   = x0+iround(Shape(iPoint, VX));
            point[2*iPoint+1] = y0+iround(Shape(iPoint, VY));
		}
    }

#define SHOW_RESULT
#ifdef  SHOW_RESULT 
	//CombinedShape.print();
	if(CombinedShape.nrows())          // successfully located landmarks?
	{
		// Invert image
		RgbImage RgbImg(wf, hf);
		for(int i=0; i<hf; i++)
		{
			for(int j=0; j<wf;  j++)
			{
				int i1 = i*wf+j;
				int i2 = (y0+hf-1-i)*s+3*(x0+j);//3*((h-i-1)*w+j);
				RgbImg.buf[i1].Blue  = data[i2];
				RgbImg.buf[i1].Green = data[i2+1];
				RgbImg.buf[i1].Red   = data[i2+2];
			}
		}
		
		DrawShape(RgbImg, CombinedShape);  // draw landmark shape on image
		CropImageToShape(RgbImg, CombinedShape);
		WriteBmp(RgbImg, "search-results.bmp", VERBOSE);
	}	
#endif

	return npoint;
}


//-------------------------------------------------------------------------------------------------
EXPORT void API FaceAsmRelease(void **p)
{
	*p = 0;
}

