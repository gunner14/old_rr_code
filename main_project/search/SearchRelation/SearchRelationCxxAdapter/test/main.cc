

#define MCE_INFO(x) (cout << (x) << endl)
#define private public
//#include "../src/LoaderPool.h"
#include "../src/LoaderAdapter.h"
#include "IceExt/src/IceLogger.h"
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>

//#include "../src/zk_event_listener.h"
//#include "/data/home/jingwei.ye/main_project/trunk/search/SearchZkClientV2/src/zk_client.h"
//#include "/data/home/jingwei.ye/main_project/trunk/search/SearchZkClientV2/src/zk_event_listener.h"
#undef private


#define BOOST_TEST_MODULE timeline_test 
#include <boost/test/auto_unit_test.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH
//#define MCE_INFO(x) (cout << (x) << endl)

using namespace std;
using namespace boost;
using namespace search::relation;
using namespace MyUtil;


BOOST_AUTO_TEST_SUITE(abc)

void InitLogger(){
	string path = "./yjwtmp/";
	mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	Ice::LoggerPtr logger =  new IceLogger(path + "/oce_log", "INFO");;
}
void InitDefaultLogger(){
	default_init_logger("Mce","INFO");
}

BOOST_AUTO_TEST_CASE( test_zk){
//	InitLogger();
	InitDefaultLogger();
	MCE_INFO("yahoo");	
	string nodestr = "10.3.18.75_9012_SearchRelationLoader2";
	RelationLoaderAdapter adapter; 
	string zk = "st0.xce.d.xiaonei.com:2181";
	string loaderpath = "/searchrelation/loader";

	adapter.Init(zk,loaderpath);

	int uid = 224730548;
	while(1){
		map<int,map<int,short> > res ; 
		try{
			res = adapter.GetRelationV2(uid,3,"CxxAdapterTest");
		}catch(std::exception& e){
			MCE_WARN("test zk. error" << e.what());
		}
		MCE_INFO( "res size:" << res.size() );
		typedef map<int,map<int,short> >::value_type vt;
		BOOST_FOREACH(const vt& p, res){
			MCE_INFO("degree:" << p.first << " count:" << p.second.size() );
		}
		sleep(1);
	}
}


BOOST_AUTO_TEST_SUITE_END()


