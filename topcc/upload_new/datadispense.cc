#include "datadispense.h"
#include <libmemcached/memcached.h>
#include "util.h"
#include "upload_head.h"
#include "memcache.h"
#include "runtimeconfig.h"
#include "restoremanage.h"
#include "base3/logging.h"
#include "base3/sysloging.h"
#include "base3/ptime.h"
#include "imagehelper.h"
#include "base3/asyncall.h"

namespace upload {

DataDispense::DataDispense() :diskcache_pool_(1){
  std::vector<net::Address> v;
  v.push_back(std::make_pair("10.7.18.125", 80));
  webpool_.Start(v);
 
  std::vector<net::Address> v1;
  v1.push_back(std::make_pair("10.4.16.103", 80));
  webpool_new_.Start(v1);
}

void DataDispense::Stop() {
  webpool_.Close();
  webpool_new_.Close();
}

void DataDispense::PostFace(const std::string &hostid, const OutImageUrl &oiu) {
  net::HtmlData hd;
  //hd.head.push_back("Cookie: "+request.Env("COOKIE"));
  char tmp[128];
  sprintf(tmp, "width=%d", oiu.width_);
  hd.AppendBody(tmp);
  sprintf(tmp, "height=%d", oiu.height_);
  hd.AppendBody(tmp);
  hd.AppendBody(std::string("largeUrl=")+oiu.url_);
  hd.AppendBody(std::string("hostId=")+hostid);
  hd.AssignData(oiu.data_.c_str(), oiu.data_.size());
  //hd.body.push_back(std::string("albumId=")+request.Query("albumid"));
  webpool_.PostFile(hd);
  webpool_new_.PostFile(hd);
}

int DataDispense::Disp(const std::string &hostid, const ServiceConf* sc, const std::vector<int> &cachetimes, OutResponse &resp) {
  std::set<std::string> filter;
  int sucess = 0;
  OutPathPair paths[2];
  bool path_constructed[2];

  for (std::vector<OutFile>::iterator it = resp.files_.begin(); it != resp.files_.end(); ++it) {
    // initiate two flags to support constructing save path on demand
    path_constructed[kGifPath] = path_constructed[kOtherImagePath] = false;

    std::vector<int>::const_iterator cacheit = cachetimes.begin();
    std::vector<OutImageUrl>::iterator it1 = it->images_.begin();
    for (; it1 != it->images_.end(); ++it1) {
      // 重复文件,不保存
      size_t pathType =
          CImageHelper::IsGIF(it1->data_) ? kGifPath : kOtherImagePath;
      if (!path_constructed[pathType]) {
        if (!RestoreManager::GetPath(*sc, pathType == kGifPath,
            &paths[pathType].urlpath, &paths[pathType].savepath)) {
          return sucess;
        }
        path_constructed[pathType] = true;
      }
      std::string fullsavepath = paths[pathType].savepath + it1->url_;
      it1->url_ = paths[pathType].urlpath + it1->url_;
      if (it1 == it->images_.begin())
        LOG(INFO) << "generator first path: " << it1->url_ << " size: "  << it1->data_.size();
      if (sc->checkface_ == true && it1->nameprefix_ == "large") {
        PostFace(hostid, *it1);
      }
      if (filter.find(it1->url_) != filter.end())
        continue;
      filter.insert(it1->url_);
      {
        PTIME(pt1, "savefile", true, 5);
        if (!RestoreManager::SaveFile(fullsavepath, it1->data_)) {
          it->code_ = 535;
          it->msg_ = "存储图片失败";
          break;
        }
        else
        {
          // successful
          logging::syslog("%s", fullsavepath.c_str());
          // cache image
          const size_t kMaxPending = 1024;
          static size_t counter = 0;
          if (++counter % kMaxPending == 0) {
            size_t pending = diskcache_pool_.pending();
            if (pending > kMaxPending) {
              // error
              diskcache_pool_.clear();
              LOG(ERROR) << "DiskCacheQueue is too long, cleared";
            } else {
              LOG(INFO) << "DiskCacheQueue size: " << pending;
            }
          }
          CacheImageTask task(&gDiskCache, it1->url_, it1->data_);
          diskcache_pool_.schedule(task);
        }
      }
    }
    if (it1 == it->images_.end())
      sucess++;
  }

  //std::string groupname_; 这个其实没什么作用 
  return sucess;
}

CacheImageTask::CacheImageTask(DiskCache* diskcache, const std::string& url, const std::string& image)
  : diskcache_(diskcache), url_(url), image_(image)  {
}
void CacheImageTask::operator()() {
  if (!diskcache_ || !diskcache_->Valid()) return;
  const time_t expiration = 7 * 24 * 3600;
  const size_t kMaxRetry = 2;
  size_t retry = 0;
  const size_t kBufSize = 256;
  char errbuf[kBufSize];
  do {
    if (diskcache_->Set(url_, image_, expiration)) {
      LOG(INFO) << "CacheImage: ok, url: " << url_ << ", size: " << image_.size();
      return;
    } else {
      if (diskcache_->LastError() == MEMCACHED_E2BIG) {
        LOG(INFO) << "CacheImage: E2BIG, url: " << url_ << ", size: " << image_.size();
        return;
      } else {
        const char* msg = diskcache_->LastErrorMsg();
        if (msg) {
          strncpy(errbuf, msg, kBufSize);
        } else {
          sprintf(errbuf, "%d", diskcache_->LastError());
        }
        LOG(INFO) << "CacheImage: failed, rc: " << errbuf << " url: " << url_ << ", size: " << image_.size();
        sleep(1);
      }
    }
  } while (++retry < 2);
}

} //namespace
