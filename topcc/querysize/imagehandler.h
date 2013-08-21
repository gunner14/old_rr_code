#ifndef _IMAGE_PROCESSOR_H_
#define _IMAGE_PROCESSOR_H_
#include <string>
#include "photo_head.h"
#include "cwf/stream.h"
/*
namespace cwf {
 struct Request;
 struct Request::FormDisposition;
};*/
namespace Magick {
  class Image;
};
namespace photo {

class ImageHandler {
 public:
  static bool ProcessImage(const std::string &url, OutResponse *resp);
  static bool VerityImage(const std::string &data, OutResponse *resp);
 private:

};


}
#endif
