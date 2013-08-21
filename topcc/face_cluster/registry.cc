#include "base3/logging.h"
#include "base3/sysloging.h"
#include "arch_diff/site_xiaonei.h"
#include "cwf/frame.h"
#include "memcache.h"

#include "action_cluster.h"

namespace cwf {
void InstallDefaultAction() {

  // open system log
  logging::opensyslog("cluster.fcgi");

  g_memcache.Init("conf/facememcache.conf");
  xce::Init();
  static upload::ClusterAction action_cluster;
  cwf::FrameWork::RegisterAction(&action_cluster);
}

void UninstallDefaultAction() {
}

}

