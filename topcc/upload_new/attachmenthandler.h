#ifndef _ATTACHMENT_PROCESSOR_H_
#define _ATTACHMENT_PROCESSOR_H_
#include <string>
#include "cwf/stream.h"
#include "upload_head.h"

struct ServiceConf;

namespace attachment {

class AttachmentHandler {
 public:
  int ProcessAttachment(const ServiceConf* sc, const cwf::Request::DispositionArrayType &files, upload::OutResponse *resp);

};

}
#endif
