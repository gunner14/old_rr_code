#ifndef _IMAGE_PROCESSOR_H_
#define _IMAGE_PROCESSOR_H_
#include <string>
#include <map>
#include<Magick++.h>
#include "upload_head.h"
#include "runtimeconfig.h"
#include "cwf/stream.h"
/*
 namespace cwf {
 struct Request;
 struct Request::FormDisposition;
 };*/
namespace Magick {
class Image;
}
;
namespace photo {

typedef enum {
  PROCESS_TYPE_COMMON,
  PROCESS_TYPE_COPY,
  PROCESS_TYPE_TOTAL,
} ENUM_PROCESS_TYPE;
class ImageHandler {
 public:
  typedef std::map<std::string, std::string> WaterMarkDataMap;
 public:
  bool Route(const ServiceConf *sc, double deg, const std::string &data,
             upload::OutFile *outfile);
  int ProcessImage(const ServiceConf* sc, const std::string &comment,
                   cwf::Request::DispositionArrayType &files,
                   const ENUM_PROCESS_TYPE type,
                   const Magick::Geometry& crop_geom,
                   upload::OutResponse *resp);
  int ProcessImage(const ServiceConf* sc, const std::string &comment,
                   const cwf::Request::DispositionArrayType &files,
                   const int crop_x, const int crop_y,
                   upload::OutResponse *resp);

 private:
  bool VerityFile(const ServiceConf* sc, upload::OutFile *outfile);
  bool VerityImage(const std::string &data, int minwidth, int minheight,
                   upload::OutFile *outf, Magick::Image *image, int *width,
                   int *height, std::string *magick, bool bDecode = true);
  bool Handle(const ServiceConf &sc, double dep, const std::string &comment,
              const std::string &watermark, const std::string &data,
              const Magick::Geometry& crop_geom, upload::OutFile *outf);
  bool HandleCopyImage(const ServiceConf &sc, const std::string &comment,
                       const std::string &watermark, const std::string &data,
                       upload::OutFile *outf);
  bool HandleHeadImage(const ServiceConf &sc, const std::string &comment,
                       const std::string &data, const int crop_x,
                       const int crop_y, upload::OutFile *outf);
  WaterMarkDataMap watermark_data_;
};

}
#endif
