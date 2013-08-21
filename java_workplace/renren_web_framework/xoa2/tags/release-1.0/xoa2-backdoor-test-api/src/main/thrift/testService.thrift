namespace cpp renren.xoa2.backdoor.test
namespace java com.renren.xoa2.backdoor.test

include "renrenbackdoor.thrift"

service TestService extends renrenbackdoor.RenrenService {
  void ping(1: i32 timeout);
}

