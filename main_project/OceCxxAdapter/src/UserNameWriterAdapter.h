#ifndef __USER_NAME_WRITER_ADAPTER_H__
#define __USER_NAME_WRITER_ADAPTER_H__

#include <ext/hash_set>
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include <IceUtil/IceUtil.h>
#include <boost/lexical_cast.hpp>
#include "Markable.h"

namespace xce {
namespace adapter {
namespace username{

using namespace ::xce::userbase;
using namespace __gnu_cxx;
class UserNameWriterAdapter : public MyUtil::ReplicatedClusterAdapterI<UserNameWriterPrx>,
	public MyUtil::Singleton<UserNameWriterAdapter>, IceUtil::Thread
{
public:
	UserNameWriterAdapter() : MyUtil::ReplicatedClusterAdapterI<UserNameWriterPrx>("ControllerUserBaseWriter",120,300,new MyUtil::OceChannel,true,"UNM"){
		start();
	}

	void reload(int id);

protected:

	void run();

private:

	UserNameWriterPrx getUserNameWriter(int userId);

private:

	IceUtil::Monitor<IceUtil::Mutex> _mutex;
	hash_set<int> _idSet;
};

}
}
}

#endif

