#include "jpg.h"
#include <string.h>

#include <ipp.h>
#include "growbuff.h"
#include "membuffin.h"
#include "membuffout.h"
#include "jpegenc.h"
#include "jpegdec.h"
#include "uic_jpeg_dec.h"
#include "uic_jpeg_enc.h"
#include "boost/shared_ptr.hpp"
#include <boost/scoped_array.hpp>
#include "bmploader/bmploader.h"
#include "image.h"

namespace rtimage {
using namespace UIC;

int BYTES_PAD(int iWidth, int iChannels, int iDU) {
  const int ALIGN_VALUE = (sizeof(int) - 1);
  return (((iWidth * iChannels * iDU + ALIGN_VALUE) & (~ALIGN_VALUE))
      - iWidth * iChannels * iDU);
}

class Canvas {
 private:
  boost::scoped_array<unsigned char> canvas_buf_;
  int width_;
  int height_;
 public:
  Canvas(int w, int h)
      : width_(w),
        height_(h),
        canvas_buf_(new unsigned char[w * h]) {

  }
  int GetCanvasWidth() {
    return width_;
  }
  int GetCanvasHeight() {
    return height_;
  }
  unsigned char* GetBuffer() {
    return canvas_buf_.get();
  }

};

class CanvasVector {
 private:
  std::vector<boost::shared_ptr<Canvas> > vec_canvas_;

 public:
  CanvasVector() {
  }
  bool AddCanvas(int width, int height) {
    boost::shared_ptr<Canvas> p(new Canvas(width, height));
    vec_canvas_.push_back(p);
    return true;
  }
  bool RemoveCanvas(int index) {
    //to be implement
    return true;
  }
  int GetCount() {
    return vec_canvas_.size();
  }

  Canvas* GetCanvas(size_t index) {
    size_t size = vec_canvas_.size();

    if (size == 0 || index >= size)
      return NULL;
    return vec_canvas_[index].get();
  }

};

struct JpgUicInfo : public UicInfoBase {
  JpgUicInfo(GrowBufs* growbufs):UicInfoBase(growbufs)  {
    decoder = new UIC::JPEGDecoder();
    ft_manager_.Init("conf/font.conf");
    png_manager_.Load("util/watermark/");
    for (int i = 0; i < 2; i++)
      canvas_vec_.AddCanvas(1024, 1024);

  }  // TODO: test data
  ~JpgUicInfo() {
    delete decoder;
  }

