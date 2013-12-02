#include <iostream>

#include <Ice/Ice.h>
#include "HelloXoa.h"
#include "IceExt/src/XoaServiceAdapterI.h"

using namespace std;
using namespace xoa2;
using namespace MyUtil;

int main() {
  XoaServiceAdapterI<HelloXoaPrx> * adapter = XoaServiceAdapterI<HelloXoaPrx>::instance("xxx");
  try {
    HelloXoaPrx proxy = adapter->GetProxy(0);
    if (proxy) {
      string s = proxy->SayHello("hehehhe");
      cout << s << endl;
    } else {
      cerr << "null proxy" << endl;
    }
  } catch (const Ice::Exception& ex) {
    cerr << "ice ex : "<< ex << endl;
  } catch (const char* msg) {
    cerr << "---" << msg << endl;
  }
  return 0;
}
