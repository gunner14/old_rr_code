#ifndef _XOA2_SERVICE_CONFIG_H_
#define _XOA2_SERVICE_CONFIG_H_

#include <string>
#include <vector>
namespace xoa2 {

class Xoa2ServiceConfig {
public:
  Xoa2ServiceConfig() : listen_port_(0),
      thread_pool_core_size_(0),
      thread_pool_max_size_(0),
      version_("1") {
  }

  int listen_port() const {
    return listen_port_;
  }

  std::string service_id() const {
    return service_id_;
  }

  int thread_pool_core_size() const {
    return thread_pool_core_size_;
  }

  int thread_pool_max_size() const {
    return thread_pool_max_size_;
  }

  std::string version() const {
    return version_;
  }


  int Init(const std::string & conf_file);

private:
  std::string service_id_;
  unsigned short listen_port_;
  int thread_pool_core_size_;
  int thread_pool_max_size_;
  std::string version_; 

  std::vector<std::string> tag_path_;

  std::string GetPath(const std::string & key_name) const;

  static void SchemaStartTag(void *data, const char *el, const char **attr);
  static void SchemaEndTag(void *data, const char *el);
};

}

#endif // _XOA2_SERVICE_CONFIG_H_

