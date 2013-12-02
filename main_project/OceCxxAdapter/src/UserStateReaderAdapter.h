#ifndef __USER_STATE_READER_ADAPTER_H__
#define __USER_STATE_READER_ADAPTER_H__

#include "UserStateInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "Markable.h"
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userstate{

using namespace xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserStateReaderAdapter : public MyUtil::ReplicatedClusterAdapterI<UserStateReaderPrx>,
	public MyUtil::Singleton<UserStateReaderAdapter>
{
public:
	UserStateReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserStateReaderPrx>("ControllerUserBaseReader",120,300,new MyUtil::OceChannel,true,"USM"){

	}

	void setStateOn(int id,int pos);
	void setStateOff(int id, int pos);
	UserStateInfoPtr getUserState(int id, const Ice::Context& ctx = Ice::Context());
	void setUserState(int id, const MyUtil::Str2StrMap& props);
	void setData(int id, const Ice::ObjectPtr& data);

	UserStateReaderPrx getUserStateReaderPrx(int id);

};

}
}
}

#endif

