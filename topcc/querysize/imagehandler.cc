#include "imagehandler.h"
#include "imagehelper.h"
#include "util.h"
#include "cwf/stream.h"
#include "base3/logging.h"

namespace photo {

bool ImageHandler::VerityImage(const std::string &data, OutResponse *resp) {

  int width,height;
  std::string magick;
  Image image;

  CImageHelper::GetSize(data , width, height, magick);
  if ((width == 0 || height == 0 ) && magick.empty()) {
    resp->code_ = 534;
    resp->msg_ = "图片处理错误,确认是否是图片文件";
    return false;
  }
  resp->width_ = width;
  resp->height_ = height;

  //bool resize = CalcResize(maxwidth, maxheight, width, height, &width0, &height0);

  /*if (!CImageHelper::ReadImage(data, image, 0, 0)) {
    resp->code_ = 534;
    resp->msg_ = "图片处理错误,确认正确图片格式";
    return false;
  }*/
  return true;
}

bool ImageHandler::ProcessImage(const std::string &url, OutResponse *resp) {

  //if (url.find(
  std::string path = "/bigpool/" + url;
  std::string data = LoadFile(path);

  VerityImage(data, resp);
  return true;
}

}
