

#define MCE_INFO(x) (cout << (x) << endl)
#define private public
#include "../src/client.hpp"
#undef private


#define BOOST_TEST_MODULE timeline_test 
#include <boost/test/auto_unit_test.hpp>

#define foreach BOOST_FOREACH
#define MCE_INFO(x) (cout << (x) << endl)

using namespace std;
using namespace boost;


BOOST_AUTO_TEST_SUITE(abc)

BOOST_AUTO_TEST_CASE( _simple_split_){
	string s="host1:port1";
	vector<string> res = SimpleSplit(s,":");
	BOOST_CHECK(res[0] == "host1");
	BOOST_CHECK(res[1] == "port1");
	s="host1:port1,host2:port2";
	res = SimpleSplit(s,",");
	BOOST_CHECK(res[0] == "host1:port1");
	BOOST_CHECK(res[1] == "host2:port2");
}

BOOST_AUTO_TEST_CASE( _parse_kafka_ ){
	string a="host1:port1,host2:port2";
	map<int,pair<string,string> > m = ParseKafkaAddress(a);
	BOOST_CHECK(m.size() == 2);
	BOOST_CHECK(m.count(0) == 1);
	BOOST_CHECK(m.count(1) == 1);
	BOOST_CHECK(m[0].first == "host1");
	BOOST_CHECK(m[0].second == "port1");
	BOOST_CHECK(m[1].first == "host2");
	BOOST_CHECK(m[1].second == "port2");

}
BOOST_AUTO_TEST_CASE( _test_init_client_)
{

	//boost::asio::io_service s;
	//boost::asio::io_service::work w(s);
	//boost::thread * bt = new boost::thread(boost::bind(&boost::asio::io_service::run, &s));
	string host = "10.2.72.187";
	string port = "9092";
	KafkaClient client(1,host,port);

	BOOST_CHECK(client.getHost() == host);
	BOOST_CHECK(client.getPort() == port);

	BOOST_CHECK(client.checkConnect());
	vector<string> ms;
	cout << "endl test" << endl;
	//ms.push_back("message1. from unitest");
	//ms.push_back("message2. from unitest");
	//client.send(ms, "test");

	//BOOST_CHECK(client.checkConnect() );
}
BOOST_AUTO_TEST_CASE( _test_get_client_ )
{
	//bool init(int kafkacnt_,int partitioncnt_,const string & serverlist)
	ClientManager cm ;
	string host0 = "10.2.72.187";
	string port0= "9092";
	string host1 = "10.2.72.187";
	string port1 = "9093";

	BOOST_CHECK(cm.init(2,2,host0+":"+port0+","+host1+":"+port1));	
	BOOST_CHECK(cm.getClientSize() == 2);
	BOOST_CHECK(cm.getClient(0) == cm.getClient(2));
	BOOST_CHECK(cm.getClient(1) == cm.getClient(3));
	BOOST_CHECK(cm.getClient(0) != cm.getClient(1));

	//BOOST_CHECK(cm.getClient(0)->getHost() == host0 );
	//BOOST_CHECK(cm.getClient(0)->getPort() == port0);
	BOOST_CHECK(cm.getClient(1)->getHost() == host1 );
	BOOST_CHECK(cm.getClient(1)->getPort() == port1);

	int uid = 1;
	KafkaClientPtr c = cm.getClient(uid);	
	BOOST_CHECK(c->checkConnect() );


	vector<string> ms;
	ms.push_back("message1. from unitest");
	ms.push_back("message2. from unitest");
	int p = uid % cm.getPartition();
	c->send(ms, "test",p);
}

 
BOOST_AUTO_TEST_SUITE_END()


