#include "FeedPreferGateI.h"

#include <cstring>
#include <boost/algorithm/string/replace.hpp>
#include "pugixml.hpp"
#include "Notify/util/headurl.h"
#include "Notify/util/json_escape.h"
#include "Notify/util/time_stat.h"
#include "NotifyDispatcherAdapter.h"
#include "FeedPreferCacheAdapter.h"
#include <boost/foreach.hpp>
#include <openssl/md5.h>
using namespace std;
using namespace xce::notify;
// using namespace xce::feed;
using namespace MyUtil;
using namespace xce::feed;
using namespace boost;

static const char SOURCE_SEPERATOR = '-';

static const int kMinPageUid = 600000000;
static const int kMaxPageUid = 700000000;

static const int STYPE_STATUS = 502;
static const int STYPE_PHOTO_ONE = 701;
static const int STYPE_PHOTO_MORE = 709;
// static const int STYPE_ALBUM = 602;


class PrintHelper {
public:
  template<typename T>
  static string Print(const T & container,int limit = 30) {
    ostringstream ios;

//    ios << "(";
    int idx = 0;
    typedef typename T::value_type vt;
    BOOST_FOREACH(const vt & v,container){
    	if(idx++ > 0) ios << ",";
    	ios << v;
      if(idx >= limit) break;
    }
//    ios << ")";
    return ios.str();
  }
};

class FeedPreferRequest: public Request {
public:
	FeedPreferRequest(FCGX_Request* r) :
    Request(r) {
  }
  virtual bool response();
};

class FeedRequestFactory: public RequestFactory {
public:
  virtual RequestPtr Create(FCGX_Request * r) {
    char * path = FCGX_GetParam("SCRIPT_NAME", r->envp);
    if (path) {
    	MCE_INFO("@@@ path:" << path);
      if (strcmp(path, "/feedprefer/get") == 0) {
        return new FeedPreferRequest(r);
      }
    }
    return NULL;
  }
};

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedPreferGateI::instance(), service.createIdentity("M", ""));


  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  string fcgi_socket = props->getPropertyWithDefault("Service."
      + service.getName() + ".FcgiSocket", "0.0.0.0:10400");//TODO
  MCE_DEBUG("Fcgi listens on : " << fcgi_socket);

  FcgiServer * fcgi_server = new FcgiServer(fcgi_socket, 64);
  fcgi_server->RegisterRequestFactory(new FeedRequestFactory());
  fcgi_server->Start();
}

FeedPreferGateI::FeedPreferGateI(){}

bool checkMd5(int uid, const string & key) {
	static const string kClubDispatchKey = "renrenclub$^%&^%&$%^&";

	ostringstream os;
	os << uid << kClubDispatchKey;
	string md5input = os.str() ;
	unsigned char* md5 = MD5((unsigned char*) md5input.c_str(), md5input.size(), NULL);

	stringstream res;
	for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		res << hex << setw(2) << setfill('0') << (unsigned int) md5[i];
	}
	MCE_INFO("checkMd5.input uid:"<< uid << " md5input:" << md5input <<",generate md5:" << res.str() << ",input key:" << key );
	return res.str() == key;
}

bool FeedPreferRequest::response() {
  if (!_fcgi_out) {
    MCE_WARN("get notify fcgi_out null");
    return false;
  }

//  if (!Request::response()) {
//    FCGX_FPrintF(_fcgi_out, "Content-type: text/html\r\n\r\nverify failure");
//    return false;
//  }

  string emptyRet = "Content-type: text/html\r\n\r\n{}";

  MCE_DEBUG("ticket verify ok");

  Ice::Int uid = getPropertyInt("uid");
  Ice::Int weekcnt = getPropertyInt("weekcnt");
  string key = getProperty("key");

  if(key.empty()){
		FCGX_FPrintF(_fcgi_out,
				"Content-type: text/html\r\n\r\n-6,key does not exist,");
		return false;
  }
  if(weekcnt != 1 and
  		weekcnt != 2 and
  		weekcnt != 4 and
  		weekcnt != 6 and
  		weekcnt != 8){
  	ostringstream os;
  	os << "Content-type: text/html\r\n\r\n-6,weekcnt:"<< weekcnt << " . should be 1 2 4 6 8.";
		FCGX_FPrintF(_fcgi_out,	os.str().c_str());
		return false;
  }
	if (!checkMd5(uid, key)) {
		FCGX_FPrintF(_fcgi_out,
				"Content-type: text/html\r\n\r\n-6,MD5 not match,");
		return false;
	}

  MCE_INFO("FeedGetRequest::response. uid:" <<uid << " weeks:" << weekcnt);
  stringstream ss;
  ss << "Content-type: text/html; charset=utf-8\r\n\r\n{";
  vector<int> friends;
  try{
  	friends = FeedPreferCacheAdapter::instance().Get(uid,weekcnt);
  }catch (Ice::Exception& e) {
		MCE_WARN("FeedPreferRequest::response. uid:" << uid << " weekcnt:" << weekcnt << " key:" << key << " exception:" << e );
		string out = "Content-type: text/html\r\n\r\n-3,call notify fail";
		out += e.what();
		FCGX_PutS(out.c_str(), _fcgi_out);
		return false;
	}
  ss << "\"host_id\":" << uid << ",\"friends\":\"" << PrintHelper::Print(friends)<<"\"}";
  FCGX_FPrintF(_fcgi_out, "%s", ss.str().c_str());

  return true;
}
