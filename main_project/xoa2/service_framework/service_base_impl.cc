#include "service_base_impl.h"

#include <iostream>

namespace xoa2 {

void ServiceBaseImpl::Stop() {
  // service_->Stop();
  std::cout << "Stop() called, to stop service..." << std::endl;
}

}

