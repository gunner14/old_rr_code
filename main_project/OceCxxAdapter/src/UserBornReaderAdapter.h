#ifndef __USER_BORN_READER_ADAPTER_H__
#define __USER_BORN_READER_ADAPTER_H__

#include "UserBornInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "Markable.h"
#include "boost/lexical_cast.hpp"

namespace xce {
namespace adapter {
namespace userborn {

using namespace ::xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserBornReaderAdapter : public MyUtil::ReplicatedClusterAdapterI<UserBornReaderPrx>,
	public MyUtil::Singleton<UserBornReaderAdapter>
{
public:
	UserBornReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserBornReaderPrx>("ControllerUserBaseReader",120,300,new MyUtil::OceChannel,true,"UBOM"){
	}
	UserBornInfoPtr getUserBorn(int id, const Ice::Context& ctx = Ice::Context());
	void setUserBorn(int id, const MyUtil::Str2StrMap& props);

	void setData(int id, const Ice::ObjectPtr& data);
		

	UserBornReaderPrx getUserBornReaderPrx(int userId);

};

}
}
}

#endif

