#include <UserLogic.h>
#include <ScoreCacheAdapter.h>
#include <ScoreCache.h>
#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <string>

using namespace std;

void help() {
 cout << "usage:" <<endl;
 cout << "1)./ValidTool M@ScoreCacheN0 isvalid" << endl;
 cout << "2)./ValidTool M@ScoreCacheN0 setvalid true" << endl;
 cout << "3)./ValidTool M@ScoreCacheN0 getscore 200308413(uid)" << endl;
 cout << "notice:only for ScoreNew cluster." << endl;
}
int main(int argc, char ** argv) {
  if(argc < 3) {
    help();
    return 1;
  }
  string proxy(argv[1]);
  string op(argv[2]);

  int status = 0;
  Ice::CommunicatorPtr ic;
  Ice::ObjectPrx base;

  try {
    Ice::PropertiesPtr properties = Ice::createProperties();
    properties->setProperty("Ice.Override.ConnectTimeout", "300");
    properties->setProperty("IceGrid.InstanceName", "ScoreNew");
    properties->setProperty("Ice.ThreadPool.Client.StackSize", "65535");
    properties->setProperty("Ice.ThreadPool.Server.StackSize", "65535");
    properties->setProperty("Ice.MessageSizeMax", "20480");
    properties->setProperty("Ice.Default.Host", "127.0.0.1");

    properties->setProperty("Ice.Default.Locator", "ScoreNew/Locator:tcp -h ScoreNewMaster -p 4070:tcp -h ScoreNewReplica1 -p 4070:tcp -h ScoreNewReplica2 -p 4070");
    Ice::InitializationData id;
    id.properties = properties;
    ic = Ice::initialize(id);
    Ice::ObjectPrx prx = ic->stringToProxy(proxy)->ice_timeout(300);
    xce::scorecache::ScoreCacheNManagerPrx prx2 = xce::scorecache::ScoreCacheNManagerPrx::uncheckedCast(prx);
    if (op == "isvalid") {
      cout << prx2->isValid() << endl;
    } else if (op == "setvalid") {
      if(argc < 4) {
        help();
        return 1;
      }
      int value = atoi(argv[3]);
      prx2->setValid(value);
      cout << "set " << prx2 << " value:" << value << endl;
    } else if (op == "getscore") {
      if(argc < 4) {
        help();
        return 1;
      }
      int id = atoi(argv[3]);
      xce::scorecache::ScoreDataN data = prx2->getScoreDataN(id);
      cout << "continueDay:" << data.continueLoginDays << ", score:" << data.score << ", level:" << data.level << ", vip:" << data.vip  << ", historyDay:" << data.historyLoginDays << ", lastTime:" << data.lastLoginTime << endl;
    }
  } catch (const Ice::Exception & ex) {
    cerr << ex << endl;
    status = 1;
  } catch (const char * msg) {
    cerr << msg << endl;
    status = 1;
  }
  if (ic) {
    try {
      ic->destroy();
    } catch (const Ice::Exception & ex) {
      cerr << ex << endl;
      status = 1;
    }
  }
  return status;
}
