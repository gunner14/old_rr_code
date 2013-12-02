#include <iostream>

#include <Ice/Ice.h>
#include "HelloXoa.h"
#include "xoa2/client/xoa_registry_client.h"

using namespace std;
using namespace xoa2;

int main() {
  Ice::CommunicatorPtr ic;
  Ice::StringSeq args;
  Ice::InitializationData initData;

  xoa2::XoaRegistryClient registry_client_("10.2.78.62:2181/xce");
  vector<string> children;
  registry_client_.GetChildren("/", &children);

  try {
    // Create a communicator
    ic = Ice::initialize(args, initData);
    // Create a proxy for the root directory
    for (size_t i = 0; i < children.size(); ++i) {
      string sprx("M:");
      sprx += children[i];
      HelloXoaPrx twoway = HelloXoaPrx::checkedCast(ic->stringToProxy(sprx)->ice_twoway()->ice_timeout(1000)->ice_secure(false));
      cout << twoway->SayHello(sprx) << endl;

      // HelloXoaPrx datagram = twoway->ice_datagram();
      // cout << datagram->SayHello(sprx) << endl;
    }
  } catch (const Ice::Exception& ex) {
    cerr << ex << endl;
  } catch (const char* msg) {
    cerr << msg << endl;
  }

  if (ic) {
    ic->destroy();
  }
  return 0;
}
