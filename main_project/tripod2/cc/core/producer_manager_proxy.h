#ifndef PRODUCER_MANAGER_PROXY_H_
#define PRODUCER_MANAGER_PROXY_H_

#include <vector>

#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"

#include "tripod2/cc/core/common.h"
#include "tripod2/cc/core/zk_client.h"

#include "Ice/Ice.h"
#include "OceSlice/slice/tripod2.h"

namespace xce {
namespace tripod {

class ProducerManagerProxy : public ZNodeListener {
 public:
  ProducerManagerProxy(const std::string& ns, const std::string biz, int conn_time_out);
  virtual ~ProducerManagerProxy();
  virtual bool Update(const std::list<std::string>& children_names);

  bool Produce(const std::string& key, std::string* value, bool use_master);

  bool IsValid() const {
    return valid_ == true;
  }

 private:
  ProducerManagerPrx LocateProducerManagerProxy(const std::string& key);

  void set_valid(bool v) {
    valid_ = v;
  }

 private:
  std::string namespace_id_;
  std::string biz_id_;
  int conn_time_out_;

  bool valid_;

  Ice::CommunicatorPtr communicator_;

  boost::shared_mutex shared_mutex_;
  typedef std::vector<ProducerManagerPrx> ProducerManagerPrxSeq;
  ProducerManagerPrxSeq producer_manager_prx_seq_;
};


}} // xce::tripod
#endif

