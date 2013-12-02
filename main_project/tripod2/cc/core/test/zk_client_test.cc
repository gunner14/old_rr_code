#include "gtest/gtest.h"

#include <boost/thread/thread.hpp>
#include <boost/foreach.hpp>
#include "base/logging.h"
#include "base/port.h"

#include "LogWrapper.h"

#include "tripod2/cc/core/zk_client.h"


namespace test {


using namespace xce::tripod;

class MyListener : public ZNodeListener {
 public:
  MyListener(const std::string& ns) : ZNodeListener(ns) {

  }
  bool Update(const std::list<std::string>& children_names) {
    static int loop = 1;
    BOOST_FOREACH(const std::string& name, children_names) {
      MCE_DEBUG("\tname:" << name);
    }
    if (++loop > 5) {
      loop = 1;
      return true;
    }
    return true;
  }
};


TEST(zkclient, test) {
  MCE_DEFAULT_INIT("DEBUG");
  ZkClientPtr zc = ZkClient::GetInstance("10.2.132.209:2181");
  MCE_DEBUG("zc.use_count():" << zc.use_count());
  std::list<std::string> nodes;
  std::string ns("/test2");
  zc->GetNodes(ns, &nodes);
  BOOST_FOREACH(const std::string& node, nodes) {
    MCE_DEBUG("node:" << node);
  }
  zc->AddZNodeListener(new MyListener("/test2"));
  Sleep(50000);
  zc.reset();
}

} //  test


