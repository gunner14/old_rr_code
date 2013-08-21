#ifndef _RTIMAGE_IMAGE_H_
#define _RTIMAGE_IMAGE_H_
#include <vector>
#include <ippi.h>
#include <uic_image.h>
#include <membuffin.h>
#include <uic_jpeg_dec.h>
#include <uic_jpeg_enc.h>
#include <uic_png_dec.h>
#include <uic_png_enc.h>
#include "growbuff.h"
#include "ftstring.h"

// forward declarations
namespace UIC {
class BaseImageDecoder;
}

namespace rtimage {

enum rt_state {
  STATE_NONE = 0,
  STATE_ATTACHED = 1,
  STATE_DECODED = 2,
  STATE_FAKERESIZED = 4,
  STATE_RESIZED = 8,
  STATE_WATERMARKED = 16
};

typedef enum _IM_ERROR {
  IE_OK = 0,
  IE_RHEADER = -1,  // can not read header
  IE_RDATA = -2,  // can not read data
  IE_WHEADER = -3,  // can not write header
  IE_WDATA = -4,  // can not write data
  IE_INIT = -5,  // can not initialize codec
  IE_ASTREAM = -6,  // can not attach stream
  IE_AIMAGE = -7,  // can not attach stream
  IE_RESET = -8,  // can not reset decoder
  IE_ALLOC = -9,  // can not allocate memory
  IE_PARAMS = -10,  // can not set params
  IE_UNSUP = -11,  // unsupported image format
  IE_FOPEN = -12,  // can not open file
  IE_FCLOSE = -13,  // can not close file
  IE_FSEEK = -14,  // can not seek in file
  IE_FREAD = -15,  // can not read file
  IE_FWRITE = -16,  // can not write file
  IE_PROCESS = -17,  // image process error
  IE_UNDEF = -255  // unknown error
} IM_ERROR;

struct GrowBufs {
  GrowBuffer read_buf;
  GrowBuffer hander_buf;
  GrowBuffer operation_buf;
  GrowBuffer file_out_buf;
};
struct UicInfoBase {
  int width;
  int height;
  UIC::BaseImageDecoder* decoder;
  UIC::JPEGEncoder* jpeg_encoder;
  UIC::PNGEncoder* png_encoder;
  CMemBuffInput file_in;
  UIC::Image image_in;
  UIC::Image image_out;
  UIC::ImageColorSpec colorspec;
  UIC::ImageSamplingGeometry geometry;
  // 因为我们的程序处理流程是单线程，所以可以让jpghandler, pnghandler,
  // gifhandler共享growbufs。 这样可以显著节省内存
  GrowBufs* grow_bufs;
  int component_count;  // number of channels
  ImageEnumSampling sampling;
  //rt_state
  unsigned int state_;
 public:
  UicInfoBase(GrowBufs* growbufs)
      : width(0),
        height(0),
        decoder(NULL),
        grow_bufs(growbufs),
        state_(STATE_NONE) {
    jpeg_encoder = new UIC::JPEGEncoder();
    png_encoder = new UIC::PNGEncoder();
  }
  virtual ~UicInfoBase() {
    decoder = NULL;
    grow_bufs = NULL;
    delete jpeg_encoder;
    delete png_encoder;
  }
};

class ImageHandler {
 public:
  ImageHandler() {
    uic_ = NULL;
  }
  virtual ~ImageHandler() {
    uic_ = NULL;
  }
  virtual bool Attach(unsigned char *data, int size, int *w, int *h);
  virtual bool Decode(int w, int h) = 0;
  // no need delete fileptr
  virtual bool EncodeJpg(int quality, char* comment, unsigned char **fileptr,
                         int *sz);
  virtual bool EncodePng(unsigned char **fileptr, int *sz);
  virtual bool Resize(int left, int top, int width, int height, int w, int h);
  bool FakeResize();
  virtual bool Watermark(int pic_width, int pic_height,
                         const std::vector<FreeTypeString>& vec,
                         const aligntype align_type, int hor_distance,
                         int ver_distance, const std::string& logo_file_name)=0;
  virtual bool Sharpen(int sharpen_type) {
    return false;
  }
  unsigned char* GetDecodedPicData() {
    if (!uic_ || uic_->image_in.Buffer().DataPtr() == 0)
      return NULL;
    return uic_->image_in.Buffer().DataPtr()->p8u;
  }
 protected:
  UicInfoBase* uic_;
 private:
  ImageHandler(const ImageHandler &gif);
  ImageHandler & operator =(const ImageHandler &gif);
};

int BYTES_PAD(int iWidth, int iChannels, int iDU);

}  //namespace
#endif
