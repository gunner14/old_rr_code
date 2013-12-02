#ifndef SG_MESSAGE_HANDLER_H_
#define SG_MESSAGE_HANDLER_H_

#include "SocialGraphCoreLogic.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/mutex.hpp>

namespace xce {
namespace socialgraph {

//Existed Action Type
#define RemoveRcdCache "RemoveRcdCache"
#define InsertSGBlock "InsertSGBlock"
#define RemoveRedisCache "RemoveRedisCache"
#define NotifyFeedDistribution "NotifyFeedDistribution"
#define NotifyGuideUGCServer "NotifyGuideUGCServer"
#define RemoveFriendCluster "RemoveFriendCluster"
#define RemoveRcdPageCfTripod "RemoveRcdPageCfTripod"

//-------------------------------------------

typedef std::vector<unsigned char> BinaryData;

//-------------------------------------------

class SGMessageHandler {
 public:
	virtual void Update(const SGMessage& sgm) {}
};
typedef boost::shared_ptr<SGMessageHandler> HandlerPtr;

//-------------------------------------------
/*RemoveRcdCache handler*/
class RemoveRcdCacheHandler : public SGMessageHandler {
 public:
	void Update(const SGMessage& sgm);
};

//-------------------------------------------
/*RemoveRedisCache handler*/
class RemoveRedisCacheHandler : public SGMessageHandler {
 public:
	void Update(const SGMessage& sgm);
};

//-------------------------------------------
/*InsertSGBlock handler*/
class InsertSGBlockHandler : public SGMessageHandler {
 public:
	void Update(const SGMessage& sgm);
};

//-------------------------------------------
/*RemoveFriendClusterHandler handler*/
class RemoveFriendClusterHandler : public SGMessageHandler {
 public:
	void Update(const SGMessage& sgm);
};

//-------------------------------------------
/*Notify RemoveRcdPageCfTripodHandler handler*/
class RemoveRcdPageCfTripodHandler : public SGMessageHandler {
 public:
	void Update(const SGMessage& sgm);
};

//-------------------------------------------
/*Notify feed Distribution manager handler*/
class NotifyFeedDistributionHandler : public SGMessageHandler {
 public:
	void Update(const SGMessage& sgm);
};

//-------------------------------------------
/*Notify GuideUGCServer manager handler*/
class NotifyGuideUGCServerHandler : public SGMessageHandler {
 public:
	void Update(const SGMessage& sgm);
};

//-------------------------------------------
/*Create message handler according to behaviour read from configuration*/
class HandlerFactory {
 public:
	static HandlerPtr Create(const std::string& behaviour);
};

}}

#endif
