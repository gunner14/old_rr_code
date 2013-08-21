#include "base3/logging.h"
#include "base3/sysloging.h"
#include "cwf/frame.h"
#include "memcache.h"

#include "action_face_store.h"

namespace cwf {
void InstallDefaultAction() {

  // open system log
  logging::opensyslog("facestore");

  g_memcache.Init("conf/facememcache.conf");
  static upload::StoreFaceAction action;
  cwf::FrameWork::RegisterAction(&action);
}

void UninstallDefaultAction() {
}

}

