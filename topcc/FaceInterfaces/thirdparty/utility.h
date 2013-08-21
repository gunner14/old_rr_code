#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <string>
//#include <sys/time.h>
using namespace std;

#include <opencv2/highgui/highgui.hpp>



inline void show_img(cv::Mat&im, cv::Rect* rect1, const char* title="im" ){

    cv::Mat im1 = im.clone();
    IplImage img = im1;

    //cvSetImageROI(&img, cvRect(rect1->x, rect1->y, rect1->width, rect1->height));


    cv::Rect face = *rect1;
    cvRectangle(&img, cvPoint(face.x, face.y), cvPoint(face.x+face.width, face.y+face.height),CV_RGB(255,0,0));//,3.0);


    cvShowImage(title, &img);
    cvWaitKey(0);


}
inline void show_img(cv::Mat& im, vector<cv::Rect>* faces_det=NULL, const char* title="im")
{


    cv::Mat im1 = im.clone();
    CvMat img = im1;

    if(faces_det!=NULL)
    {
        for(unsigned int i=0; i< faces_det->size(); i++)
        {
            cv::Rect face = (*faces_det)[i];
            cvRectangle(&img, cvPoint(face.x, face.y), cvPoint(face.x+face.width, face.y+face.height),CV_RGB(255,0,0));//,3.0);

        }


    }    //imshow("im", &im);
    cvShowImage(title, &img);
    cvWaitKey(0);

}


inline void show_img(string& im_path, cv::Rect* rect, const char* title="im"){

    cv::Mat im = cv::imread(im_path.c_str());
    show_img(im, rect,title);

}


inline void vSplitString( string strSrc ,vector<string>& vecDest ,char cSeparator )
{
    if( strSrc.empty() )
        return ;

      string::size_type size_pos = 0;
      string::size_type size_prev_pos = 0;

      while( (size_pos = strSrc.find_first_of( cSeparator ,size_pos)) != string::npos)
      {
           string strTemp=  strSrc.substr(size_prev_pos , size_pos-size_prev_pos );

           vecDest.push_back(strTemp);
           //cout << "string = "<< strTemp << endl;
           size_prev_pos =++ size_pos;
      }

      vecDest.push_back(&strSrc[size_prev_pos]);
      //cout << "end string = "<< &strSrc[size_prev_pos] << endl;
}
/*

#define CALC_TIME(x) struct timeval start, end;\
                   long mtime, seconds, useconds;\
                   gettimeofday(&start, NULL);\
                   x;\
                   gettimeofday(&end, NULL);\
                   seconds  = end.tv_sec  - start.tv_sec;\
                   useconds = end.tv_usec - start.tv_usec;\
                   mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;\
                   printf("Elapsed time: %ld milliseconds\n", mtime);

*/
#endif
