#include "photo_upload.h"
#include "cwf/frame.h"

#include "runtimeconfig.h"
#include "memcache.h"
#include "processdispatcher.h"
#include "util.h"
#include "base3/logging.h"

namespace photo {
CProcessDispatcher * g_dispatcher;
}
namespace cwf {
void InstallDefaultAction() {

  g_runTimeConfig.Init(); // TODO: 暂时这里初始化,以后放zookeeper中
  g_memcache.Init();
  g_nginxcache.Init();
  photo::g_dispatcher = new CProcessDispatcher();

  static photo::PhotoAction action_;
  cwf::FrameWork::RegisterAction(&action_);
}
void UninstallDefaultAction() {
}
}
