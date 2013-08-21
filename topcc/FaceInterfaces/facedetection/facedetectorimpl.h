#pragma once

#include "facedetector.h"

class FaceDetector:public IFaceDetector
{
public:
    FaceDetector();
    ~FaceDetector();

private:
//       CascadeClassifier* m_detector;

public:
    void init(string modelpath);
    void detect(string fpath, vector<FRECT>& faces);
	bool detectFace(unsigned char* pImageData, int imgWidth, int imgHeight, vector<FRECT>& faces);
	void release();

};
