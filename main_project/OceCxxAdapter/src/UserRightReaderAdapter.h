#ifndef __USER_RIGHT_READER_ADAPTER_H__
#define __USER_RIGHT_READER_ADAPTER_H__

#include "UserRightInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "Markable.h"
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userright {

using namespace xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserRightReaderAdapter : public MyUtil::ReplicatedClusterAdapterI<UserRightReaderPrx>,
	public MyUtil::Singleton<UserRightReaderAdapter>
{
public:
	UserRightReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserRightReaderPrx>("ControllerUserBaseReader",120,300,new MyUtil::OceChannel, true,"URM"){
	}
	UserRightInfoPtr getUserRight(int id, const Ice::Context& ctx = Ice::Context());
	void setUserRight(int id, const MyUtil::Str2StrMap& props);
	void setData(int id, const Ice::ObjectPtr& data);
	UserRightReaderPrx getUserRightReaderPrx(int userId);

};

}
}
}
#endif

