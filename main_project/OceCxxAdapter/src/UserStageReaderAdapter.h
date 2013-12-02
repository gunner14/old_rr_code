#ifndef __USER_STAGE_READER_ADAPTER_H__
#define __USER_STAGE_READER_ADAPTER_H__

#include "UserStageInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include <boost/lexical_cast.hpp>
#include "Markable.h"

namespace xce {
namespace adapter {
namespace userstage {

using namespace ::xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserStageReaderAdapter : public MyUtil::ReplicatedClusterAdapterI<UserStageReaderPrx>,
	public MyUtil::Singleton<UserStageReaderAdapter>
{
public:
	UserStageReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserStageReaderPrx>("ControllerUserBaseReader",120,300,new MyUtil::OceChannel,true,"USGM"){
	}
	UserStageInfoPtr getUserStage(int id, const Ice::Context& ctx = Ice::Context());
	void setUserStage(int id, const MyUtil::Str2StrMap& props);
	void setData(int id, const Ice::ObjectPtr& data);
	UserStageReaderPrx getUserStageReaderPrx(int userId);
};

}
}
}

#endif

