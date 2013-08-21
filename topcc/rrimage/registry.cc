//#include "base3/logging.h"
#include "cwf/frame.h"

#include "action_runimage.h"

namespace cwf {
void InstallDefaultAction() {

  static runimage::RunImageAction action_;
  cwf::FrameWork::RegisterAction(&action_);
}

void UninstallDefaultAction() {
}

}