  int mode;  // 显示模式
  FreeTypeManager ft_manager_;
  CanvasVector canvas_vec_;
  PNGManager png_manager_;
};

JpgHandler::JpgHandler(GrowBufs* growbufs) {
  uic_ = new JpgUicInfo(growbufs);
  JpgUicInfo *p = (JpgUicInfo*) uic_;
  p->width = 0;
  p->height = 0;
  p->component_count = 0;
}

JpgHandler::~JpgHandler() {
  delete uic_;
}

IM_ERROR ReadJpg(JpgUicInfo &uic, int width, int height, JDD jd) {
  ImageDataPtr dataptr;
  ImageDataOrder dataorder;
  Image &imagein = uic.image_in;
  ImageSamplingGeometry &geometry = uic.geometry;
  JPEGDecoder &jpgdec = *(JPEGDecoder*) uic.decoder;

  imagein.ColorSpec().ReAlloc(uic.component_count);
  imagein.ColorSpec().SetColorSpecMethod(Enumerated);
  imagein.ColorSpec().SetComponentToColorMap(Direct);

  JPEGDecoderParams jparam;
  jparam.SetDCTScaleFactor(jd);  // 1:1 ,此处需要优化
  geometry.SetEnumSampling(S444);  // 是否可以别的值
  jparam.SetDCTType(0);
  jparam.SetThreadingMode(JT_OLD);
  jpgdec.SetParams(jparam);
  jpgdec.SetNOfThreads(1);

  ImageColorSpec &colorspec = uic.colorspec;

  int du;
  if (colorspec.DataRange()->BitDepth() + 1 <= 8) {
    dataorder.SetDataType(T8u);
    du = sizeof(Ipp8u);
  } else {
    dataorder.SetDataType(T16u);
    du = sizeof(Ipp16u);
  }
  int linestep = width * uic.component_count * du
      + BYTES_PAD(width, uic.component_count, du);

  dataorder.ReAlloc(Interleaved, uic.component_count);
  dataorder.PixelStep()[0] = uic.component_count;
  dataorder.LineStep()[0] = linestep;

  imagein.ColorSpec().ReAlloc(uic.component_count);
  imagein.ColorSpec().SetColorSpecMethod(Enumerated);
  imagein.ColorSpec().SetComponentToColorMap(Direct);
  if (3 == uic.component_count)
    imagein.ColorSpec().SetEnumColorSpace(RGB);
  else
    imagein.ColorSpec().SetEnumColorSpace(Grayscale);

  for (int i = 0; i < uic.component_count; i++) {
    if (colorspec.DataRange()->BitDepth() + 1 <= 8) {
      imagein.ColorSpec().DataRange()[i].SetAsRange8u(255);
    } else {
      imagein.ColorSpec().DataRange()[i].SetAsRange16u(
          1 << colorspec.DataRange()->BitDepth());
    }
  }
  dataptr.p8u = uic.grow_bufs->read_buf.ReAlloc(linestep * height);
  imagein.Buffer().Attach(&dataptr, dataorder, geometry);
  if (ExcStatusOk
      != jpgdec.SetParams(imagein.ColorSpec(),
                          imagein.Buffer().BufferFormat().SamplingGeometry()))
    return IE_PARAMS;

  if (ExcStatusOk != jpgdec.ReadData(imagein.Buffer().DataPtr(), dataorder))
    return IE_RDATA;
  uic.state_ = uic.state_ | STATE_DECODED;
  return IE_OK;
}

bool ImageHandler::Attach(unsigned char *data, int size, int *w, int *h) {
  UicInfoBase& uic = *uic_;
  uic.file_in.Open(data, size);
  //Image &imageCn = uic.image_in;
  ImageColorSpec &colorspec = uic.colorspec;
  ImageSamplingGeometry &geometry = uic.geometry;

  UIC::BaseImageDecoder* decoder = uic.decoder;

  decoder->Close();
  if (ExcStatusOk != decoder->Init())
    return false;
  //return IE_INIT;

  if (ExcStatusOk != decoder->AttachStream(uic.file_in))
    return false;
  //return IE_ASTREAM;

  if (ExcStatusOk != decoder->ReadHeader(colorspec, geometry))
    return false;
  //return IE_RHEADER;

  uic.component_count = geometry.NOfComponents();
  uic.sampling = geometry.EnumSampling();
  *w = uic.width = geometry.RefGridRect().Width();
  *h = uic.height = geometry.RefGridRect().Height();
  uic.state_ = STATE_ATTACHED;
  return true;
}

bool JpgHandler::Attach(unsigned char *data, int size, int *w, int *h) {
  if (!ImageHandler::Attach(data, size, w, h)) {
    return false;
  }
  JpgUicInfo* uic = (JpgUicInfo*) uic_;
  JPEGDecoder* jpgdecoder = (JPEGDecoder*) uic->decoder;
  uic->mode = jpgdecoder->Mode();
  return true;
}

// out_buff的分配由上层完成
IM_ERROR WriteJpg(const JpgUicInfo &uic, int quality, char *comment,
                  unsigned char *buff_ptr, int *buff_size) {
  CMemBuffOutput out;
  out.Open(buff_ptr, *buff_size);

  JPEGEncoder jpegenc = *uic.jpeg_encoder;
  jpegenc.Close();
  jpegenc.SetNOfThreads(1);

  if (ExcStatusOk != jpegenc.Init())
    return IE_INIT;
  if (ExcStatusOk != jpegenc.AttachStream(out))
    return IE_ASTREAM;
  if (ExcStatusOk != jpegenc.SetComment(strlen(comment), comment))
    return IE_PARAMS;

  if (ExcStatusOk != jpegenc.AttachImage(uic.image_out))
    return IE_AIMAGE;

  // 目前仅仅支持 JPEG_PROGRESSIVE:
  JPEGEncoderParamsPRG je_params;
  if (3 == uic.component_count)
    je_params.SetColor((JCOLOR) 10);  //IC_YCBCR  RGB?
  else
    je_params.SetColor((JCOLOR) 1);  //IC_YCBCR  RGB?

  je_params.SetSampling((JSS) uic.sampling);  // 也要用ic的,这些需要改源码
  je_params.SetRestartInterval(0);  // 1 ? 0 什么区别
  je_params.SetHuffmanOption(0);
  je_params.SetQuality(quality);
  if (ExcStatusOk != jpegenc.SetParams(je_params))
    return IE_PARAMS;

  //if(ExcStatusOk != jpegenc.WriteHeader())
  ExcStatus err = jpegenc.WriteHeader();
  if (err != ExcStatusOk)
    return IE_WHEADER;

  if (ExcStatusOk != jpegenc.WriteData())
    return IE_WDATA;

  out.Position(*(UIC::BaseStream::TPosition*) buff_size);

  return IE_OK;
}

struct SharpenFilterKernel {
  const static unsigned kKernelLen = 9;
  SharpenFilterKernel(const Ipp32s* kernel, const IppiSize& size,
                      const IppiPoint& anchor, const int divisor)
      : kKernel(kernel),
        kKernelSize(size),
        kAnchor(anchor),
        kDivisor(divisor) {

  }
  const Ipp32s* kKernel;
  const IppiSize kKernelSize;
  const IppiPoint kAnchor;
  const int kDivisor;
};
const static Ipp32s kKernels[][SharpenFilterKernel::kKernelLen] = { { -1, -1,
    -1, -1, 16, -1, -1, -1, -1 }, { 0, -1, 0, -1, 8, -1, 0, -1, 0 }, };
const static IppiSize kSize3x3 = { 3, 3 };
const static IppiPoint kPoint11 = { 1, 1 };
const static SharpenFilterKernel kSharpenFilterKernels[] = {
    SharpenFilterKernel(kKernels[0], kSize3x3, kPoint11, 8),
    SharpenFilterKernel(kKernels[1], kSize3x3, kPoint11, 4), };

bool Sharpen(JpgUicInfo &uic, int sharpen_type) {
  IppStatus status = ippStsNoErr;
  int i = 0;
  int j = 0;
  ImageBuffer* pimagebuff = NULL;
  if (uic.state_ & STATE_RESIZED) {
    pimagebuff = &(uic.image_out.Buffer());

  } else {
    pimagebuff = &(uic.image_in.Buffer());
  }

  const ImageSamplingGeometry &geom = pimagebuff->BufferFormat()
      .SamplingGeometry();
  const ImageDataOrder &order = pimagebuff->BufferFormat().DataOrder();
  IppiSize srcRoi = { geom.RefGridRect().Width(), geom.RefGridRect().Height() };
  IppiRect dstRect = { 0, 0, srcRoi.width, srcRoi.height };
  unsigned int channels = geom.NOfComponents();
  ImageDataPtr dstptr;
  ImageSamplingGeometry dstgeom;
  dstgeom.CreateCopy(geom);
  ImageDataOrder dstorder;
  dstorder.CreateCopy(order, channels);

  int du;
  if (order.DataType() == T8u) {
    du = sizeof(Ipp8u);
  } else {
    du = sizeof(Ipp16u);
  }
  int srcStep = *order.LineStep();
  int dstStep = srcRoi.width * channels * du
      + BYTES_PAD(srcRoi.width, channels, du);

  /*
   if (uic.state_ & STATE_RESIZED)
   dstptr.p8u = uic.hander_buf.ptr();
   else
   dstptr.p8u = uic.read_buf.ptr();
   */
  dstptr.p8u = pimagebuff->DataPtr()->p8u;

  *dstorder.LineStep() = dstStep;
  dstgeom.SetRefGridRect(
      Rect(Point(0, 0), RectSize(srcRoi.width, srcRoi.height)));
  const unsigned kNumKernels = sizeof(kSharpenFilterKernels)
      / sizeof(kSharpenFilterKernels[0]);
  assert(sharpen_type > 0);
  sharpen_type -= 1;
  sharpen_type %= kNumKernels;
  const SharpenFilterKernel* kernel = &kSharpenFilterKernels[sharpen_type];
  int border_width = kernel->kKernelSize.width - kernel->kAnchor.x - 1;
  int border_height = kernel->kKernelSize.height - kernel->kAnchor.y - 1;
  int dst_width = srcRoi.width - (kernel->kKernelSize.width - 1);
  int dst_height = srcRoi.height - (kernel->kKernelSize.height - 1);
  IppiSize dst_roi = { dst_width, dst_height };
  Ipp8u* ptr_with_offset = dstptr.p8u + border_height * srcStep
      + border_width * channels * du;
  switch (channels) {
    case 1: {
#if 0
      status = ippiFilterSharpen_8u_C1R(dstptr.p8u, srcStep, dstptr.p8u,
          dstStep, srcRoi);
#else
      status = ippiFilter_8u_C1R(ptr_with_offset, srcStep, ptr_with_offset,
                                 srcStep, dst_roi, kernel->kKernel,
                                 kernel->kKernelSize, kernel->kAnchor,
                                 kernel->kDivisor);
#endif
      break;
    }
    case 3: {
      status = ippiFilter_8u_C3R(ptr_with_offset, srcStep, ptr_with_offset,
                                 srcStep, dst_roi, kernel->kKernel,
                                 kernel->kKernelSize, kernel->kAnchor,
                                 kernel->kDivisor);
      break;
    }
    case 4: {
      status = ippiFilter_8u_C4R(ptr_with_offset, srcStep, ptr_with_offset,
                                 srcStep, dst_roi, kernel->kKernel,
                                 kernel->kKernelSize, kernel->kAnchor,
                                 kernel->kDivisor);
      break;
    }
    default: {
      status = ippStsErr;
      break;
    }
  }
  uic.image_out.Buffer().Attach(&dstptr, dstorder, dstgeom);
  if (uic.state_ & STATE_RESIZED)
    uic.image_out.ColorSpec().CreateCopy(uic.image_out.ColorSpec(), channels);
  else
    uic.image_out.ColorSpec().CreateCopy(uic.image_in.ColorSpec(), channels);
  return (ippStsNoErr != status);

  return false;

}

bool Shear(JpgUicInfo &uic, int left, int top, int width, int height, int w,
           int h) {
  int bufsize;
  IppStatus status;

  ImageBuffer &imagebuff = uic.image_in.Buffer();
  const ImageDataPtr *ptr = imagebuff.DataPtr();
  const ImageSamplingGeometry &geom =
      imagebuff.BufferFormat().SamplingGeometry();
  const ImageDataOrder &order = imagebuff.BufferFormat().DataOrder();
  IppiSize srcRoi = { geom.RefGridRect().Width(), geom.RefGridRect().Height() };
  IppiRect srcRect = { left, top, width, height };
  IppiRect dstRect = { 0, 0, w, h };
  double xfactor = 0.2;
  double yfactor = -0.1;
  double xshift = 0.0;
  double yshift = 0.0;
  unsigned int channels = geom.NOfComponents();
  ImageDataPtr dstptr;  // Interleaved 就一个
  ImageSamplingGeometry dstgeom;
  dstgeom.CreateCopy(geom);
  ImageDataOrder dstorder;
  dstorder.CreateCopy(order, channels);
  int du;
  if (order.DataType() == T8u) {
    du = sizeof(Ipp8u);
  } else {
    du = sizeof(Ipp16u);
  }
  int srcStep = *order.LineStep();
  int dstStep = w * channels * du + BYTES_PAD(w, channels, du);
  uic.grow_bufs->hander_buf.ReAlloc(dstStep * h);
  dstptr.p8u = uic.grow_bufs->hander_buf.ptr();

  *dstorder.LineStep() = dstStep;
  dstgeom.SetRefGridRect(Rect(Point(0, 0), RectSize(w, h)));
  switch (channels) {
    case 1: {
      status = ippiShear_8u_C1R((Ipp8u*) ptr->p8u, srcRoi, srcStep, srcRect,
                                dstptr.p8u, dstStep, dstRect, xfactor, yfactor,
                                xshift, yshift, IPPI_INTER_CUBIC);
      break;
    }
    case 3: {
      status = ippiShear_8u_C3R((Ipp8u*) ptr->p8u, srcRoi, srcStep, srcRect,
                                dstptr.p8u, dstStep, dstRect, xfactor, yfactor,
                                xshift, yshift, IPPI_INTER_CUBIC);
      break;
    }
    case 4: {
      status = ippiShear_8u_C4R((Ipp8u*) ptr->p8u, srcRoi, srcStep, srcRect,
                                dstptr.p8u, dstStep, dstRect, xfactor, yfactor,
                                xshift, yshift, IPPI_INTER_CUBIC);
      break;
    }
    default: {
      status = ippStsErr;
      break;
    }
  }
  uic.image_out.Buffer().Attach(&dstptr, dstorder, dstgeom);
  uic.image_out.ColorSpec().CreateCopy(uic.image_in.ColorSpec(), channels);
  return (ippStsNoErr != status);
}
#define kResizeType IPPI_INTER_CUBIC2P_B05C03
bool Resize(UicInfoBase &uic, int left, int top, int width, int height, int w,
            int h) {
  int bufsize;
  Ipp8u* buf;
  IppStatus status;

  ImageBuffer &imagebuff = uic.image_in.Buffer();
  const ImageDataPtr *ptr = imagebuff.DataPtr();
  const ImageSamplingGeometry &geom =
      imagebuff.BufferFormat().SamplingGeometry();
  const ImageDataOrder &order = imagebuff.BufferFormat().DataOrder();

  IppiSize srcRoi = { geom.RefGridRect().Width(), geom.RefGridRect().Height() };
  IppiRect srcRect = { left, top, width, height };
  IppiRect dstRect = { 0, 0, w, h };
  double xfactor = (double) w / (double) width;
  double yfactor = (double) h / (double) height;
  double xshift = 0.0 - xfactor * left;
  double yshift = 0.0 - yfactor * top;
  unsigned int channels = geom.NOfComponents();
  ImageDataPtr dstptr;  // Interleaved 就一个
  ImageSamplingGeometry dstgeom;
  dstgeom.CreateCopy(geom);
  ImageDataOrder dstorder;
  dstorder.CreateCopy(order, channels);

  int du;
  if (order.DataType() == T8u) {
    du = sizeof(Ipp8u);
  } else {
    du = sizeof(Ipp16u);
  }
  int srcStep = *order.LineStep();
  int dstStep = w * channels * du + BYTES_PAD(w, channels, du);

  uic.grow_bufs->hander_buf.ReAlloc(dstStep * h);
  dstptr.p8u = uic.grow_bufs->hander_buf.ptr();

  *dstorder.LineStep() = dstStep;
  dstgeom.SetRefGridRect(Rect(Point(0, 0), RectSize(w, h)));

  status = ippiResizeGetBufSize(srcRect, dstRect, channels, kResizeType,
                                &bufsize);
  if (ippStsNoErr != status)
    return false;
  buf = (Ipp8u*) ippMalloc(bufsize);
  if (0 == buf)
    return false;

  switch (channels) {
    case 1: {
      status = ippiResizeSqrPixel_8u_C1R((Ipp8u*) ptr->p8u, srcRoi, srcStep,
                                         srcRect, dstptr.p8u, dstStep, dstRect,
                                         xfactor, yfactor, xshift, yshift,
                                         kResizeType, buf);
      break;
    }

    case 3: {
      status = ippiResizeSqrPixel_8u_C3R((Ipp8u*) ptr->p8u, srcRoi, srcStep,
                                         srcRect, dstptr.p8u, dstStep, dstRect,
                                         xfactor, yfactor, xshift, yshift,
                                         kResizeType, buf);
      break;
    }

    case 4: {
      status = ippiResizeSqrPixel_8u_C4R((Ipp8u*) ptr->p8u, srcRoi, srcStep,
                                         srcRect, dstptr.p8u, dstStep, dstRect,
                                         xfactor, yfactor, xshift, yshift,
                                         kResizeType, buf);
      break;
    }

    default: {
      status = ippStsErr;
      break;
    }
  }

  if (0 != buf)
    ippFree(buf);

  uic.image_out.Buffer().Attach(&dstptr, dstorder, dstgeom);
  uic.image_out.ColorSpec().CreateCopy(uic.image_in.ColorSpec(), channels);
  bool ret = ippStsNoErr == status;
  if (ret)
    uic.state_ = uic.state_ | STATE_RESIZED;
  return ret;
}

bool ImageHandler::FakeResize() {
  UicInfoBase& uic = *uic_;

  ImageBuffer &imagebuff = uic.image_in.Buffer();
  const ImageDataPtr *ptr = imagebuff.DataPtr();
  const ImageDataOrder &order = imagebuff.BufferFormat().DataOrder();
  const ImageSamplingGeometry &geom =
      imagebuff.BufferFormat().SamplingGeometry();

  uic.image_out.Buffer().Attach(ptr, order, geom);
  uic.image_out.ColorSpec().CreateCopy(uic.image_in.ColorSpec(),
                                       geom.NOfComponents());
  uic.state_ = uic.state_ | STATE_FAKERESIZED;
  return true;
}

bool CalculateOffset(const int pic_width, const int pic_height,
                     const int hor_distance, const int ver_distance,
                     const aligntype align_type, const int line_count,
                     const PNGData* pPNGData, const int distance_p,
                     const std::vector<int>& vec_str_width,
                     const std::vector<int>& vec_str_height,
                     std::vector<int>& vec_offset_x,
                     std::vector<int>& vec_offset_y, int& logo_offset_x,
                     int& logo_offset_y) {
  int i;
  int j;
  if (pic_width <= 0 || pic_height <= 0 || hor_distance >= pic_width
      || ver_distance >= pic_height || align_type <= align_type_start
      || align_type >= align_type_end || line_count < 0) {
    return false;
  }
  bool isLogoVaild = pPNGData && pPNGData->ippdata && pPNGData->height > 0
      && pPNGData->width > 0 && pPNGData->height <= pic_height
      && pPNGData->width <= pic_width;
  if (isLogoVaild && line_count == 0) {
    //只有图标
    if (align_bottom_right == align_type || align_top_right == align_type) {
      logo_offset_x = pic_width - hor_distance - pPNGData->width;
      if (logo_offset_x < 0)
        return false;
      if (align_bottom_right == align_type) {
        logo_offset_y = pic_height - ver_distance - pPNGData->height;
        if (logo_offset_y < 0)
          return false;
      } else {
        if (ver_distance + pPNGData->height > pic_height)
          return false;
        logo_offset_y = ver_distance;
      }

    } else if (align_bottom_center == align_type
        || align_top_center == align_type) {
      logo_offset_x = (pic_width - pPNGData->width) / 2;
      if (logo_offset_x <= 0)
        return false;
      if (align_bottom_center == align_type) {
        logo_offset_y = pic_height - ver_distance - pPNGData->height;
        if (logo_offset_y < 0)
          return false;
      } else {
        if (ver_distance + pPNGData->height > pic_height)
          return false;
        logo_offset_y = ver_distance;
      }
    } else if (align_bottom_left == align_type
        || align_top_left == align_type) {
      logo_offset_x = hor_distance;
      if (hor_distance + pPNGData->width > pic_width)
        return false;
      if (align_bottom_left == align_type) {
        logo_offset_y = pic_height - ver_distance - pPNGData->height;
        if (logo_offset_y < 0)
          return false;
      } else {
        if (ver_distance + pPNGData->height > pic_height)
          return false;
        logo_offset_y = ver_distance;
      }
    }
    return true;
  }

  int sum_y = 0;
  int value;
  //以下计算水印文字的offset_x
  for (i = 0; i < line_count; i++) {
    if (align_bottom_right == align_type || align_top_right == align_type) {
      value = pic_width - hor_distance - vec_str_width[i];
      if (value <= 0)
        return false;
      vec_offset_x.push_back(value);
    } else if (align_bottom_left == align_type
        || align_top_left == align_type) {
      if (i == 0 && isLogoVaild) {
        value = hor_distance + pPNGData->width + distance_p;
        if (value + vec_str_width[i] > pic_width)
          return false;
        vec_offset_x.push_back(value);
      } else {
        if (hor_distance + vec_str_width[i] > pic_width)
          return false;
        vec_offset_x.push_back(hor_distance);
      }
    } else if (align_bottom_center == align_type
        || align_top_center == align_type) {
      if (i == 0 && isLogoVaild) {
        value = (pic_width + pPNGData->width + distance_p - vec_str_width[i]);
        if (value < 0)
          return false;
        vec_offset_x.push_back(value / 2);
      } else {
        value = (pic_width - vec_str_width[i]);
        if (value < 0)
          return false;
        vec_offset_x.push_back(value / 2);
      }
    }

    //以下计算水印文字的offset_y
    if (align_bottom_right == align_type || align_bottom_center == align_type
        || align_bottom_left == align_type) {
      int sum = 0;
      for (j = i; j < line_count; j++) {
        sum += vec_str_height[j];
      }
      value = pic_height - ver_distance - sum;
      if (value < 0)
        return false;
      vec_offset_y.push_back(value);
    } else if (align_top_right == align_type || align_top_center == align_type
        || align_top_left == align_type) {
      //TO DO
      if (i == 0) {
        if (isLogoVaild) {
          if (pPNGData->height > vec_str_height[i]) {
            //图标的高度大于文字
            value = ver_distance + pPNGData->height - vec_str_height[i];
            if (value + vec_str_height[i] > pic_height)
              return false;
            vec_offset_y.push_back(
                ver_distance + pPNGData->height - vec_str_height[i]);
          } else {
            if (ver_distance + vec_str_height[i] > pic_height)
              return false;
            vec_offset_y.push_back(ver_distance);
          }
        } else {
          if (ver_distance + vec_str_height[i] > pic_height)
            return false;
          vec_offset_y.push_back(ver_distance);
        }
      } else {
        sum_y = ver_distance;
        for (int ii = 0; ii < i; ii++) {
          sum_y += vec_str_height[ii] + vec_offset_y[ii];
        }
        if (sum_y + vec_str_height[i] > pic_height)
          return false;
        vec_offset_y.push_back(sum_y);
      }
    }
  }

  //以下计算logo的offset_x和offset_y
  if (isLogoVaild) {
    //以下计算logo的offset_x
    if (align_bottom_right == align_type || align_top_right == align_type) {
      logo_offset_x = pic_width - hor_distance - vec_str_width[0] - distance_p
          - pPNGData->width;
      if (logo_offset_x < 0)
        return false;
    } else if (align_bottom_left == align_type
        || align_top_left == align_type) {
      if (hor_distance + pPNGData->width > pic_width)
        return false;
      logo_offset_x = hor_distance;
    } else if (align_bottom_center == align_type
        || align_top_center == align_type) {
      logo_offset_x = vec_offset_x[0] - distance_p - pPNGData->width;
      if (logo_offset_x < 0)
        return false;
    }
    //计算logo的offset_x结束

    //以下计算logo的offset_y
    if (align_bottom_center == align_type || align_bottom_left == align_type
        || align_bottom_right == align_type) {
      int tmp = pic_height - ver_distance;
      for (i = line_count - 1; i > 0; i--) {
        tmp = tmp - vec_str_height[i];
      }
      logo_offset_y = tmp - pPNGData->height;
      if (logo_offset_y < 0)
        return false;
    } else if (align_top_center == align_type || align_top_left == align_type
        || align_top_right == align_type) {
      if (pPNGData->height > vec_str_height[0]) {
        //图标高度大于字符串高度
        if (ver_distance + pPNGData->height > pic_height)
          return false;
        logo_offset_y = ver_distance;
      } else {
        logo_offset_y = ver_distance + vec_str_height[0] - pPNGData->height;
        if (logo_offset_y + pPNGData->height > pic_height)
          return false;
      }

    }
    //计算logo的offset_y结束
  }

  return true;
}

bool Watermark(JpgUicInfo &uic, int pic_width, int pic_height,
               const std::vector<FreeTypeString>& vec,
               const aligntype align_type, int hor_distance, int ver_distance,
               const std::string& logo_file_name) {
  IppStatus status = ippStsNoErr;
  int i = 0;
  int j = 0;
  ImageBuffer* pimagebuff = NULL;
  if (uic.state_ & STATE_RESIZED) {
    pimagebuff = &(uic.image_out.Buffer());

  } else {
    pimagebuff = &(uic.image_in.Buffer());
  }

  const ImageSamplingGeometry &geom = pimagebuff->BufferFormat()
      .SamplingGeometry();
  const ImageDataOrder &order = pimagebuff->BufferFormat().DataOrder();

  IppiSize srcRoi = { pic_width, pic_height };
  IppiRect dstRect = { 0, 0, pic_width, pic_height };
  unsigned int channels = geom.NOfComponents();
  if (channels == 1)
    return true;
  ImageDataPtr dstptr;
  ImageSamplingGeometry dstgeom;
  dstgeom.CreateCopy(geom);
  ImageDataOrder dstorder;
  dstorder.CreateCopy(order, channels);

  int du;
  if (order.DataType() == T8u) {
    du = sizeof(Ipp8u);
  } else {
    du = sizeof(Ipp16u);
  }
  int srcStep = *order.LineStep();
  int dstStep = pic_width * channels * du + BYTES_PAD(pic_width, channels, du);

  dstptr.p8u = pimagebuff->DataPtr()->p8u;

  *dstorder.LineStep() = dstStep;
  dstgeom.SetRefGridRect(Rect(Point(0, 0), RectSize(pic_width, pic_height)));

  int line_count = vec.size();
  CanvasVector& canvas_vector = uic.canvas_vec_;
  if (canvas_vector.GetCount() < line_count)
    return false;

  std::vector<int> vec_str_width;
  std::vector<int> vec_str_height;
  int string_width = 0;
  int string_height = 0;

  for (i = 0; i < line_count; i++) {
    const FreeTypeString& freetype_str = vec[i];
    FreeTypeManager& ft_manager = uic.ft_manager_;
    FreeType* p = ft_manager.GetFont(freetype_str.font_name_);
    p->SetCharSize(freetype_str.char_width_, freetype_str.char_height_);
    Canvas* canvas = canvas_vector.GetCanvas(i);
    if (!canvas)
      return false;

    bool isOk = p->RenderString(freetype_str.text_, canvas->GetBuffer(),
                                canvas->GetCanvasWidth(),
                                canvas->GetCanvasHeight(), string_width,
                                string_height);
    if (!isOk)
      break;
    vec_str_width.push_back(string_width);
    vec_str_height.push_back(string_height);

  }
  std::vector<int> vec_offset_x;
  std::vector<int> vec_offset_y;
  int logo_offset_x = 0;
  int logo_offset_y = 0;
  PNGManager& png_manager = uic.png_manager_;
  std::string logo_file_full_path;

  logo_file_full_path += "util/watermark/" + logo_file_name + ".png";
  const PNGData* pPNGData = png_manager.GetPNGData(logo_file_full_path);
  if (!CalculateOffset(pic_width, pic_height, hor_distance, ver_distance,
                       align_type, line_count, pPNGData, 0, vec_str_width,
                       vec_str_height, vec_offset_x, vec_offset_y,
                       logo_offset_x, logo_offset_y))
    return false;
  int freetype_mem_size = 0;
  ImageDataPtr pDstFreetype;
  pDstFreetype.p8u = NULL;
  unsigned char* pic_ippdata = NULL;
  int pic_step = dstStep;

//here ok
  if (channels == 3) {

    pic_step = pic_width * 4 * du + BYTES_PAD(pic_width, 4, du);
    if (uic.state_ & STATE_RESIZED) {
      uic.grow_bufs->operation_buf.ReAlloc(pic_step * pic_height);
      pic_ippdata = uic.grow_bufs->operation_buf.ptr();
    } else {
      uic.grow_bufs->hander_buf.ReAlloc(pic_step * pic_height);
      pic_ippdata = uic.grow_bufs->hander_buf.ptr();
    }

  }
  for (i = 0; i < line_count; i++) {
    const FreeTypeString& freetype_str = vec[i];
    Canvas* pCanvas = canvas_vector.GetCanvas(i);

    const int free_type_width = pCanvas->GetCanvasWidth();
    const int free_type_height = pCanvas->GetCanvasHeight();
    int free_type_step = free_type_width;
    IppiSize freetype_ROI = { free_type_width, free_type_height };
    unsigned char* pcanvasbuf = pCanvas->GetBuffer();
    int total_offset = 0;

    if (channels == 3) {
      const int freetype_channels = 4;
      free_type_step = free_type_width * freetype_channels * du
          + BYTES_PAD(free_type_width, freetype_channels, du);
      if (free_type_step * free_type_height > freetype_mem_size) {
        freetype_mem_size = free_type_step * free_type_height;
        pDstFreetype.p8u = (unsigned char*) realloc(pDstFreetype.p8u,
                                                    freetype_mem_size);
      }

      total_offset = vec_offset_y[i] * pic_step + vec_offset_x[i] * 4;

    } else {
      total_offset = vec_offset_y[i] * pic_step + vec_offset_x[i];
    }

    switch (channels) {
      case 1:
        ippiAlphaComp_8u_AC1R((Ipp8u *) pcanvasbuf, free_type_step,
                              (Ipp8u *) dstptr.p8u + total_offset, pic_step,
                              (Ipp8u *) dstptr.p8u + total_offset, pic_step,
                              srcRoi, ippAlphaOver);

        break;
      case 3: {
        status = ippiCopy_8u_C1C4R((Ipp8u*) pcanvasbuf, free_type_width,
                                   (Ipp8u*) pDstFreetype.p8u + 3,
                                   free_type_step, freetype_ROI);
        Ipp8u value[3] = { (freetype_str.color_ >> 16) & 255, (freetype_str
            .color_ >> 8) & 255, (freetype_str.color_) & 255 };
        status = ippiSet_8u_AC4R((Ipp8u*) value, (Ipp8u*) pDstFreetype.p8u,
                                 free_type_step, freetype_ROI);
        if (i == 0) {
          //when multi-line, only copy once
          ippiCopy_8u_C3AC4R((Ipp8u*) dstptr.p8u, dstStep, (Ipp8u*) pic_ippdata,
                             pic_step, srcRoi);
          ippiSet_8u_C4CR(255, (Ipp8u*) pic_ippdata + 3, pic_step, srcRoi);
        }
        IppiSize strRoi = { vec_str_width[i], vec_str_height[i] };
        ippiAlphaComp_8u_AC4R((Ipp8u *) pDstFreetype.p8u, free_type_step,
                              (Ipp8u *) pic_ippdata + total_offset, pic_step,
                              (Ipp8u *) pic_ippdata + total_offset, pic_step,
                              strRoi, ippAlphaOver);
        break;
      }
      default:
        break;
    }

  }
  if (pDstFreetype.p8u != NULL)
    free(pDstFreetype.p8u);

  bool is_logo_valid = pPNGData && pPNGData->ippdata && pPNGData->height > 0
      && pPNGData->width > 0;
  if (is_logo_valid) {
    IppiSize logo_ROI = { pPNGData->width, pPNGData->height };
    const int total_logo_offset = logo_offset_y * pic_step + logo_offset_x * 4;
    if (i == 0) {
      ippiCopy_8u_C3AC4R((Ipp8u*) dstptr.p8u, dstStep, (Ipp8u*) pic_ippdata,
                         pic_step, srcRoi);
      ippiSet_8u_C4CR(255, (Ipp8u*) pic_ippdata + 3, pic_step, srcRoi);
    }

    ippiAlphaComp_8u_AC4R((Ipp8u *) pPNGData->ippdata, pPNGData->step,
                          (Ipp8u *) pic_ippdata + total_logo_offset, pic_step,
                          (Ipp8u *) pic_ippdata + total_logo_offset, pic_step,
                          logo_ROI, ippAlphaOver);
  }

  if (line_count > 0 || is_logo_valid)
    ippiCopy_8u_AC4C3R((Ipp8u*) pic_ippdata, pic_step, (Ipp8u*) dstptr.p8u,
                       dstStep, srcRoi);
  else
    return true;
  uic.image_out.Buffer().Attach(&dstptr, dstorder, dstgeom);
  if (uic.state_ & STATE_RESIZED)
    uic.image_out.ColorSpec().CreateCopy(uic.image_out.ColorSpec(), channels);
  else
    uic.image_out.ColorSpec().CreateCopy(uic.image_in.ColorSpec(), channels);

  return (ippStsNoErr == status);
}

bool JpgHandler::Decode(int w, int h) {
  JpgUicInfo &uic = *(JpgUicInfo*) uic_;
  double lv = (double) uic.width / w;
  JDD jd = JD_1_1;
  if (lv < 3 && lv > 1)
    jd = JD_1_2;
  else if (lv < 5 && lv > 3)
    jd = JD_1_4;
  else if (lv < 9 && lv > 7)
    jd = JD_1_8;

  return IE_OK == ReadJpg(uic, w, h, jd);
}

bool ImageHandler::EncodeJpg(int quality, char* comment,
                             unsigned char **fileptr, int *sz) {
  JpgUicInfo &uic = *(JpgUicInfo*) uic_;
  IM_ERROR p;
  int i = 0;
  if (uic.image_out.Buffer().DataPtr() == 0)
    return false;
  bool ret = true;
  for (; i < 1; i++) {
    int size = uic.grow_bufs->file_out_buf.size();
    p = WriteJpg(uic, quality, comment, uic.grow_bufs->file_out_buf.ptr(), &size);
    if (p == IE_WHEADER || p == IE_WDATA) {
      //uic.file_out_buf.ReAlloc(uic.file_out_buf.size() * 2);
      ret = false;
      break;
    } else {
      *fileptr = uic.grow_bufs->file_out_buf.ptr();
      *sz = size;
      ret = true;
      break;
    }
  }
//  if (i == 2)
//    return false;
  if (uic_->state_ & STATE_RESIZED) {
    uic.image_out.Buffer().FreeOrDetach();
  }
  return ret;
}

bool JpgHandler::Watermark(int pic_width, int pic_height,
                           const std::vector<FreeTypeString>& vec,
                           const aligntype align_type, int hor_distance,
                           int ver_distance,
                           const std::string& logo_file_name) {
  return rtimage::Watermark(*(JpgUicInfo*) uic_, pic_width, pic_height, vec,
                            align_type, hor_distance, ver_distance,
                            logo_file_name);
}

bool ImageHandler::Resize(int left, int top, int width, int height, int w,
                          int h) {
  return rtimage::Resize(*uic_, left, top, width, height, w, h);
}

bool JpgHandler::Shear(int left, int top, int width, int height, int w, int h) {
  return rtimage::Shear(*(JpgUicInfo*) uic_, left, top, width, height, w, h);
}

bool JpgHandler::Sharpen(int sharpen_type) {
  return rtimage::Sharpen(*(JpgUicInfo*) uic_, sharpen_type);
}

}  // namespace
