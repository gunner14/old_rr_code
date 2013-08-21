#include "base3/logging.h"
#include "base3/sysloging.h"
#include "arch_diff/site_xiaonei.h"
#include "cwf/frame.h"
#include "memcache.h"

#include "action_detect.h"

namespace cwf {
void InstallDefaultAction() {

  // open system log
  logging::opensyslog("detect");

  g_memcache.Init("conf/facememcache.conf");
  xce::Init();
  static upload::DetectAction action_detect;
  cwf::FrameWork::RegisterAction(&action_detect);
}

void UninstallDefaultAction() {
}

}

