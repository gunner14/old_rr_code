#ifndef __USER_DOING_READER_ADAPTER_H__
#define __USER_DOING_READER_ADAPTER_H__

#include "UserDoingInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include <IceUtil/IceUtil.h>
#include <boost/lexical_cast.hpp>
#include "Markable.h"

namespace xce {
namespace adapter {
namespace userdoing {

using namespace ::xce::userbase;
using namespace MyUtil;

class UserDoingReaderAdapter : public MyUtil::ReplicatedClusterAdapterI<UserDoingReaderPrx>,
	public MyUtil::Singleton<UserDoingReaderAdapter>
{
public:
	UserDoingReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserDoingReaderPrx>("ControllerUserBaseReader",120,300,new MyUtil::OceChannel,true,"UDM"){
	}
	UserDoingInfoPtr getUserDoing(int id, const Ice::Context& ctx = Ice::Context());
	void setUserDoing(int id, const MyUtil::Str2StrMap& props);
	void setData(int id, const Ice::ObjectPtr& data);
	UserDoingReaderPrx getUserDoingReaderPrx(int id);
};

}
}
}

#endif

