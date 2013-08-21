#pragma once
#pragma comment(lib, "landmarks.lib")


//-------------------------------------------------------------------------------------------------
#ifndef EXPORT
#define EXPORT __declspec(dllexport)
#endif

//-------------------------------------------------------------------------------------------------
#ifndef IMPORT
#define IMPORT __declspec(dllimport)
#endif

//-------------------------------------------------------------------------------------------------
#ifndef API
#define API __stdcall
#endif

//-------------------------------------------------------------------------------------------------
#ifndef byte
typedef unsigned char byte;
#endif

//-------------------------------------------------------------------------------------------------
IMPORT bool API FaceAsmCreate(void **pp);

//-------------------------------------------------------------------------------------------------
// (w,s)     - width and width step, e.g. s = (3*w+3)/4*4
// h         - height
// data[s*h] - image pixels data[s*h]
// (x,y)     - face center
// d         - face size d*d
// landmarks - output landmark points
// b         - eigenvalues for shape
IMPORT int  API FaceAsmLandmarks(void *p, int w, int s, int h, byte *data, int x, int y, int d, 
								 int *point, float *b = 0);

//-------------------------------------------------------------------------------------------------
IMPORT void API FaceAsmRelease(void **pp);
