#ifndef __XCE_STORMI_H__
#define __XCE_STORMI_H__

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "NotifyStorm.h"
#include "ServiceI.h"
#include <ext/hash_map>
#include "util/cpp/String.h"

namespace xce {
namespace notify {

using namespace std;
using namespace __gnu_cxx;
using namespace MyUtil;

const static string NOTIFY_STORM = "M";
const static int kPeriodOfPingAllInMilliseconds = 20*60*1000;
const static int kPeriodOfPingOneInMilliseconds = 30*1000;
const static int kMaximumTimesOfPing = 2;
const static int kPeriodOfNotifyInMilliseconds = 30*1000;
const static int kMaximumTimesOfNotify = 3; //重试3次
const static int kMaximumTimerTaskChannel = 10;

struct ObserverLessTo: public binary_function<ObserverPrx, ObserverPrx, bool> {
  bool operator()(const ObserverPrx& __x, const ObserverPrx& __y) const {
    return __x->ice_getIdentity() < __y->ice_getIdentity();
  }
};

class NotifyStormI: public NotifyStorm,
    public IceUtil::Mutex,
    public Singleton<NotifyStormI> {
public:
  virtual void notify(const string& name, const MyUtil::Str2StrMap& context,
      const Ice::Current& = Ice::Current());

  virtual void subscribe(const string& name, const ObserverPrx& observer,
      const Ice::Current& = Ice::Current());
  virtual void unsubscribe(const string& name, const ObserverPrx& observer,
      const Ice::Current& = Ice::Current());

  virtual ObserverPoolMap query(const string& name, const Ice::Current& =
      Ice::Current());
private:
  typedef set<ObserverPrx, ObserverLessTo> ObserverSet;
  typedef hash_map<string, ObserverSet> ObserverSetMap;
  ObserverSetMap _pool;
};

class NotifyTask: public MyUtil::Task {
public:
  NotifyTask(int channel, const string& name, const ObserverPrx& observer, 
      const Ice::Context& context, int retry) :
      MyUtil::Task(channel), _name(name), _observer(observer),
        _context(context), _retry(retry) {}
  virtual void handle();
  virtual void exception(const string& message);
private:
  string _name;
  ObserverPrx _observer;
  Ice::Context _context;
  int _retry;
};

class PingTimerTask: public MyUtil::Timer {
 public:
  PingTimerTask(int delay, int channel, const string& name) : 
      Timer(delay, channel), _name(name) {}
  virtual void handle();
 private:
  string _name;
};

class NotifyStormPrintHelper {
 public:
  template<typename T>
  static string Print(const T& container, int limit = 100) {
    ostringstream ios;
    ios << "(";
    bool first_item = true;
    int count = 0;
    for (typename T::const_iterator it = container.begin(); it
        != container.end(); ++it, ++count) {
      if (first_item == true) {
        first_item = false;
      } else {
        ios << ",";
      }
      ios << *it;
      if (count >= limit) {
        break;
      }
    }
    ios << ")";
    return ios.str();
  }
  static string Print(const map<string, string>& map_str, int limit = 20) {
    if (map_str.empty()) {
      return "NULL";
    }
    ostringstream o_str_stream;
    o_str_stream << "(";
    map<string, string>::const_iterator it = map_str.begin();
    o_str_stream << it->first;
    o_str_stream << ":" << it->second;
    ++it;
    for (int count = 0; it != map_str.end(); ++it, ++count) {
      o_str_stream << "," << it->first << ":" << it->second;
      if ( count >= limit) { 
        break; 
      }
    }
    o_str_stream << ")";
    return o_str_stream.str();
  }
};

}
}

#endif
