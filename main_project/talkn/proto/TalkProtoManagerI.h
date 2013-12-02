#ifndef __TALK_PROTO_MANAGERI__
#define __TALK_PROTO_MANAGERI__


#include "ServiceI.h"
#include "TalkProto.h"
#include "TalkFunctionAdapter.h"
#include <IceUtil/RWRecMutex.h>

namespace com{
namespace xiaonei{
namespace talk{
namespace proto{

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::adapter::function;

class TplInfo: public IceUtil::Shared {
public:
  string description_;
  map<string, string> tpls_;
  vector<string> important_keys_; 
};
typedef IceUtil::Handle<TplInfo> TplInfoPtr;

class CheckerInfo : public IceUtil::Shared{
public:
	CheckerInfo(const string& key, const string& value){
		checker_key_ = key;
		checker_value_ = value;
    counter_ = 0;
	}
	CheckerInfo(){checker_key_ = ""; checker_value_ = ""; }
	bool operator < (const CheckerInfo& c1) const{ 
    return (checker_key_+checker_value_) < (c1.checker_key_+c1.checker_value_);
	}
	bool operator == (const CheckerInfo& c1) const{ 
		if(c1.checker_key_ == checker_key_ && c1.checker_value_ == checker_value_){
			return true;
		}
		return false;
	}
	string checker_value_;
	string checker_key_;
  volatile long counter_;
};

class vola_type {
public:
  volatile long var_;
  friend ostream& operator << (ostream& out, const vola_type& vt) {
    out << vt.var_;
    return out;
  }
  void operator = (long var) {
    var_ = var;
  }
};

class ParasConfig {
public:
  string config_type;
  string item_type;
  map<string, string> tpls;
  vector<Ice::Int> to_ids;
  Ice::Int online_type;
  string from_jid;
  string to_jid;
  Ice::Int msg_type;
  set<string> replaceable_keys;
};

typedef IceUtil::Handle<CheckerInfo> CheckerInfoPtr;

class TalkProtoManagerI: public TalkProtoManager, public Singleton<TalkProtoManagerI>{
public:
	virtual void Send(const JidPtr& actor, const string& xml, const Ice::Current& = Ice::Current());
	virtual void Receive(const Str2StrMap& paras, const Ice::Current& = Ice::Current());
	virtual void BatchReceive(const Str2StrMapSeq& parasSeq, const Ice::Current& = Ice::Current());
  virtual void ReceiveExpress(const Str2StrMap& paras, const Ice::Current& = Ice::Current());
  virtual void BatchReceiveExpress(const Str2StrMapSeq& paraSeq, const Ice::Current& = Ice::Current());
	void LoadProtoConfig(const string& configpath);
	void LoadTplConfig(const string& configpath);
	void GetAdapter(vector<string>& controllernames, vector<TalkFunctionAdapterPtr>& ptrs, const Str2StrMap& mp);

  void PrintCounter();

private:
	bool HasAdapter(const string& adaptername);
	void AddAdapter(const string& controllername, const string& channel);

  bool CheckParas(const Str2StrMap &paras, ParasConfig& pc);
  string GetTplByJid(const JidPtr &jid,const std::string &config_type,const std::string &item_type) ;

	map<string, TalkFunctionAdapterPtr> adapters_;//controllername <--> adapter
  // 一个check可对应多个controller
	map<CheckerInfoPtr, vector<string> > checkers_; //checkerinfo <--> controllername;
	IceUtil::RWRecMutex rwmutex_;

  map<Ice::Int, TplInfoPtr> tpls_info_;
  IceUtil::RWRecMutex tpls_info_mutex_;

  hash_map<string, vola_type> tpl_counter_;
};

class CounterTimer : public Timer, public Singleton<CounterTimer> {
public:
  CounterTimer() : Timer(1000 * 60 * 20) {} 
  virtual void handle();
};
	
};
};
};
};

#endif

