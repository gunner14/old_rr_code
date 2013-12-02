#ifndef __USER_CONFIG_READER_ADAPTER_H__
#define __USER_CONFIG_READER_ADAPTER_H__

#include "UserConfig.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "UserConfigInfo.h"

namespace xce {
namespace readeradapter {

using namespace xce::adapter;
class UserConfigReaderAdapter: public MyUtil::ReplicatedClusterAdapterI<mop::hi::svc::model::UserConfigReaderPrx>,
		public MyUtil::Singleton<UserConfigReaderAdapter> {
public:
	UserConfigReaderAdapter() :
		MyUtil::ReplicatedClusterAdapterI<mop::hi::svc::model::UserConfigReaderPrx>("ControllerUserConfigReader", 120, 300) {
	}

	UserConfigInfoPtr getUserConfig(int userId, const Ice::Context& ctx = Ice::Context());

	void setUserConfig(int userId, const MyUtil::Str2StrMap& properties);
	void setData(int userId, const Ice::ObjectPtr& userData);

private:
	mop::hi::svc::model::UserConfigReaderPrx getUserConfigReader(int userId);
};

}
}

#endif

