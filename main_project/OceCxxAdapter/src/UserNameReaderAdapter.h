#ifndef __USER_NAME_READER_ADAPTER_H__
#define __USER_NAME_READER_ADAPTER_H__

#include "UserNameInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "Markable.h"
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace username {

using namespace ::xce::userbase;
using namespace MyUtil;

class UserNameReaderAdapter : public MyUtil::ReplicatedClusterAdapterI<UserNameReaderPrx>,
	public MyUtil::Singleton<UserNameReaderAdapter> {

public:
	UserNameReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserNameReaderPrx>("ControllerUserBaseReader",120,300,new MyUtil::OceChannel,true,"UNM"){
	}
	
  UserNameInfoPtr getUserName(int id, const Ice::Context& ctx = Ice::Context());
  void setUserName(int id, const Str2StrMap& props);
  void setData(int id, const Ice::ObjectPtr& data);
private: 
  UserNameReaderPrx getUserNameReaderPrx(int id);
};

}
}
}

#endif

