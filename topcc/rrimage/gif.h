#ifndef _RTIMAGE_GIF_H_
#define _RTIMAGE_GIF_H_
#include "image.h"

namespace rtimage {

class GifHandler : public ImageHandler {
 public:
  GifHandler();
  ~GifHandler();
  bool Attach(unsigned char *data, int size, int *w, int *h);
  bool Decode(int w, int h);
  bool EncodeJpg(int quality, char* comment, unsigned char **fileptr, int *sz);
  bool Resize(int left, int top, int width, int height, int w, int h);
  bool Watermark(int pic_width, int pic_height,
                 const std::vector<FreeTypeString>& vec,
                 const aligntype align_type, int hor_distance, int ver_distance,
                 const std::string& logo_file_name);

  bool Sharpen(int sharpen_type);
 private:
  GifHandler(const GifHandler &gif);
  GifHandler & operator =(const GifHandler &gif);
};

}  //namespace

#endif
