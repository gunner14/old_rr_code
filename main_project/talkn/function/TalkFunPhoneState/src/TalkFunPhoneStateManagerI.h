#ifndef __TALK_FUN_PHONE_STATE_MANAGERI__
#define __TALK_FUN_PHONE_STATE_MANAGERI__


#include "ServiceI.h"
#include "TalkFunPhoneState.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace fun{
namespace state{

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
const float TIMEOUT = 200.0;

class OnlineUids {
 private:
	IceUtil::Mutex mutex_;
  map<int, int> uids_;

  enum {
    ONLINE_TIMEOUT = 600,
  };


 public:
  size_t add(int uid);
  bool del(int uid);
  bool check(int uid);

  size_t size();
  size_t check_all();

  void get_uids(map<int, int> &uids);

  //void set(const vector<int> &uids);
  //void get(const vector<int> &uids, vector<int> &res);
};

class ControllerSub : virtual public IceUtil::Thread {
 private:
  int start_time_;
  int mod_;
  enum {
    SUB_CONTROLLER_GET = 60 * 15,
  };

  //  typedef MyUtil::ReplicatedClusterAdapterI<TalkFunPhoneStateManagerPrx> t_adapter;
 public:

 ControllerSub(int mod) : start_time_(time(NULL)), mod_(mod) {}
  void showState();
  void showState2();
  virtual void run();
};

typedef IceUtil::Handle<ControllerSub> ControllerSubPtr;


class TalkFunPhoneStateManagerI: public TalkFunPhoneStateManager, public Singleton<TalkFunPhoneStateManagerI>{
 private:
  enum {
    POOL_SIZE = 98765,
  };

  OnlineUids pool_[POOL_SIZE];
  ControllerSubPtr thread_;


 public:

  TalkFunPhoneStateManagerI() {
  }
  void startSub(int mod);
  void stat();
  size_t size();
  size_t check_all();
  size_t checkPool();
  virtual int sizeAll(const Ice::Current & = Ice::Current());
  virtual int checkAll(int idx, const Ice::Current & = Ice::Current());
  virtual map<int, int> getPoolUids(int idx, const Ice::Current & = Ice::Current());
  virtual void HeartBeatPhone(const MyUtil::IntSeq& uids, const Ice::Current& = Ice::Current()); 
  virtual void setStatus(int st, const Ice::Current & = Ice::Current());
  virtual void Offline(int uid, const Ice::Current& = Ice::Current());
  virtual void Online(int uid, const Ice::Current& = Ice::Current());
  virtual MyUtil::IntSeq getUsersIds(const MyUtil::IntSeq& uids, const Ice::Current& = Ice::Current());


};

	

};
};
};
};
};

#endif

