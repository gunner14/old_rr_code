#ifndef _CWF_ACTION_UPLOAD_H_
#define _CWF_ACTION_UPLOAD_H_

#include <vector>
#include <map>
#include <string>
#include "cwf/action.h"
#include "datadispense.h"
#include "imagehandler.h"
#include "musichandler.h"
#include "attachmenthandler.h"
#include "memcache.h"

namespace Magick {
class Geometry;
}

namespace upload {

using namespace cwf;

class UploadAction : public BaseAction {
 public:
  UploadAction();
  void Stop();
 private:
  bool Match(const std::string& url) const;
  HttpStatusCode Process(Request*, Response*);
  bool ParseCropGeom(const std::string& crop_geom_str,
                     Magick::Geometry* crop_geom);
 private:
  DataDispense disp_;
  photo::ImageHandler imagehandler_;
  music::MusicHandler musichandler_;
  attachment::AttachmentHandler attachmenthandler_;
  const static std::string kMethods_[];
};

}  // namespace
#endif 
