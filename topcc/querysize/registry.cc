#include "base3/logging.h"
#include "arch_diff/site_xiaonei.h"
#include "cwf/frame.h"

#include "photo_upload.h"
#include "memcache.h"

namespace cwf {
void InstallDefaultAction() {

  g_nginxcache.Init();

  xce::Init(); 
  static photo::PhotoAction action_;
  cwf::FrameWork::RegisterAction(&action_);
}

void UninstallDefaultAction() {
}

}
