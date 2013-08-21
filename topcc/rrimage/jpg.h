#ifndef _RTIMAGE_JPG_H_
#define _RTIMAGE_JPG_H_
#include "image.h"
#include <vector>
#include "ftstring.h"
namespace rtimage {

struct GrowBufs;

class JpgHandler : public ImageHandler {
 public:
  JpgHandler(GrowBufs* growbufs);
  ~JpgHandler();
  bool Attach(unsigned char *data, int size, int *w, int *h);
  bool Decode(int w, int h);
  bool Shear(int left, int top, int width, int height, int w, int h);  // TODO: test
  bool Watermark(int pic_width, int pic_height,
                 const std::vector<FreeTypeString>& vec,
                 const aligntype align_type, int hor_distance, int ver_distance,
                 const std::string& logo_fie_name);

  bool Sharpen(int sharpen_type);
 private:
  JpgHandler(const JpgHandler &jpg);
  JpgHandler & operator =(const JpgHandler &jpg);
};

}  //namespace
#endif
