#include "FeedRedisClient.h"
#include <sstream>
#include <iostream>
#include <vector>

namespace xce {
namespace feed {

const std::string RedisClient::kEmptyString = "";

RedisBinArgv::RedisBinArgv(std::string key):key_(key), count_(2) {
  memset(argv_, 0, kWriteBatchSize + 2);
  memset(argvlen_, 0, kWriteBatchSize + 2);
  argv_[1] = key_.c_str();
  argvlen_[1] = key_.size();
}

bool RedisBinArgv::PushArg(const char *value, size_t len) {
  if(count_ >= kWriteBatchSize + 1) {
    return false;
  }

  argv_[count_] = value;
  argvlen_[count_++] = len;
  return true;

}

void RedisBinArgv::SetCommand(std::string cmd) {
   argv_[0] = cmd.c_str(); 
   argvlen_[0] = cmd.size(); 
}

RedisBinReply::RedisBinReply(std::string key):key_(key), count_(0) { 
  memset(replyv_, 0, kReadBatchSize);
  memset(replyvlen_, 0, kReadBatchSize);
}

bool RedisBinReply::PushReply(const char *reply, size_t len) {
  if(count_ >= kReadBatchSize - 1) {
    return false;
  }

  replyv_[count_] = reply;
  replyvlen_[count_++] = len;
  return true;
}

//------------------------------------------------------------------
bool RedisClient::CheckErr(const redisReply* reply,const redisContext* rc, std::string log) {
  if(reply == NULL) {
#ifdef STDOUT 
    std::cout << log << " reply is NULL!" << " err:" << rc->err << " err_str:" << rc->errstr << std::endl;
#else
    MCE_WARN(log << " reply is NULL!" << " err:" << rc->err << " err_str:" << rc->errstr);
#endif
    return true;
  }

  if(reply->type == REDIS_REPLY_ERROR) {
#ifdef STDOUT 
    std::cout << log << " reply->type:" << reply->type << " reply->str:" << reply->str << " err:" << rc->err << " err_str:" << rc->errstr << std::endl;
#else
    MCE_WARN(log << " reply->type:" << reply->type << " reply->str:" << reply->str << " err:" << rc->err << " err_str:" << rc->errstr);
#endif
    return true;
  } 

  return false;

}

RedisClient::RedisClient(const std::string& address, const int port, const long timeout): 
		address_(address), port_(port), timeout_(timeout), rc_(NULL), valid_(false) {

	struct timeval tv;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = 0;

	std::ostringstream oss;
	oss << "RedisClient::RedisClient --> connection. address_:" << address_ << " port_:" << port_;


  {
		TimeLog tl(oss.str());
		rc_ = redisConnectWithTimeout(address_.c_str(), port_, tv);
	}

	if (!rc_) {
#ifdef STDOUT 
    std::cout << "RedisClient::RedisClient --> The connection to the redis server failed! rc is NULL! address_:" << address_ << " port_:" << port_ << std::endl;
#else
		MCE_WARN("RedisClient::RedisClient --> The connection to the redis server failed! rc is NULL! address_:" << address_ << " port_:" << port_);
#endif
    
	  //	SetValid(false);
		return;
	}

	if (rc_->err) {
	  //	SetValid(false);
#ifdef STDOUT 
    std::cout << "RedisClient::RedisClient --> The connection to the redis server failed! reason:" << rc_->errstr << " address_:" << address_ << " port_:" << port << " client fd:" << rc_->fd << std::endl;
#else
		MCE_WARN("RedisClient::RedisClient --> The connection to the redis server failed! reason:" << rc_->errstr << " address_:" << address_ << " port_:" << port << " client fd:" << rc_->fd);
#endif
	} else {
		SetValid(true);
	}

//  struct timeval timeout_value = { 1, 500000 };
//  int res = redisSetTimeout(rc_,  timeout_value);
//  std::cout << " time set:" << res << std::endl;
}

RedisClient::~RedisClient() {
#ifdef STDOUT 
  std::cout << "RedisClient::~RedisClient() valid:" << std::boolalpha << valid_ << " address_:" << address_ << " port_:" << port_ << std::endl;
#else
	MCE_INFO("RedisClient::~RedisClient() valid:" << std::boolalpha << valid_ << " address_:" << address_ << " port_:" << port_);
#endif
	if (rc_) {
		redisFree(rc_);
	}
}

bool RedisClient::Ping() {
	static const std::string command = "PING";
#ifdef STDOUT 
  std::cout << "RedisClient::Ping --> address_:" << address_ << " port_:" << port_ << std::endl;
#else
	MCE_INFO("RedisClient::Ping --> address_:" << address_ << " port_:" << port_);                                              
#endif
	redisReply* reply = NULL;

	std::ostringstream oss;
	oss << "RedisClient::ping --> address_:" << address_ << " port_:" << port_;

	{
		TimeLog tl(oss.str());
		reply = (redisReply*) redisCommand(rc_, command.c_str());                                                                                                 
	}

  if(CheckErr(reply, rc_, oss.str())) {
		SetValid(false);

    if(reply) {
      freeReplyObject(reply);
    }

		return false;
  }


  // reply->type should be REDIS_REPLY_STATUS here. 
#ifdef STDOUT 
  std::cout << "RedisClient::Ping --> reply->type:" << reply->type << " reply->str:" << reply->str
        << " reply->len:" << reply->len << " address_:" << address_ << " port_:" << port_ << std::endl;
#else
	MCE_INFO("RedisClient::Ping --> reply->type:" << reply->type << " reply->str:" << reply->str 
			<< " reply->len:" << reply->len << " address_:" << address_ << " port_:" << port_); 
#endif

	freeReplyObject(reply);
	return true;                        
}

bool RedisClient::Push2List(RedisBinArgv& cmd_argv) {

  static const std::string kPushListCmd = "RPUSH";
  cmd_argv.SetCommand(kPushListCmd);

	std::ostringstream oss;
	oss << "RedisClient::Push2List --> address_:" << address_ << " port_:" << port_ << " cmd_argv.key:" << cmd_argv.key() << " cmd_argv.count:" << cmd_argv.count(); 
#ifdef STDOUT 
  std::cout << oss.str() << std::endl;
#else
  MCE_INFO(oss.str());                                              
#endif

	redisReply* reply = NULL;

	{
		TimeLog tl(oss.str());
		reply = (redisReply*) redisCommandArgv(rc_, cmd_argv.count(), cmd_argv.argv(), cmd_argv.argvlen());
	}

  if(CheckErr(reply, rc_, oss.str())) {
    SetValid(false);
    if(reply) {
      freeReplyObject(reply);
    }
    return false;
  }


  if(reply->integer < cmd_argv.count() - 2) {

#ifdef STDOUT 
    std::cout << oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer << " cmd_argv count:" << cmd_argv.count() << std::endl;
#else
    MCE_WARN(oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer << " cmd_argv count:" << cmd_argv.count());
#endif
  }

//  if(reply->integer > kMaxListSize) {
//
//#ifdef STDOUT 
//    std::cout << oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer << " cmd_argv count:" << cmd_argv.count() << std::endl;
//#else
//    MCE_INFO(oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer << " cmd_argv count:" << cmd_argv.count());
//#endif
//  }

#ifdef STDOUT 
  std::cout << oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer<< std::endl;
#else
  MCE_INFO(oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer);
#endif

  freeReplyObject(reply);

  return true;
}



//bool RedisClient::GetList(const std::string& key, RedisBinReply* reply_list, int begin = 0, int limit = 1) {

bool RedisClient::Del(const std::string& key) {

	std::ostringstream oss;
	oss << "RedisClient::Del --> address_:" << address_ << " port_:" << port_ << " key:" << key; 
#ifdef STDOUT 
  std::cout << oss.str() << std::endl;
#else
  MCE_INFO(oss.str());                                              
#endif

	redisReply* reply = NULL;

  std::ostringstream cmd; 
  cmd << "DEL " << key; 
  std::string command = cmd.str(); 

	{
		TimeLog tl(oss.str());
    reply = (redisReply*) redisCommand(rc_, command.c_str());
	}

  if(CheckErr(reply, rc_, oss.str())) {
    SetValid(false);
    if(reply) {
      freeReplyObject(reply);
    }
    return false;
  }


//  if(reply->integer != 1) {
//
//#ifdef STDOUT 
//    std::cout << oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer << " key:" << key << std::endl;
//#else
//    MCE_WARN(oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer << " key:" << key);
//#endif
//  }

#ifdef STDOUT 
  std::cout << oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer<< std::endl;
#else
  MCE_INFO(oss.str() << " reply->type:" << reply->type << " reply->integer:" << reply->integer);
#endif

  freeReplyObject(reply);

  return true;
}



}
}

