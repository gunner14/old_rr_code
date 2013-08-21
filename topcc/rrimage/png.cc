/*
 * png.cpp
 *
 *  Created on: 2012-4-12
 *      Author: Huabin Zheng
 */

#include "png.h"

#include <ipp.h>
#include "growbuff.h"
#include "membuffin.h"
#include "membuffout.h"
#include "uic_png_dec.h"
#include "uic_png_enc.h"
#include "bmploader/bmploader.h"
#include "image.h"

namespace rtimage {

struct PngInfo: public UicInfoBase {
  PngInfo(GrowBufs* growbufs):UicInfoBase(growbufs) {
    decoder = new UIC::PNGDecoder();
  }
  ~PngInfo() {
    delete decoder;
  }
};
PngHandler::PngHandler(GrowBufs* growbufs) {
    uic_ = new PngInfo(growbufs);
}

PngHandler::~PngHandler() {
  delete uic_;
}

bool PngHandler::Decode(int w, int h) {
  UIC::ImageSamplingGeometry& geometry = uic_->geometry;
  UIC::ImageColorSpec& colorSpec = uic_->colorspec;
  UIC::PNGDecoder* decoder = (UIC::PNGDecoder*)uic_->decoder;

  UIC::Image& imageCn = uic_->image_in;
  UIC::ImageDataPtr     dataPtr;
  UIC::ImageDataOrder   dataOrder;
  UIC::ImageDataType    dataType = colorSpec.DataRange()->DataType();
  IppiSize              roi = {w, h};
  Ipp32u iChannels = geometry.NOfComponents();
  Ipp32u iPrecision = colorSpec.DataRange()->BitDepth() + 1;
  Ipp32u iDU = UIC::NOfBytes(dataType);
  Ipp32u bSigned = 0;
  Ipp32u bFloat = 0;

  if(dataType == T16s || dataType == T32s)
    bSigned = 1, iPrecision++;
  if(dataType == T16f || dataType == T32f)
    bFloat = 1;

  dataOrder.ReAlloc(Interleaved, iChannels);
  dataOrder.SetDataType(dataType);
  dataOrder.PixelStep()[0] = iChannels * iDU;
  dataOrder.LineStep() [0] = roi.width * iChannels * iDU + BYTES_PAD(roi.width, iChannels, iDU);

  imageCn.ColorSpec().ReAlloc(iChannels);
  imageCn.ColorSpec().SetColorSpecMethod(Enumerated);
  imageCn.ColorSpec().SetComponentToColorMap(Direct);
  imageCn.ColorSpec().SetEnumColorSpace(colorSpec.EnumColorSpace());

  for(Ipp32u i = 0; i < iChannels; i++)
    imageCn.ColorSpec().DataRange()[i] = colorSpec.DataRange()[i];

  imageCn.Buffer().ReAlloc(dataOrder, geometry);

  if(ExcStatusOk != decoder->ReadData(imageCn.Buffer().DataPtr(), dataOrder))
    return IE_RDATA;

  return IE_OK;
}

bool PngHandler::Watermark(int pic_width, int pic_height,
    const std::vector<FreeTypeString>& vec, const aligntype align_type,
    int hor_distance, int ver_distance, const std::string& logo_file_name) {
  return false;
}

PngHandler::PngHandler(const PngHandler &rhs) {
}
PngHandler & PngHandler::operator =(const PngHandler &rhs) {
  return *this;
}

static IM_ERROR WritePng(const UicInfoBase* uic, bool* filters,
		unsigned char *buff_ptr, int *buff_size) {
	CMemBuffOutput out;

	out.Open(buff_ptr, *buff_size);

	UIC::PNGEncoder* encoder = uic->png_encoder;

	encoder->Close();
	encoder->SetNOfThreads(1);

	if (ExcStatusOk != encoder->Init())
		return IE_INIT;
	if (ExcStatusOk != encoder->AttachStream(out))
		return IE_ASTREAM;

	if (ExcStatusOk != encoder->AttachImage(uic->image_out))
		return IE_AIMAGE;

	if (ExcStatusOk != encoder->SetParams(filters))
		return IE_PARAMS;

	ExcStatus err = encoder->WriteHeader();
	if (err != ExcStatusOk)
		return IE_WHEADER;

	if (ExcStatusOk != encoder->WriteData())
		return IE_WDATA;

	out.Position(*(UIC::BaseStream::TPosition*) buff_size);

	return IE_OK;
}
bool ImageHandler::EncodePng(unsigned char **fileptr, int *sz) {
	  if (uic_->image_out.Buffer().DataPtr() == 0)
	    return false;
	  PARAMS_PNG params = {0};
	  for (int i = 0; i < 2; ++i) {
	    int size = uic_->grow_bufs->file_out_buf.size();
	    IM_ERROR p = WritePng(uic_, params.filters, uic_->grow_bufs->file_out_buf.ptr(), &size);
	    if (p == IE_WHEADER || p == IE_WDATA) {
	      uic_->grow_bufs->file_out_buf.ReAlloc(size * 2);
	    } else {
	      *fileptr = uic_->grow_bufs->file_out_buf.ptr();
	      *sz = size;
	      return true;
	    }
	  }
	  return false;
}
} /* namespace rtimage */
