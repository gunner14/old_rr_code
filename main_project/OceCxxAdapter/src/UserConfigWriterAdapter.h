#ifndef __USER_CONFIG_WRITER_ADAPTER_H__
#define __USER_CONFIG_WRITER_ADAPTER_H__

#include "UserConfig.h"
#include "Singleton.h"
#include "AdapterI.h"
#include <ext/hash_set>
#include <IceUtil/IceUtil.h>
#include "UserConfigInfo.h"

namespace xce {
namespace adapter {

using namespace mop::hi::svc::model;
using namespace __gnu_cxx;

class UserConfigWriterAdapter: public MyUtil::ReplicatedClusterAdapterI<UserConfigWriterPrx>, public MyUtil::Singleton<UserConfigWriterAdapter>, IceUtil::Thread {
public:
	UserConfigWriterAdapter() :
		MyUtil::ReplicatedClusterAdapterI<UserConfigWriterPrx>("ControllerUserConfigWriter", 120, 300) {
		start();
	}

	void setUserConfig(int userId, const UserConfigInfoPtr& info);
	void createUserConfig(int userId, const UserConfigInfoPtr& info);
	void reload(int userId);

protected:
	void run();

private:
	UserConfigWriterPrx getUserConfigWriter(int userId);
	UserConfigWriterPrx getUserConfig10sWriter(int userId);

private:
	IceUtil::Monitor<IceUtil::Mutex> _mutex;
	hash_set<int> _idSet;
};

}
}

#endif

