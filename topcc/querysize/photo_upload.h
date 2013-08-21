#ifndef _CWF_PHOTO_UPLOAD_H_
#define _CWF_PHOTO_UPLOAD_H_
#include <vector>
#include <map>
#include "cwf/action.h"
#include "imagehandler.h"

namespace photo {

using namespace cwf;

class PhotoAction : public BaseAction {
 public:
  PhotoAction();
 private:
  bool Match(const std::string& url) const;
  HttpStatusCode Process(Request*, Response*);
 private:
  ImageHandler imagehandler_;
};

} // namespace 
#endif 
