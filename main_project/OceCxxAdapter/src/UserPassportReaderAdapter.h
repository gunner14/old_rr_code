#ifndef __USER_PASSPORT_READER_ADAPTER_H__
#define __USER_PASSPORT_READER_ADAPTER_H__

#include "UserPassportInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include <boost/lexical_cast.hpp>
#include "Markable.h"

namespace xce {
namespace adapter {
namespace userpassport {

using namespace xce::userbase;
using namespace MyUtil;

class UserPassportReaderAdapter : virtual public MyUtil::ReplicatedClusterAdapterI<UserPassportReaderPrx>,
	virtual public MyUtil::Singleton<UserPassportReaderAdapter>
{
public:
	UserPassportReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserPassportReaderPrx>( "ControllerUserBaseReader",120,300,new MyUtil::OceChannel,true,"UPM"){

	}
	UserPassportInfoPtr getUserPassport(int id, const Ice::Context& ctx = Ice::Context());
	void setUserPassport(int id, const Str2StrMap& props);
	void setData(int id, const Ice::ObjectPtr& data);

private:
	UserPassportReaderPrx getUserPassportReaderPrx(int id);

};

}
}
}

#endif

