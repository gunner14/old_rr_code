#ifndef _PHOTO_HEAD_H_
#define _PHOTO_HEAD_H_
#include <vector>
#include <string>
#include <map>

namespace photo {

struct OutResponse {
  int code_;
  std::string msg_;
  std::string url_;
  int width_;
  int height_;
  std::string ToJson();
};

}
#endif
