#ifndef _MUSIC_PROCESSOR_H_
#define _MUSIC_PROCESSOR_H_
#include <string>
#include "cwf/stream.h"
#include "upload_head.h"

struct ServiceConf;

namespace music {

class MusicHandler {
 public:
  int ProcessMusic(const ServiceConf* sc, const cwf::Request::DispositionArrayType &files, upload::OutResponse *resp);

};

}
#endif
