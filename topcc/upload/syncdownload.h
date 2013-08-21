#ifndef _SYNC_DOWNLOAD_H_
#define _SYNC_DOWNLOAD_H_

#include <boost/asio.hpp>

namespace net {
using boost::asio::ip::tcp;

class SyncDownload {
 public:
  SyncDownload();
  int Get(const std::string &url, std::string *data, std::string *head);
  int Post(const std::string &url, const std::string &data);
 private:
  boost::asio::io_service ios_;
};

}
#endif
