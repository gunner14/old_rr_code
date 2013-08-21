#ifndef _CWF_PHOTO_UPLOAD_H_
#define _CWF_PHOTO_UPLOAD_H_
#include "cwf/action.h"

namespace photo {

using namespace cwf;
class PhotoAction : public BaseAction {
  bool Match(const std::string& url) const;
  HttpStatusCode Process(Request*, Response*);

};





} // namespace 
#endif 
