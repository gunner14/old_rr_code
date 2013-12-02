#ifndef SOCIALGRAPH_CORE_LOGIC_I_H_
#define SOCIALGRAPH_CORE_LOGIC_I_H_

#include "SocialGraphCoreLogic.h"

#include "IceExt/src/ServiceI.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "UtilCxx/src/TaskManager.h"

#include "sgmessage_handler.h"

namespace xce {
namespace socialgraph {

const static int LEVEL_NOTIFY_HANDLER = 5;			//notify all handler
const static std::string BEHAVIOUR = "behaviour";		//key in configuration

//-------------------------------------------------------------------

class SocialGraphCoreLogicI : virtual public SocialGraphCoreLogic, 
		virtual public MyUtil::Singleton<SocialGraphCoreLogicI> {
 public:
	SocialGraphCoreLogicI() { }
	virtual ~SocialGraphCoreLogicI() { }

	virtual void SendSGMessage(const SGMessage&, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<SocialGraphCoreLogicI>;
};

//-------------------------------------------------------------------

class NotifyTask : public MyUtil::Task {
 public:
	NotifyTask(const SGMessage& message, int level = LEVEL_NOTIFY_HANDLER):
		message_(message) {
	}

	virtual void handle();

 private:
	SGMessage message_;
};

}
}

#endif
