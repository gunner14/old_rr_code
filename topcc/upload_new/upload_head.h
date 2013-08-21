#ifndef _PHOTO_HEAD_H_
#define _PHOTO_HEAD_H_
#include <vector>
#include <string>
#include <map>

namespace upload {

struct OutImageUrl {
  std::string url_;
  std::string data_;
  int width_;
  int height_;
  std::string nameprefix_;
};

struct OutFile {
  int code_;
  //int index;
  std::string msg_;
  std::string filename_;
  int filesize_;
  int height_;
  int width_;
  std::vector<OutImageUrl> images_;
  std::map<std::string, std::string> exifs_;
  std::vector<int> lostblocks_;   // [0,1,4,9],                 #当使用分块上传时,标示没有传输的块列表.  
};

struct OutResponse {
  int code_;
  std::string msg_;
  std::string uploadid_;
  std::vector<OutFile> files_;
};

}
#endif
