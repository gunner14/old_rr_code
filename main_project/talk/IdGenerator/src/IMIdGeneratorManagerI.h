#ifndef IMIdGeneratorManager_H
#define IMIdGeneratorManager_H

#include "IMIdGenerator.h"
#include "ServiceI.h"
#include "Singleton.h"

namespace xce {
namespace xntalk {
const std::string ID_TABLE = "im_id_generator";
const int BUFFER_MAXSIZE = 5000;
const int ID_SEQ_STEP = 100000;
const int BEGIN_ID = 1;
const std::string DB_INSTANCE = "im";

class IdItem :public IceUtil::Shared {
public:
  Ice::Int current_index_;
  Ice::Int top_index_;
  Ice::Int count_;
  IceUtil::Mutex mutex_;
};
typedef IceUtil::Handle<IdItem> IdItemPtr;

class IMIdGeneratorManagerI : public xce::xntalk::IMIdGeneratorManager,public MyUtil::Singleton<IMIdGeneratorManagerI> {
public:
  virtual Ice::Long getId(Ice::Int type,  const Ice::Current& = Ice::Current());
  virtual Ice::Long createType(Ice::Int type, Ice::Long id, const std::string &note,  const Ice::Current& = Ice::Current());
  
  void InitalizeIdItems();
private:
  IdItemPtr id_items_[BUFFER_MAXSIZE];
};

};
};
#endif
