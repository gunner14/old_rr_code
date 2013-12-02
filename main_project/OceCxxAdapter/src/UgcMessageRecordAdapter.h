
#ifndef UGC_MESSAGERECORD_ADAPTER_H
#define UGC_MESSAGERECORD_ADAPTER_H

#include "AdapterI.h"
#include "Singleton.h"

// 注意：对应的版本的slice文件是ugc提供的，没有进行改名
#include "MessageAdapter.h"

namespace renren {
namespace com {
namespace message {
namespace client {

using namespace MyUtil;

class UgcMessageRecordAdapter: public MyUtil::AdapterI,
		public AdapterISingleton<MyUtil::ImUgcChannel, UgcMessageRecordAdapter> {

public:
	UgcMessageRecordAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }

  void saveMessge(const MessageVO &msg);

protected:
	virtual string name() {
		return "UGCGroupMessageRecord";
	}
	virtual string endpoints() {
		return "@UGCGroupMessageRecord";
	}
	virtual size_t cluster() {
		return 1;
	}

	MessageAdapterPrx  getManager(int id);
	MessageAdapterPrx  getManagerOneway(int id);

	vector<MessageAdapterPrx>  _managersOneway;
	vector<MessageAdapterPrx>  _managers;
};

}
}
}
}
#endif
