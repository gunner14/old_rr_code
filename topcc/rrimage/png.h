/*
 * png.h
 *
 *  Created on: 2012-4-12
 *      Author: Huabin Zhengu
 */

#ifndef PNG_H_
#define PNG_H_
#include <uic_image.h>
#include "image.h"

namespace rtimage {

typedef struct _PARAMS_PNG
{
  bool filters[5];
} PARAMS_PNG;

struct GrowBufs;

class PngHandler: public ImageHandler {
public:
  PngHandler(GrowBufs* growbufs);
  virtual ~PngHandler();
protected:
  /*virtual*/ bool Decode(int w, int h);
  /*virtual*/ bool Watermark(int pic_width, int pic_height,
                   const std::vector<FreeTypeString>& vec,
                   const aligntype align_type, int hor_distance, int ver_distance,
                   const std::string& logo_file_name);

private:
  PngHandler(const PngHandler &rhs);
  PngHandler & operator =(const PngHandler &rhs);
};

} /* namespace rtimage */
#endif /* PNG_H_ */
