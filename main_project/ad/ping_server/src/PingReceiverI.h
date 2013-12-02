#ifndef __AD_PING_RECEIVER_H__
#define __AD_PING_RECEIVER_H__

#include <string>
#include "PingReceiver.h"
#include "Singleton.h"
#include "PingInfoManager.h"

namespace xce{
namespace ad{

class PingReceiverI: public PingReceiver, public MyUtil::Singleton<PingReceiverI> {
public:
  PingReceiverI(){
    init_ok_ = false;
  }
  void Initialize(const int threshold, const string& DbInstance);
  void WriteBatch(const xce::ad::ServerMethodSeq&, const Ice::Current&);
  void initOK(); 
  //测试
  //void SendEmailTest();
private:
  bool init_ok_;
  int threshold_;
  string db_source_;
  PingInfoManagerPtr manager_;
};

}
}
#endif
