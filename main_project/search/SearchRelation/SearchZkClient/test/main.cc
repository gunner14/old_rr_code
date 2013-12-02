

#define MCE_INFO(x) (cout << (x) << endl)
#define private public
#include "../src/zk_client.h"
#include "../src/zk_event_listener.h"
#include "IceExt/src/IceLogger.h"

//#include "/data/home/jingwei.ye/main_project/trunk/search/SearchZkClientV2/src/zk_client.h"
//#include "/data/home/jingwei.ye/main_project/trunk/search/SearchZkClientV2/src/zk_event_listener.h"
#undef private


#define BOOST_TEST_MODULE timeline_test 
#include <boost/test/auto_unit_test.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH
#define MCE_INFO(x) (cout << (x) << endl)

using namespace std;
using namespace boost;
using namespace search::relation;
using namespace MyUtil;


BOOST_AUTO_TEST_SUITE(abc)

class LoaderListerner : public ZkEventListener{
	public:
		LoaderListerner(){ }
		virtual ~LoaderListerner(){}
		virtual void HandleNodeEvent(const char * path, const std::string & value){
			cout << "HandleNodeEvent:" << path << endl;	

		}


		virtual void HandleChildEvent(const char * path, const std::vector<std::string> & children) {
			cout << "HandleChildEvent:" << path << endl;	
		}

};

void InitDefaultLogger(){
	        default_init_logger("Mce","INFO");
}


BOOST_AUTO_TEST_CASE( test_zk){
	InitDefaultLogger();
	string zk = "st0.xce.d.xiaonei.com:2181";
	string loaderpath = "/searchrelation/loader";
	ZookeeperClient::Instance().SetZk(zk);
	ZkEventListenerPtr zl(new LoaderListerner());
	ZookeeperClient::Instance().AddChildListener(loaderpath.c_str(), zl);
	vector<std::string> list;
	ZookeeperClient::Instance().GetChildren(loaderpath.c_str(), &list);
	BOOST_FOREACH(const string & s, list){
		cout << "get child node:"	 << s << endl;
	}

	while(1){
		cout << " in loop" << endl;
		sleep(1000);	
	}

}


BOOST_AUTO_TEST_SUITE_END()


