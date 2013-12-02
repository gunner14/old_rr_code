#include "service_arch/service_arch.h"
#include "service_arch/service_base_impl.h"

#include "TwitterService_types.h"
#include "TwitterService.h"

#include <stdio.h>

using namespace xoa2::demo::twitter;

class TwitterServiceHandler : public TwitterServiceIf, public ServiceBaseImpl {
  int count_;
 public:
  TwitterServiceHandler() : count_(0) {
    printf("TwitterServiceHandler created.\r\n");
  }


  virtual void ping(const int32_t timeout) {
    printf("ping called.\r\n");
  }

  virtual bool postTweet(const Tweet& tweet) {
    if (++count_ % 10000 == 0) {
      printf("postTweet called %d\r\n", count_);
    }
    return true;
  }

  virtual void searchTweets(TweetSearchResult& _return, const std::string& query) {
    printf("searchTweets called.\r\n");
  }

  virtual void zip() {
    printf("zip called.\r\n");
  }
};

// int main(int argc, char **argv) {
//   int port = 9091;
//   cout << "listens on port " << port << endl;

//   ThriftService<TwitterServiceHandler, TwitterServiceProcessor> service;
//   service.Start(port);
//   return 0;
// }

extern "C" {

void ServiceEntry() {
  int port = 9091;
  ThriftService<TwitterServiceHandler, TwitterServiceProcessor> service;
  service.Start(port);
}

}

