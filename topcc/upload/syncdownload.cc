#include "syncdownload.h"
#include "url_parse.h"

namespace net {

SyncDownload::SyncDownload() {
}

bool MakeGetStream(const std::string &host, const std::string &path, boost::asio::streambuf *request) {
  if (!request)
    return false;
  std::ostream request_stream(request);
  request_stream << "GET " << path << " HTTP/1.0\r\n";  
  request_stream << "Host: " << host << "\r\n";  
  request_stream << "Accept: */*\r\n";  
  request_stream << "Connection: close\r\n\r\n";
  return true;
}

int SyncDownload::Get(const std::string &url, std::string *data, std::string *head) {
  tcp::resolver resolver(ios_);
  std::string host, path;
  int code = 200;
  unsigned int status_code;
  int port;
  bool b = arch::ParseUrlSimple(url, &host, &port, &path);
  if (!b)
    return 1;
  char strport[20];
  sprintf(strport, "%d", port);
  tcp::resolver::query query(host, strport);
  
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  tcp::resolver::iterator end;
  boost::system::error_code error = boost::asio::error::host_not_found;
  boost::asio::streambuf request;
  MakeGetStream(host, path, &request);
  while (error && endpoint_iterator != end) {
    if (data)
      data->clear();
    if (head)
      head->clear();
    tcp::socket socket(ios_);
    socket.connect(*endpoint_iterator++, error);
    boost::asio::write(socket, request);
    boost::asio::streambuf recv;

    boost::asio::read_until(socket, recv, "\r\n", error); // first line, state 200 http
    if (error) continue;
 
    const char* debug = boost::asio::buffer_cast<const char*>(recv.data()); // TODO:test
    std::istream response_stream(&recv);
    std::string http_version;
    response_stream >> http_version;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
      code = 1;
      continue;
    }
    if (status_code != 200) {
      code = status_code;
      continue;
    }
    //读取http头  
    boost::asio::read_until(socket, recv, "\r\n\r\n", error);
    if (error) continue;
    debug = boost::asio::buffer_cast<const char*>(recv.data()); // TODO:test
    // Process the response headers.
    std::string header;
    bool chunk = false;
    int leftnumb = 0;
    while (std::getline(response_stream, header) && header != "\r") {
      if (head) {
        head->append(header);
        head->append("\n");
      }
      if (header == "Transfer-Encoding: chunked\r") {
        chunk= true;
      } else if (header.find("Content-Length:") == 0) {
        std::string numstr = header.substr(16);
        leftnumb = atoi(numstr.c_str());
      } /*else if (header == "Content-Encoding:gzip\r") { }*/
    }

    if (!leftnumb) {
      if (chunk == true) {
        boost::asio::read_until(socket, recv, "0\r\n\r\n", error); // TODO: 这里有问题
        if (error) continue;
        std::string tmp;
        if (!std::getline(response_stream, tmp)) // filter  chunked number
          continue;
        while (std::getline(response_stream, tmp) && tmp != "\r") {
          if (tmp == "0\r") // filter chunked number
            continue;
          //DLOG(INFO) << tmp; // TODO: test
          if (tmp[tmp.size()-1] == '\r')
            tmp.erase(tmp.size()-1);
          if (data)
           data->append(tmp);
        }
      }
    } else {
      int left = recv.size();
      if (left < leftnumb) {
        boost::asio::read(socket, recv, boost::asio::transfer_at_least(leftnumb - left), error);
        if (error) continue;
      }
        
      const char* buf = boost::asio::buffer_cast<const char*>(recv.data());
      if (data)
        data->assign(buf, leftnumb);
      //size_t size = it->second->recv.size();
      recv.consume(leftnumb);
    }

    if (error != boost::asio::error::eof)
      break;
    socket.close();
  }

  return code;
}

} //namespace
