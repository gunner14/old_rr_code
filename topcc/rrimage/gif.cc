#include "gif.h"
#include "growbuff.h"
#include<Magick++.h>

namespace rtimage {

using namespace Magick;

struct GifInfo : public UicInfoBase {
  GifInfo() : UicInfoBase(NULL) {

  }
  Magick::Image img;
  unsigned char* data;
  int size;
  GrowBuffer read_buf;
};

GifHandler::GifHandler() {
  uic_ = new GifInfo();
}

GifHandler::~GifHandler() {
  delete uic_;
}

bool GifHandler::Attach(unsigned char *data, int size, int *w, int *h) {
  GifInfo &gif = *(GifInfo*) uic_;
  try {
    Blob blob(data, size);
    Magick::Image &image = gif.img;
    image.ping(blob);
    *w = gif.width = image.columns();
    *h = gif.height = image.rows();
    std::string magick = image.magick();
    if (magick != "GIF")
      return false;
    if ((gif.width == 0) || (gif.height == 0)) {  //通过ping没有读出来，尝试全部读取
      //Image readImage;
      image.read(blob);
      gif.width = image.columns();
      gif.height = image.columns();
      magick = image.magick();
      if (magick != "GIF")
        return false;
    }
  } catch (...) {
    return false;
  }
  gif.data = data;
  gif.size = size;
  return true;
}

bool GifHandler::Decode(int w, int h) {
  GifInfo &gif = *(GifInfo*) uic_;
  Magick::Image &image = gif.img;
  try {
    Blob blob(gif.data, gif.size);
    if (gif.width == 0 || gif.height == 0) {
      image.read(blob);
      return true;
    }
    //char size[20];
    //snprintf(size, sizeof(size), "%dx%d!", width, height);
    //image.size(size);
    image.read(blob);
  } catch (...) {
    return false;
  }
  return true;
}

// no need delete fileptr
bool GifHandler::EncodeJpg(int quality, char* comment, unsigned char **fileptr,
                           int *sz) {
  GifInfo &gif = *(GifInfo*) uic_;
  Magick::Image &image = gif.img;
  if (quality != 0)
    image.quality(quality);
  image.interlaceType(Magick::PlaneInterlace);
  //image.magick("JPEG");
  /*为什么生成的图的文件名里面会有-0?*/
  //image.write(path);
  Blob blob;
  image.write(&blob, "JPEG");
  if (fileptr) {
    gif.read_buf.ReAlloc(blob.length());
    *fileptr = gif.read_buf.ptr();
    *sz = blob.length();
    memcpy(*fileptr, blob.data(), blob.length());
    //buf->assign((char*)fileptr, blob.length());
  }
  return true;
}

bool GifHandler::Resize(int left, int top, int width, int height, int w,
                        int h) {
  GifInfo &gif = *(GifInfo*) uic_;
  try {
    char size[20];
    gif.img.crop(Geometry(width, height, left, top));
    snprintf(size, sizeof(size), "%dx%d!", w, h);
    gif.img.zoom(size);
  } catch (...) {
    return false;
  }
  return true;
}
bool GifHandler::Watermark(int pic_width, int pic_height,
                           const std::vector<FreeTypeString>& vec,
                           const aligntype align_type, int hor_distance,
                           int ver_distance,
                           const std::string& logo_file_name) {
  return false;
}

bool GifHandler::Sharpen(int sharpen_type) {
  return false;
}
}
;
// namespace
