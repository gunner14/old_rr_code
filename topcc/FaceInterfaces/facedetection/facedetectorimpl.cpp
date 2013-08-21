#include "facedetectorimpl.h"
//#include <utility.h>
#include <string>
#include <vector>
#include <iostream>
#include "cv.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

string g_sPathName; //此处的g_sPathName来自何处？在哪里赋值？什么用途？

using namespace std;

#define MODEL_PATH "data/front_15_cascade.xml"

static void detect_img(cv::Mat& im, vector<FRECT>& faces);
static cv::CascadeClassifier* g_detector;
//static bool show = false;

IFaceDetector* IFaceDetector::createInstance(){

	IFaceDetector* det = new FaceDetector();
	string modelpath = g_sPathName;
	//modelpath.append(g_sPathName);
	//modelpath.append("\\front_15_cascade.xml");
	//det->init(modelpath);
	det->init(MODEL_PATH);
	return det;

}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
FaceDetector::~FaceDetector()
{
    delete g_detector;
    g_detector = 0;
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
FaceDetector::FaceDetector()
{

}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
void FaceDetector::init(string modelpath)
{
    if(g_detector) {delete g_detector; g_detector=0;}
    g_detector = new cv::CascadeClassifier(modelpath);
}


//static detect(cv::Mat& )

/** @brief (one liner)
  *
  * (documentation goes here)
  */
void detect_img(cv::Mat& im, vector<FRECT>& faces)
{

	if(0 == g_detector) return;

    vector<cv::Rect> rects;
    float scalor = 1.1f;
    int neig = 5;

    float rescalor = 1.0f;
    cv::Mat imp = im;

///resize image to size less than 800 piexls for larger image
//*
    if(max(im.rows, im.cols) > 800)
    {
        rescalor = 800.0f/max(im.rows, im.cols);
        cv::resize(im, imp, cv::Size(0,0), rescalor, rescalor, cv::INTER_LINEAR);
    }
//*/
    g_detector->detectMultiScale(imp, rects,  scalor, neig);

    for(unsigned i=0; i< rects.size(); i++)
    {
        FRECT f;
        f.x = int(rects[i].x / rescalor);
        f.y = int(rects[i].y / rescalor);
        f.w = int(rects[i].width / rescalor);
        f.h = int(rects[i].height / rescalor);
        faces.push_back(f);
    }

    //if(show)
    //    show_img(im, &rects);
#if 0

	for(unsigned i=0; i<faces.size();i++){

		cout<<faces[i].x<<" "<<faces[i].y<<" "<<faces[i].w<<" "<<faces[i].h<<endl;

	}
#endif
}



/** @brief (one liner)
  *
  * (documentation goes here)
  */
void FaceDetector::detect(string fpath, vector<FRECT>& faces)
{

    cv::Mat im = cv::imread(fpath);
    detect_img(im,faces);

}


bool FaceDetector::detectFace(unsigned char* pImageData, int imgWidth, int imgHeight, vector<FRECT>& faces){

	cv::Mat im(imgHeight, imgWidth, CV_8UC1, pImageData);
	detect_img(im, faces);

	if (0 == faces.size()){
		return false;
	}else{
		return true;
	}
}

//bool FaceDetector::detectFace(byte* pImageData, int imgWidth, int imgHeight, vector<FRECT>& faces){
//
//	cv::Mat im(imgHeight, imgWidth, CV_8UC1, pImageData);
//	detect_img(im, faces);
//	return true;
//}

void FaceDetector::release(){

	delete this;

}