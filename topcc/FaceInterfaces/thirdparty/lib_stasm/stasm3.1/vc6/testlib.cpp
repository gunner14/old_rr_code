#include "image.h"
#pragma comment(lib, "image.lib")
#include "facelandmark.h"
#define   _CRTDBG_MAP_ALLOC   
#include <stdlib.h>
#include <crtdbg.h> 
#include <stdio.h>

//-------------------------------------------------------------------------------------------------
void main()
{
	void *p = 0;
	if(FaceAsmCreate(&p))
	{
		Startup();
		
		CImage image;
		//image.ReadImage("test1.jpg", true);
		//image.ReadImage("test2.jpg", true);
		//image.ReadImage("test3.jpg", true);
		//image.ReadImage("test4.jpg", true);
		//image.ReadImage("test5.jpg", true);
		//image.ReadImage("test9.jpg", true);
		image.ReadImage("test10.jpg", true);
		//image.ReadImage("0012.bmp", true);
		int w = image.GetWidth(), h = image.GetHeight();
		byte *im = image.GetData();
		
		int n, point[256];
		float b[256];
		//int x = 203, y = 139, d = 164; // test1.jpg
		//int x = 354, y = 292, d = 148; // test2.jpg
		//int x = 103, y = 110, d = 148; // test3.jpg
		//int x = 107, y = 101, d = 135; // test4.jpg
		//int x = 139, y = 128, d = 111; // test5.jpg
		//int x = 198, y = 144, d = 192; // test6.jpg
		//int x = 175, y = 138, d = 178; // test9.jpg
		int x = 203, y = 228, d = 180; // test10.jpg
		//int x = 336, y = 271, d = 160; // test12.bmp
		
		DWORD t0 = GetTickCount();
		n = FaceAsmLandmarks(p, w, 3*w, h, im, x, y, d, point, b);
		DWORD t1 = GetTickCount();
		
		printf("FaceAsmLandmarks(): time = %d ms\n", t1-t0);
		
		FaceAsmRelease(&p);

		Shutdown();
	}
//	else printf("ERROR: FaceAsmInit()\n");

	_CrtDumpMemoryLeaks();
}