struct cls {
  int a;
  long b;
};

class CT {
  public:
   char &  operator[](std::size_t pos) const {
    //return pt[pos];
    //*pt ='a';
   // char * tmp = pt;
    return *(pt);
    //return tmp;
  }
  private:
  //char  pt[100];
  char * pt;
};

//int main() {
//  
//
//
////  cls ob1;
////  ob1.a = 1;
////  ob1.b = 2;
////
////  cls ob2;
////  ob2.a = 3;
////  ob2.b = 4;
//
//  
//  xce::feed::RedisBinArgv arg("list20");
////  void * ptr1 = &ob1;
////  void * ptr2 = &ob2;
////  arg.PushArg(static_cast<const char*>(ptr1), sizeof(ob1));
////  arg.PushArg(static_cast<const char*>(ptr2), sizeof(ob2));
//  arg.PushArg("0", 1);
//  arg.PushArg("012345", 6);
//  xce::feed::RedisClient cl("10.3.22.141", 6379); 
////  cl.Ping();
//  cl.Push2List(arg);
// // std::vector<cls> vec; 
// // cl.GetList<cls>("list20",vec, 0,5);
////  std::vector<std::string> vec; 
////  cl.GetList<std::string>("list18",vec, 0,5);
////  for( std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); it++) {
////     std::cout << " str:" << *it << " len:" << it->size() << std::endl;
////     //std::cout << " a:" << it->a << " b:" << it->b << " " << sizeof(*it) << std::endl;
////  }
//


//}

