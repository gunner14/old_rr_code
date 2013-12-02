#include "service_arch.h"
#include "service_base_impl.h"

#include "twitter_types.h"
#include "TwitterService.h"
#include "service_config.h"

#include <stdio.h>
#include <iostream>
#include <getopt.h>
using namespace xoa2::demo::twitter;
using namespace xoa2::base;
using namespace xoa2::service;
using namespace std;
using namespace renren::xoa2::base::statistics;
class TwitterServiceHandler : public TwitterServiceIf, public ServiceBaseImpl{
  int count_;
  std::string name_;
  std::string version_;
public:
  TwitterServiceHandler(std::string name, std::string version) : ServiceBaseImpl(name, version),
    count_(0), name_(name), version_(version) {
    printf("TwitterServiceHandler created.\r\n");
  }


  virtual void ping(const int32_t timeout) {
    usleep(timeout*1000); 
    printf("ping called.\r\n");
  }

  virtual bool postTweet(const Tweet& tweet) {
    //if (++count_ % 10000 == 0) {
      printf("postTweet called %d\r\n", count_);
    //}
    return true;
  }

  virtual void searchTweets(TweetSearchResult& _return, const std::string& query) {
    printf("searchTweets called.\r\n");
  }

  virtual void zip() {
    printf("zip called.\r\n");
  }

};

 void print_usage() {  
   printf("start xoa2 server \n");  
   printf("Usage: -r <registry_config_path> -x <xoa.xml_path> -s <true>\n");  
   printf("    -r registry configure file path\n");  
   printf("    -x xoa.xml file path\n");  
   printf("    -s skip the xcs registry\n");  
   printf("    -m militia configure file path\n");  
   printf("    -t service start with test phase\n");  
 }  
 int main(int argc, char **argv) {
   print_usage();
   MCE_INIT("./serverlib.log", "DEBUG");

   std::string config;
   std::string xoa_path;
    
   bool registry = true;
   int opt;
   struct option longopts[] = {
     {"registry",required_argument, NULL, 'r'},
     {"xoa", required_argument, NULL, 'x'},
     {"skip", no_argument, NULL,'s'},
     {"militia", required_argument, NULL,'m'},
     {"test", no_argument, NULL,'t'},
     {0,0,0,0},
   };
   while((opt = getopt_long(argc, argv, "r:x:sm:t", longopts, NULL)) != -1){
     switch(opt){
     case 'r':
       printf("registry config path is: %s\n",optarg);
       config = optarg;
       break;
     case 'x':
       printf("xoa.xml path is: %s\n",optarg);
       xoa_path = optarg;
       break;
     case 's':
       printf("service start with skip registry !");
       registry = false;
       break;
     case 'm':
       printf("militia path is %s\n", optarg);
       g_Militia = optarg; 
       break;
     case 't':
       printf("serivce start with test phase!");
       g_TestPhase = true;
       break;
     default:
      print_usage();
      break; 
     }
       
   }
   if(config == "") {
     config = "./conf/service_registry_demo.conf";
   } else {
     if(access(config.c_str(), R_OK) != 0) {
       printf("config path error! start server fail!\n");
       MCE_ERROR("config path error! start server fail!");
       return 0;
     } 
   }
   if(xoa_path == "") {
     xoa_path = "./conf/xoa.xml";
   } else {
     if(access(xoa_path.c_str(), R_OK) != 0) {
       printf("xoa.xml path error! start server fail!\n");
       MCE_ERROR("xoa.xml path error! start server fail!");
       return 0;
     }
   }

   if(g_Militia == "") {
     g_Militia = xoa_path;
   } else {
     if(access(g_Militia.c_str(), R_OK) != 0) {
       printf("militia path error! start server fail!\n");
       MCE_ERROR("militia path error! start server fail!");
       return 0;
     }
   }
   ThriftService<TwitterServiceHandler, TwitterServiceProcessor> service;
   service.Start(config, xoa_path, registry);
   return 0;
 }

//extern "C" {

//void ServiceEntry() {
//  int port = 9091;
//  ThriftService<TwitterServiceHandler, TwitterServiceProcessor> service;
//  service.Start(port);
//}

//}

