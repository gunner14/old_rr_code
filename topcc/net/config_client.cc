#include "config_client.h"
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace net {
void watcher(zhandle_t *zzh, int type, int state, const char *path, void* context) {
   ConfigClient* cc = (ConfigClient*)context;
   clientid_t *myid = cc->myid_.get();
   if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            const clientid_t *id = zoo_client_id(zzh);
            if (myid->client_id == 0 || myid->client_id != id->client_id) {
                *myid = *id;
            }
        } else if (state == ZOO_AUTH_FAILED_STATE) {
            zookeeper_close(zzh);
            cc->zh_.reset();
        } else if (state == ZOO_EXPIRED_SESSION_STATE) {
            zookeeper_close(zzh);
            cc->zh_.reset();
        }
    } 
}
 
ConfigClient::ConfigClient(const std::string &addr, const std::string &path)
: myid_(new clientid_t) {
  path_ = "/xoa/";
  path_ += path;

  myid_->client_id = 0;
  zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
  zoo_deterministic_conn_order(1); // enable deterministic order
  zh_.reset(zookeeper_init(addr.c_str(), watcher, 3000, myid_.get(), this, 0));
}

bool ConfigClient::GetIpGroup(std::vector<std::pair<std::string, int> > *v) {
  if (!v )
    return false;
  sleep(1); 
  char buf[1024];
  int sz = sizeof(buf) - 1;
  struct Stat stat;
  struct timeval tv;
  int df, interest;
  int rc;
/*
  zookeeper_interest(zh, &fd, &interest, &tv);
  if (fd != -1) {
    if (interest&ZOOKEEPER_READ) {
      FD_SET(fd, &rfds);
    } else {
      FD_CLR(fd, &rfds);
    }   
    if (interest&ZOOKEEPER_WRITE) {
      FD_SET(fd, &wfds);
    } else {
      FD_CLR(fd, &wfds);
    }
  } else {
    fd = 0;
    return false;
  }

  rc = select(fd+1, &rfds, &wfds, &efds, &tv);
  events = 0;
  if (FD_ISSET(fd, &rfds)) {
    events |= ZOOKEEPER_READ;
  }   
  if (FD_ISSET(fd, &wfds)) {
    events |= ZOOKEEPER_WRITE;
  }
*/
  rc = zoo_get(zh_.get(), path_.c_str(), 0, buf, &sz, &stat);
  if (rc == ZOK) {
    std::string addrs(buf, sz);
    std::vector<std::string> vec;
    boost::split(vec, addrs, boost::is_any_of(": ")); 
    std::string str;
    int port;
    std::vector<std::string>::iterator it = vec.begin();
    for (; it != vec.end(); ++it) {
      str = *it;
      ++it;
      port = atoi(it->c_str());
      v->push_back(std::make_pair(str, port));
    }
  }
 // zookeeper_process(zh, events);

  return rc == 0;
}

ConfigClient::~ConfigClient() {
  zookeeper_close(zh_.release());
}

void my_data_completion(int rc, const char *value, int value_len,
        const struct Stat *stat, const void *data) {
  if (value) {
  }
}

//rc = zoo_aget(zh, "get /lqh", 1, my_data_completion, strdup(line));
//rc = zoo_set2(zh, line, ptr, strlen(ptr), -1, &stat);
}
/*
int main() {
  net::ConfigClient cc("10.22.200.140:2181");
  std::vector<std::pair<std::string, int> > v;
  bool b = cc.GetIpGroup(&v);
  std::vector<std::pair<std::string, int> >::iterator it = v.begin();
  for(; it != v.end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
  }
}
*/
