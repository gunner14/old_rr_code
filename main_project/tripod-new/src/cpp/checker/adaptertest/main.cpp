#include <OceCxxAdapter/src/CheckerManagerAdapter.h>
#include <iostream>

using namespace std;
using namespace MyUtil;
using namespace xce::adapter::checkermanager;
using namespace com::renren::tripod;

int main(int argc, char ** argv) {
  /* process arguments */
  string businessId = "";
  string namespaceId = "";
  string zkAddress = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/Tripod";
  char c;
  while (-1 != (c = getopt(argc, argv,
          "b:"  /* businessId */
          "n:"  /* namespaceId */
          "z:"  /* zkAddress */
          ))) {
    switch (c) {
      case 'b':
        businessId = optarg;
        break;
      case 'n':
        namespaceId = optarg;
        break;
      case 'z':
        zkAddress = optarg;
        break;
      default:
        break;
    }
  }
  if(businessId == "" || namespaceId == "") {
    cout << "useage: " << endl;
    cout << argv[0] << " -b businessId -n namespaceId [-z zkAddress]" << endl;
    cout << "example: " << endl;
    cout << argv[0] << " -b UserPassport -n test -z xcetest3:2181/Tripod" << endl;
    return -1;
  }
  MCE_DEFAULT_INIT("WARN");
  CheckerManagerAdapter::instance().restart(businessId, namespaceId, zkAddress);
  MCE_WARN("restart businessId=" << businessId << ", businessId=" << namespaceId << ", zkAddress=" << zkAddress);
  return 0;
}
