#include "base3/logging.h"
#include "base3/sysloging.h"
#include "arch_diff/site_xiaonei.h"
#include "cwf/frame.h"
#include "runtimeconfig.h"
#include "memcache.h"
#include "blockmanage.h"

#include "action_upload.h"
#include "tickmanager.h"

namespace cwf {
void InstallDefaultAction() {

  g_runtime_config.Load(); // TODO: 暂时这里初始化,以后放zookeeper中
  gDiskCache.Init("conf/diskcache.conf");
  // open system log
  logging::opensyslog("upload.fcgi");

  upload::g_blockmanager.Init("conf/blockmemcache.conf");
  upload::g_tickmanager.Init("conf/mc.conf");
  xce::Init(); 
  static upload::UploadAction action_;
  cwf::FrameWork::RegisterAction(&action_);
}

void UninstallDefaultAction() {
}

}

