#ifndef __XCE_FEED_CONSISTENT_HASHING_H__
#define __XCE_FEED_CONSISTENT_HASHING_H__

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <pthread.h>
#include "boost/foreach.hpp"
#include "doobs_hash.h"
#include "RWLock.h"


namespace xce {
namespace feed {


class Continuum {
public:

  explicit Continuum(const std::string& n = "") : name_(n) {
		pthread_rwlockattr_t lockattr;
		
		if (int ret = pthread_rwlockattr_init(&lockattr)) {
      MCE_WARN("Continuum::Continuum() --> pthread_rwlockattr_init error:" << ret);
      exit(-2);
		}

		if (int ret = pthread_rwlockattr_setkind_np(&lockattr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP)) {
      MCE_WARN("Continuum::Continuum() --> pthread_rwlockattr_setkind_np error:" << ret);
      exit(-2); 
    }


		if (int ret = pthread_rwlock_init(&rwlock_,&lockattr)) {
      MCE_WARN("Continuum::Continuum() --> pthread_rwlock_init error:" << ret);
    }

		pthread_rwlockattr_destroy(&lockattr);
	}

	// 每个 cache node 会对应 continuum 上数百个(按比例)的cache point
	struct CachePoint {
		CachePoint(unsigned int point, const std::string & desc, const std::string ip, int port, unsigned int cap = 1):
        hash_point_(point), desc_(desc), ip_addr_(ip), port_(port), capacity_(cap) { }

		CachePoint(unsigned int point = 0): hash_point_(point), port_(0), capacity_(0) { }

		unsigned int hash_point_;  // point on continuum circle
		std::string desc_;
    std::string ip_addr_;
    int port_;
    unsigned int capacity_;
		bool operator<(const CachePoint & r) const { return hash_point_ < r.hash_point_; }
	};

  struct EndPoint {
    std::string ip_addr_;
    int port_;
    unsigned int capacity_; 
    EndPoint(std::string ip, int port, unsigned int cap):ip_addr_(ip), port_(port), capacity_(cap) {}
    EndPoint():port_(0), capacity_(0) {}
  };


  Continuum::CachePoint Locate(unsigned int hash) const {
    std::string desc;
		ReadLock rl(&rwlock_);

    Continuum::CachePoint cp;
    if (points_.empty()) {
      return cp;
    }

    PointList::const_iterator it = std::lower_bound(points_.begin(), points_.end(), CachePoint(hash));
    if (it != points_.end()) {
      return *it; 
    } else {
      return points_[0];
    }
  }

  Continuum::CachePoint Locate(const char* key, size_t length) {
    return Locate(Hash(key, length));
  }

  static unsigned int Hash(const char* key, size_t length) {
    return doobs_hash(key, length, 0);
  }
  static unsigned int Hash(const std::string& key) {
    return doobs_hash(key.c_str(), key.size(), 0);
  }

  void Add(const std::string& desc, const std::string ip, int port, unsigned int capacity) {
		WriteLock wl(&rwlock_);
    EndPoint ep(ip, port, capacity);
    desc_map_[desc] = ep;
  }

  void Remove(const std::string& desc) {
		WriteLock wl(&rwlock_);
    desc_map_.erase(desc);
  }

  void Clear() {
		WriteLock wl(&rwlock_);
    desc_map_.clear();
  }

  template <typename HashFunc>
  bool Rebuild(HashFunc func) {
    unsigned int total_point = 0;

		WriteLock wl(&rwlock_);

    BOOST_FOREACH(const DescMapType::value_type& i, desc_map_) {
      total_point += i.second.capacity_;
      // TODO: overflow check
    }

    if(total_point == 0) {
      return false;
    }

    PointList target;
    // GOD! it's a very huge number
    target.reserve(total_point);

    const size_t big_enough = 50 + 5; // calc from desc_length
    char buf[big_enough];
    // unsigned char digest[16];

    BOOST_FOREACH(const DescMapType::value_type& i, desc_map_) {
      for (unsigned int k = 0; k < i.second.capacity_; ++k) {
        int len = snprintf(buf, big_enough, "%s-%x", i.first.c_str(), k);

        if(len < 0) {
          MCE_WARN("Continuum::Rebuild --> output error. desc:" << i.first << " k:" << k);
          return false;

        } else if((size_t)len >= big_enough) {
          MCE_WARN("Continuum::Rebuild --> output error. desc:" << i.first << " k:" << k);
          return false;
        }


        unsigned int p = func(buf, len);
        target.push_back(CachePoint(p, i.first, i.second.ip_addr_, i.second.port_, i.second.capacity_));
      }
    }


    // std::sort
    std::sort(target.begin(), target.end());

    points_.swap(target);

    return true;
  }

  bool Rebuild() {
    typedef unsigned int (*TheHashF)(const char*, size_t);
    return Rebuild<TheHashF>(&Continuum::Hash);
  }

  const size_t size() const {
		ReadLock rl(&rwlock_);
    int sz = points_.size();
    return sz;
  }

  const std::string& name() const {
    return name_;
  }

  std::string Dump() const {

		ReadLock rl(&rwlock_);

    std::ostringstream oss;
    
    BOOST_FOREACH(const CachePoint& p, points_) {
      oss << p.hash_point_ << " " << p.desc_  << " " << p.ip_addr_ << " " << p.port_ << " " << p.capacity_ << std::endl;
    }
    oss << "continuum name:" << name_ << " size:" << size();
    return oss.str();
  }



private:


  typedef std::vector<CachePoint> PointList;
  PointList points_;

  // 希望是字典序的列表，故使用map
  typedef std::map<std::string, EndPoint> DescMapType;
  DescMapType desc_map_;

  std::string name_;

	mutable pthread_rwlock_t rwlock_;
};

}} 
#endif 
