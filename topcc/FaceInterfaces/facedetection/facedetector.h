#pragma once


#include <vector>
using namespace std;


typedef struct st_rect
{
	int x;
	int y;
	int w;
	int h;
} FRECT;


class IFaceDetector
{

public:
	virtual void init(string modelpath)=0;
	virtual void detect(string fpath, vector<FRECT>& faces)=0;
	virtual bool detectFace(unsigned char* pImageData, int imgWidth, int imgHeight, vector<FRECT>& faces)=0;
	virtual void release()=0;

public:
	static IFaceDetector* createInstance();
};


