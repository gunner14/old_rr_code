#ifndef _DATA_DISPENSE_H_
#define _DATA_DISPENSE_H_
#include <string>
#include <boost/threadpool.hpp>
#include "runtimeconfig.h"
#include "upload/upload.h"
class DiskCache;

namespace upload {

struct OutResponse;
struct OutImageUrl;

/**
 * add processed image to disk cache
 */
struct CacheImageTask {
  CacheImageTask();
  CacheImageTask(DiskCache* diskcache, const std::string& url, const std::string& image);
  void operator()();
  DiskCache* diskcache_;
  std::string url_;
  std::string image_;
};
typedef boost::threadpool::thread_pool<CacheImageTask> DiskCachePool;

class DataDispense {
private:
  enum OutPathType
  {
    kGifPath = 0,
    kOtherImagePath
  };
  struct OutPathPair{
    std::string savepath;
    std::string urlpath;
  };
 public:
  DataDispense();
  void PostFace(const std::string &hostid, const OutImageUrl &oiu);
  int Disp(const std::string &hostid, const ServiceConf* sc, const std::vector<int> &cachetimes, OutResponse &resp);
  void Stop();
 private:
  net::WebPool webpool_;
  net::WebPool webpool_new_;
  DiskCachePool diskcache_pool_;
};



}
#endif
