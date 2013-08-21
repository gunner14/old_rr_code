#include "ClusterStateSubscriber.h"
#include "ClientInterface.h"
#include "SbjTopicI.h"

using namespace std;
using namespace Ice;
using namespace MyUtil;
using namespace xce::clusterstate;
using namespace xce::serverstate;
using namespace xce::clustercontroller;

// ======= ClusterStateSubscriber =====================================
ClusterStateSubscriber::ClusterStateSubscriber(ClusterClient& client, std::string ctlEndpoints, int interval, const MyUtil::ChannelPtr& channel):
		_client(client),
		_ctlEndpoints(ctlEndpoints),
		_interval(interval),
    _channel(channel)	{
	init();
}

void ClusterStateSubscriber::init() {

	// 初始化 Channel和Communicator
	_ic= _channel->getCommunicator();

	// 注册SbjTopic
	SbjTopicIPtr topic = new SbjTopicI("ClusterState", this, _ctlEndpoints, _interval, _channel);
	_mapTopics.insert(pair<string, SbjTopicIPtr>("ClusterState", topic));

	// 启动时, 从controller获取服务信息
	getInitState();

	// 调用使用者的renew方法 
	// renew改为同步 -- 09.07.17 by zhanghan
	// TaskManager::instance().execute(new RenewTask(_client, _clusterState));
	_client.renew(_clusterState);
}

SbjStatePtr ClusterStateSubscriber::opera(const ::MyUtil::SbjStatePtr& sbjstate, const ::Ice::Current& current) {

	ClusterStateInfoPtr csStatePtr = ClusterStateInfoPtr::dynamicCast(sbjstate);
	
	ServerStateMap tmp(csStatePtr->states);
	IceUtil::Mutex::Lock lock(*this);	

	// 备份现有ClusterState
	_oldState.swap(_clusterState);	

	// 对新赋值
	_clusterState.swap(tmp);
	
	// 触发注册的ClusterClient的行为
	_client.renew(_clusterState);

	/* SbjStatePtr emptyptr = new SbjState;
	return emptyptr; */
	return sbjstate;
}

ServerStateMap& ClusterStateSubscriber::getClusterState() {

	return _clusterState;
}

ServerStateMap& ClusterStateSubscriber::getOldClusterState() {
	
	return _oldState;
}

void ClusterStateSubscriber::getInitState() {

	// 获取controller的prx
	Ice::ObjectPrx object = _channel->getCommunicator()->stringToProxy(string("M@")+_ctlEndpoints);
	ControllerManagerPrx prx = ControllerManagerPrx::uncheckedCast(object->ice_timeout(1000)->ice_twoway());

	try {
	       	_clusterState = prx->GetServerState()->states;
	} catch (Ice::Exception& e) {
	} catch (std::exception& e) {
	} catch (...) {
	}
}

void RenewTask::handle() {
	_client.renew(_clusterState);
}

