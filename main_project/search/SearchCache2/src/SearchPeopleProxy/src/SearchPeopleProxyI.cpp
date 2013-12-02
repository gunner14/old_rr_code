/*
 * FeedQueueI.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#include "SearchPeopleProxyI.h"
#include "LogWrapper.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "boost/foreach.hpp"
#include "map"
#include "search/PeopleSearchKafkaClient/src/client.hpp"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace xce::searchcache;
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace boost;
//using namespace xce::usercache;

static ClientManager cm ;
class PrintHelper {
public:
        template<typename T>
        static string Print(const T & container) {
                using namespace boost;
                ostringstream ios;
                
                ios << "(";
                int idx = 0;
                typedef typename T::value_type vt;
                BOOST_FOREACH(const vt & v,container){
                        if(idx++ > 0) ios << ",";
                        ios << v;
                }
                ios << ")";
                
                return ios.str();
        }
};


static int const EXE_TASK_LEVEL = 300;
class SendTask: public MyUtil::Task {
	public:
		SendTask(const ::MyUtil::Int2StrMap& info) :info(info){
		}
		virtual void handle(){


			typedef MyUtil::Int2StrMap::value_type vt;	
			BOOST_FOREACH(const vt & p, info){
				vector<string> messages;
				messages.push_back(p.second);

				int partitioncnt= cm.getPartitionCnt();
				int kafkacnt = cm.getKafkaCnt();

				int partition= p.first % partitioncnt;
				int kafkaindex= p.first / partitioncnt % kafkacnt;
				KafkaClientPtr c = cm.getClient(kafkaindex);
				c->send(messages, "PeopleSearchUpdate",partition );
				MCE_INFO("SendTask::handle.uid:" << p.first 
						<< " kafkaindex:" << kafkaindex << " kafkacnt:" << kafkacnt
					       	<< " partion:" << partition << " partioncnt:" << partitioncnt << " kafkaip:" << c->getHost()
						);
			}
		}
	private:
		MyUtil::Int2StrMap info;

};

void InitKafka(const string & kafka_addresses){
	//string host0 = "10.2.72.187";
	//string port0= "9092";
	//string host1 = "10.2.72.187";
	//string port1 = "9093";
	
	//string host0 = "ns70.xce.n.xiaonei.com";
	//string port0= "9092";
	//string host1 = "ns71.xce.n.xiaonei.com";
	//string port1 = "9092";

	cm.init(2,64,kafka_addresses);
	bool b1 = cm.getClient(0)->checkConnect();
	bool b2 = cm.getClient(1)->checkConnect();
	MCE_INFO("init result. kafka0:" << b1 << ", kafka1:" << b2 << " kafka_addresses:" << kafka_addresses);
}

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&SearchPeopleProxyI::instance(), service.createIdentity("M", ""));
	Ice::Identity id = service.createIdentity("D","MyCategory");
	MyUtil::ThreadPoolConfig tpc(1,1);
	TaskManager::instance().config(EXE_TASK_LEVEL, tpc);
        string kafka_addresses = service.getCommunicator()->getProperties()->getProperty( "SearchPeopleProxy.KafkaAddresses");
	InitKafka(kafka_addresses);
        int mod = service.getCommunicator()->getProperties()->getPropertyAsInt( "SearchPeopleProxy.Mod");
        int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault( "SearchPeopleProxy.Interval", 5);
        xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerSearchPeopleProxy", &SearchPeopleProxyI::instance(), mod, interval,
                            new SearchControllerChannel());
        xce::serverstate::ServerStateSubscriber::instance().setStatus(1);
}


void  SearchPeopleProxyI::Send(const ::MyUtil::Int2StrMap& infos, const ::Ice::Current& ){
	typedef const ::MyUtil::Int2StrMap::value_type vt;
	BOOST_FOREACH(const vt & info,infos){
		MCE_INFO("SearchPeopleProxyI::Send. uid:" << info.first);
	}
	TaskManager::instance().execute(new SendTask(infos));
}
