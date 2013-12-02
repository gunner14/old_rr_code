#ifndef __BLOCK_RELATION_ADAPTER_H__
#define __BLOCK_RELATION_ADAPTER_H__

#include <Singleton.h>
#include <QueryRunner.h>

namespace xce {
namespace blockrelation {
namespace adapter {

class BlockRelationAdapter: public MyUtil::Singleton<BlockRelationAdapter> {
public:
	BlockRelationAdapter() {
		com::xiaonei::xce::ConnectionPoolManager::instance().initialize();
	}
	MyUtil::IntSeq getBlockerList(int userId, int begin, int limit);

};

class IdListResultHandler: virtual public com::xiaonei::xce::ResultHandler {
public:
	IdListResultHandler(MyUtil::IntSeq& data, const std::string& field) :
		_data(data), _field(field) {
	}
protected:
	virtual bool handle(mysqlpp::Row& res) const;
private:
	MyUtil::IntSeq& _data;
	std::string _field;
};

}
}
}

#endif
