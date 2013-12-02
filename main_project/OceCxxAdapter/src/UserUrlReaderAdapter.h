#ifndef __USER_URL_READER_ADAPTER_H__
#define __USER_URL_READER_ADAPTER_H__

#include "UserUrlInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "Markable.h"
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userurl{
using namespace xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserUrlReaderAdapter : public MyUtil::ReplicatedClusterAdapterI<UserUrlReaderPrx>,
	public MyUtil::Singleton<UserUrlReaderAdapter>
{
public:
	UserUrlReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserUrlReaderPrx>("ControllerUserBaseReader",120,300,new MyUtil::OceChannel, true,"U"){
	}
	UserUrlInfoPtr getUserUrl(int id, const Ice::Context& ctx = Ice::Context());
	void setData(int id, const Ice::ObjectPtr& data);
	void setUserUrl(int id, const MyUtil::Str2StrMap& props);

	UserUrlReaderPrx getUserUrlReaderPrx(int userId);
};

}
}
}
#endif

