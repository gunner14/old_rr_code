#include "feed/base_feed/baseasync.h"
#include "feed/base_feed/logging.h"

namespace xce { namespace detail {

boost::thread* Async::thread_ = 0;
boost::asio::io_service::work* Async::work_ = 0;

void Async::Start() {
  if (thread_)
    return;

  GetService().reset();
  work_ =
    new boost::asio::io_service::work(GetService());

  thread_ = new boost::thread(boost::bind(
    &boost::asio::io_service::run, &GetService()
    ));
}

void Async::Stop() {
  delete work_;
  work_ = 0;
  GetService().stop();

  thread_->join();
  delete thread_;
  thread_ = 0;
}

} } // namespace xce::detail
