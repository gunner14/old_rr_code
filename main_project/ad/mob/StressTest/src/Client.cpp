/***************************************************************/
/*   相关配置注意修改client.config                             */
/*                                                             */
/***************************************************************/

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include "AdMob.h"
#include "threadpool.h"

using namespace xce::ad;
using namespace std;

typedef  AdMobPrx ClientPrx;
/***************************增加服务参数***************************************/
class ClientPara{
public:
  ClientPara(){
	initMobReqPara();
  }
public:
  void initMobReqPara(){
	para.uid = 221416299;
    para.requestTime = 20120427101010;
	para.appid = "198733";
    para.sid = "11111";
	para.adzoneId = 100000000094;
    para.client.screenSize="322*150";
	para.lastGroupIds.push_back(10000505720001); 
  }
public:
	AdMobReqPara para;
};

static ClientPara clientPara;

void  DoGetAds(void* threadpara){
	AdMobPrx* adMobPrx = (AdMobPrx*)threadpara;
	AdMobResPara res = (*adMobPrx)->GetAds(clientPara.para);
}

class StatFunc {  
  string name_; 
  timeval start_, end_;  //static int count;
public:  
  StatFunc(string name) : name_(name) {
	  gettimeofday(&start_, NULL); 
  } 
  
  ~StatFunc(){
	gettimeofday(&end_, NULL);
	float timeuse = 1000000*(end_.tv_sec-start_.tv_sec)+end_.tv_usec-start_.tv_usec; 
	timeuse/=1000;  
	if(timeuse >= 20){ 
		cout << name_ << " time usage:" << timeuse << " ms" << endl;   
	} else {
		cout << name_ << " time usage:" << timeuse << " ms" << endl;
	} 
  }
};




class AdClient : public Ice::Application{
public:
  AdClient();
  virtual int run(int, char*[]);
};

AdClient::AdClient() : Ice::Application(Ice::NoSignalHandling){
}

int AdClient::run(int argc, char* argv[]){
  ClientPrx admob;
  int  poolsize = 5;
  int  taskcount = 100;
  string clientname="";
  try{
	poolsize = communicator()->getProperties()->getPropertyAsIntWithDefault("PoolSize", 5);
	taskcount = communicator()->getProperties()->getPropertyAsIntWithDefault("TaskCount", 100);
	clientname = communicator()->getProperties()->getPropertyWithDefault("ClientName", "M@AdMobEngine0");
	admob = ClientPrx::checkedCast(communicator()->stringToProxy(clientname));
  }catch(const Ice::NotRegisteredException&){
	  cerr << "couldn't find a proxy" << clientname << endl;
  }

  if(!admob){
	cerr << clientname << "couldn't find a proxy" << endl;
	return EXIT_FAILURE;
  }
  
  threadpool tp = create_threadpool(poolsize);
  {
	StatFunc func(clientname);
	for(int i = 0; i < taskcount; i ++) {   
	dispatch(tp, DoGetAds, &admob);
	}
	destroy_threadpool(tp);
  }
  

  
  cout << "--------------"<< endl;
  return 0;
}

int main(int argc, char* argv[]){
    AdClient app;
    return app.main(argc, argv, "config.client");
}
