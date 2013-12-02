#include <UserLogic.h>
#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <string>

using namespace std;

int main(int argc, char ** argv) {
  string proxy(argv[1]);
  int value = atoi(argv[2]);

  int status = 0;
  Ice::CommunicatorPtr ic;
  Ice::ObjectPrx base;

  try {
    Ice::PropertiesPtr properties = Ice::createProperties();
    properties->setProperty("Ice.Override.ConnectTimeout", "300");
    properties->setProperty("IceGrid.InstanceName", "Buddy");
    properties->setProperty("Ice.ThreadPool.Client.StackSize", "65535");
    properties->setProperty("Ice.ThreadPool.Server.StackSize", "65535");
    properties->setProperty("Ice.MessageSizeMax", "20480");
    properties->setProperty("Ice.Default.Host", "127.0.0.1");

    properties->setProperty("Ice.Default.Locator", "XiaoNei/Locator:tcp -h XiaoNeiRegistry -p 12000");
    Ice::InitializationData id;
    id.properties = properties;
    ic = Ice::initialize(id);
    Ice::ObjectPrx prx = ic->stringToProxy(proxy)->ice_timeout(300);
    mop::hi::svc::model::UserLogicManagerPrx::uncheckedCast(prx)->setValid(value);
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
