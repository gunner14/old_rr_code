#include "HelloXoaI.h"

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&xoa2::HelloXoaI::instance(), service.createIdentity("M", ""));
}

namespace xoa2 {

HelloXoaI::HelloXoaI() : counter_(0) {
}

std::string HelloXoaI::SayHello(const std::string& name, const ::Ice::Current&) {
  ofstream err("/dev/stderr");
  err << "reporter:counter:HelloXoa,SayHello," << ++counter_ << endl;
  err << "reporter:status:HelloXoa-SayHello-" << counter_ << endl;

  cerr << "x-cerr-HelloXoa-SayHello-" << counter_ << endl;

  std::string res("hello, ");
  return res + name;
}

}
