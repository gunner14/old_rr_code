#pragma once


//-------------------------------------------------------------------------------------------------
bool FaceAsmCreate(void **pp, char *pathname);
	
//-------------------------------------------------------------------------------------------------
// (w,s)     - width and width step, e.g. s = (3*w+3)/4*4
// h         - height
// data[s*h] - image pixels data[s*h]
// (x,y)     - face center
// d         - face size d*d
// point     - output landmark points
// b         - eigenvalues for shape
int  FaceAsmLandmarks(void *p, int w, int s, int h, byte *data, int x, int y, int d, int *point, 
					  float *b = 0);
	
//-------------------------------------------------------------------------------------------------
void FaceAsmRelease(void **p);

