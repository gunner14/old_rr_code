#include "ServiceBase.h"

namespace xoa2 {

class ServiceBaseImpl : virtual public ServiceBaseIf {
 public:
  virtual void Stop();
};

}

