#ifndef _IMAGE_HELPER_H_
#define _IMAGE_HELPER_H_

#include<Magick++.h>
using namespace Magick;

#include<string>
#include<vector>
#include<list>
#include<map>
#include<utility>
using std::string;
using std::vector;
using std::list;
using std::pair;
using std::map;

//#include "datatype.h"

class CImageHelper
{
public:
  ///获取图片大小和图片类型,用ping实现不用read
  static bool GetSize(const string& data, int& nWidth, int& nHeight, string& magick);
  
  ///获取exif信息
  static bool GetExif(Image& image, map<string, string>& mapExif);
  
  ///size(nWidth,nHeight)后,把data读成image 注:image的长宽不一定是nWidth和nHeight,但会最接近它们
  static bool ReadImage(const string& data, Image& image,int nWidth=0,int nHeight=0);
  
  ///缩图
  //static bool ResizeImage(Image& image, const vector<TSaveImageConf>& vecSaveImageConf, vector<Image>& vecImg);

  ///锐化
 // static bool SharpenImage(const vector<TSaveImageConf> &vecSaveImageConf, vector<Image> &vecImg, int org_width);

  static bool ImageData(const Image &source, int quality, std::string *buf, const std::string &filetype);

  ///保存gif大图
  //static bool SaveGIFImage(const string& data, const TSaveImageConf& saveImageConf, const string& imgPath, bool bResize);

  ///保存图片,image对象是什么类型的就保存成什么类型的
  //static bool SaveImage(vector<Image>& vecImg,const vector<string>& vecImgPath, const vector<TSaveImageConf>& vecSaveImageConf);

  ///保存JPEG图片，无论Image对象的类型，统一保存成jpeg格式的图片
  //static bool SaveJPEGImage(vector<Image>& vecImg,const vector<string>& vecImgPath, const vector<TSaveImageConf>& vecSaveImageConf, std::vector<std::string> *post_buf);
  static bool RotateImage(Image& image, double deg, int *width, int *height);
  static bool CropImage(Image& image, int x, int y, int width, int height);
  static bool CropImage(const Image &source, int x, int y, int width, int height, Image &image);
  //cmyk->rgb
  static bool TransferCMYKImage(Image& image);
  
  static bool ResizeImage(Image &source, int width, int height);
  static bool ResizeImage(const Image &source, int width, int height, Image &image);
  
  static bool ResizeGifData(const std::string &data, int width, int height, std::string *outdata);
private:
  static bool GetFirstImage(const string& data, int width, int height,Image &image);


  static bool SampleImage(Image &source, int width, int height);

  static bool IsJPEG(const string &data);

  static bool IsGIF(const string &data);

  static bool IsBMP(const string &data);

  static bool IsPNG(const string &data);

  static bool IsSupportImage(const string &data);
};
#endif
