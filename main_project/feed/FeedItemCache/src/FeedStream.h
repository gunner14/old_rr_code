
/*
 * FeedStream.h
 *
 *  Created on: Aug 31, 2011
 *      Author: happy, guanghe.ge@renren-inc.com
 */

#ifndef __FEEDSTREAM_H__
#define __FEEDSTREAM_H__

#include <map>
#include <deque>
#include <vector>
#include <asm/atomic.h>
#include <util/cpp/TimeCost.h>

#include <Ice/Ice.h>
#include <IceUtil/RWRecMutex.h>
#include <IceLogger.h>

#include <pthread.h>

namespace xce {
namespace feed {

using namespace MyUtil;

const int FEEDSTREAM_MUTEX_NUM = 3000;

template<typename Data>
struct Less {
	bool operator()(const Data& a, const Data& b) {
		return a < b;
	}
};

/*
 *结构内部不再加锁，减少内存占用，用外部加锁解决并发
 */
template<typename FeedData, typename Less = Less<FeedData> >
struct MutexData {
	typedef std::vector<FeedData> DataDeqType;
  struct FeedDataCompare {
    template<typename Data>
    bool operator()(const Data & a, const Data & b) {
      return a.feedid < b.feedid;
    }
  };

	bool is_order;
	DataDeqType data;

	MutexData() : is_order(false) {
	}
	~MutexData() {
	} 

	inline bool order() {
		return is_order;
	}

	inline void set_order(bool order) {
		is_order = order;
	}

	inline bool add(const FeedData& feed){
		if(!data.empty() && data[data.size() - 1] ==  feed) {
			return false;
		}

		data.push_back(feed);
		return true;
	}

	inline int free(const FeedData& upper_bound) {
		int count = 0;
		for(typename DataDeqType::iterator it = data.begin();
				it != data.end() && *it < upper_bound; ++it) {
			count++;
		}

		//free the capacity.
		if (count > 0) {
			data.erase(data.begin(), data.begin() + count);
			//DataDeqType tmp = data;
			//data.swap(tmp);
		}

		return count;
	}

	inline bool get_min(FeedData& output) {
		if(data.empty()) {
			return false;
		}
		output = data[0];
		return true;
	}

	inline int size() {
		return data.size();
	}

	inline void check_order(){
		std::sort(data.begin(), data.end(), Less());
	}

	inline int del(long feedid) {
		int res = 0;
    FeedData temp;
    temp.feedid = feedid;
    typename DataDeqType::iterator it = std::lower_bound(data.begin(), data.end(), temp, FeedDataCompare());
    if (it != data.end() && it->feedid == feedid) {
      data.erase(it);
      res++;
    }
		return res;
	}
};//end MutexData

//---------------------------------------------------------------------
//
template<typename FeedData, typename Less = Less<FeedData> >
class FeedStream {

typedef MutexData<FeedData, Less> MutexDataType;
typedef std::map<int, MutexDataType*> UserMapType;

public:
	FeedStream() {
		atomic_set( &total_count_, 0 );
		
		user_mutexes_.reserve(FEEDSTREAM_MUTEX_NUM);
		for (int i = 0; i < FEEDSTREAM_MUTEX_NUM; i++) {
			int ret;
      user_mutexes_.push_back(pthread_rwlock_t());
			ret = pthread_rwlock_init(&user_mutexes_.back(), NULL);
			
			if (0 != ret)
				user_mutexes_.pop_back();
		}
	}
	~FeedStream() {
		{
			IceUtil::RWRecMutex::WLock lock(mutex_);
			for(typename UserMapType::iterator i = usermap_.begin(); i != usermap_.end(); ++i) {
				delete i->second;
			}
			usermap_.clear();
		}

		user_mutexes_.clear();
	}

	void check_order() {
		typedef std::vector<MutexDataType*> MdsType;
		IceUtil::RWRecMutex::RLock lock(mutex_);
		for(typename UserMapType::const_iterator it = usermap_.begin(); it != usermap_.end(); ++it) {
			MutexDataType* md = it->second;
			int ret = pthread_rwlock_wrlock(&user_mutexes_[it->first%FEEDSTREAM_MUTEX_NUM]);
			if (0 != ret) continue;

			if (!md->order()) {
				md->check_order();
				md->set_order(true);
			}
			pthread_rwlock_unlock(&user_mutexes_[it->first%FEEDSTREAM_MUTEX_NUM]);
		}
	}

	void add_count(int count) {
		atomic64_add(count, &total_count_);
	}

	int user_count() const {
		IceUtil::RWRecMutex::RLock lock(mutex_);
		return usermap_.size();
	}

	long total_count() const {
		return atomic64_read(&total_count_);
	}

	int item_count(int userId) {
		IceUtil::RWRecMutex::RLock lock(mutex_);
		typename UserMapType::const_iterator it = usermap_.find(userId);
		if(it != usermap_.end()) {
			pthread_rwlock_rdlock(&user_mutexes_[userId%FEEDSTREAM_MUTEX_NUM]);
			int result = it->second->size();
			pthread_rwlock_unlock(&user_mutexes_[userId%FEEDSTREAM_MUTEX_NUM]);
			return result;
		}
		return 0;
	}

	bool get_min(FeedData& output) {
		bool init = false;
		FeedData tmp;

		IceUtil::RWRecMutex::RLock lock(mutex_);
		for(typename UserMapType::const_iterator it = usermap_.begin(); it != usermap_.end(); ++it) {
			MutexDataType * md = it->second;
			int ret = pthread_rwlock_rdlock(&user_mutexes_[it->first%FEEDSTREAM_MUTEX_NUM]);
			if (0 != ret) continue;
			if(!init) {
				init = md->get_min(output);
			} else {
				if(md->get_min(tmp) && Less()(tmp, output)) {
					output = tmp;
				}
			}
			pthread_rwlock_unlock(&user_mutexes_[it->first%FEEDSTREAM_MUTEX_NUM]);
		}
		return init;
	}
  
  
  int ItemCountByTypes(int userId,set<int> types) {
    typedef std::vector<FeedData> DataDeqType;
    IceUtil::RWRecMutex::RLock lock(mutex_);
    typename UserMapType::const_iterator it = usermap_.find(userId);
    int result=0;
    if(it != usermap_.end()) {
      pthread_rwlock_rdlock(&user_mutexes_[userId%FEEDSTREAM_MUTEX_NUM]);
      DataDeqType tmp=(*(it->second)).data;
      
      for(typename DataDeqType::iterator i=tmp.begin();i!=tmp.end();i++)
      {
         if(types.count(getsmalltype(i->type))||types.count(getbigtype(i->type))) {
           result++;
         }
      }
      pthread_rwlock_unlock(&user_mutexes_[userId%FEEDSTREAM_MUTEX_NUM]);
      return result;
    }
    return 0;
  }  
  int getItemCountByTypes(MyUtil::IntSeq userids, set<int> types) {
      int count=0;
      for(vector<int>::iterator i=userids.begin();i!=userids.end();i++)
      {
        {
           //IceUtil::RWRecMutex::RLock lock(mutex_);
           count+=ItemCountByTypes(*i,types);
        }
      }

      return count;
  }
  void ItemCountMapByTypes(int userId,set<int>types,MyUtil::Int2IntMap &result) {
      typedef std::vector<FeedData> DataDeqType;
      IceUtil::RWRecMutex::RLock lock(mutex_);
      typename UserMapType::const_iterator it = usermap_.find(userId);

      //MCE_DEBUG("begin ItemCountMapByTypes" <<" uid "<<userId);
      if(it != usermap_.end()) {
        //MCE_DEBUG("if begin ItemCountMapByTypes" <<" uid "<<userId);
        pthread_rwlock_rdlock(&user_mutexes_[userId%FEEDSTREAM_MUTEX_NUM]);
        DataDeqType tmp=(*(it->second)).data;
    
        for(typename DataDeqType::iterator i=tmp.begin();i!=tmp.end();i++)
        {
           if(types.count(getsmalltype(i->type)))
           {
             result[getsmalltype(i->type)]++;
           }  
           if(types.count(getbigtype(i->type)))
            {
             result[getbigtype(i->type)]++;
           }
        }  
        pthread_rwlock_unlock(&user_mutexes_[userId%FEEDSTREAM_MUTEX_NUM]);
      }
  }
  void getItemCountMapByTypes(MyUtil::IntSeq userids,set<int>types,MyUtil::Int2IntMap &result) {
      for(vector<int>::iterator it=userids.begin();it!=userids.end();it++)
      {
        //MCE_DEBUG("begin getItemCountMapByTypes" <<" uid "<<*it);
        ItemCountMapByTypes(*it,types,result);
      }
  }	 
  void free(const FeedData& upper_bound) {
		typedef std::vector<MutexDataType*> MdsType;
		int count = 0;
		IceUtil::RWRecMutex::RLock lock(mutex_);
		for(typename UserMapType::const_iterator it = usermap_.begin(); it != usermap_.end(); ++it) {
			MutexDataType* md = it->second;
			pthread_rwlock_wrlock(&user_mutexes_[it->first%FEEDSTREAM_MUTEX_NUM]);
			count += md->free(upper_bound);
			pthread_rwlock_unlock(&user_mutexes_[it->first%FEEDSTREAM_MUTEX_NUM]);
		}

		if(count > 0) {
			MCE_INFO("FeedStream::free, count: " << count);
			atomic64_sub(count, &total_count_);
		}
	} 

	bool put(int uid, const FeedData& feed, bool addCount = true) {
		MutexDataType* md = NULL;
		{
			IceUtil::RWRecMutex::RLock lock(mutex_);
			typename UserMapType::const_iterator it = usermap_.find(uid);
			if(it != usermap_.end()) {
				md = it->second;
				assert(md);
			}
		}

		if(md == NULL) {
			std::pair<typename UserMapType::iterator, bool> p;
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				typename UserMapType::const_iterator it = usermap_.find(uid);
				if(it != usermap_.end()) {
					md = it->second;
					assert(md);
				}

				if (NULL == md) {
					p = usermap_.insert(std::make_pair(uid, (MutexDataType*)(new MutexDataType)));
					md = p.first->second;
				}
			}

		}

		int ret = pthread_rwlock_wrlock(&user_mutexes_[uid%FEEDSTREAM_MUTEX_NUM]);
		if (0 != ret) return false;
		bool result = md->add(feed);
		pthread_rwlock_unlock(&user_mutexes_[uid%FEEDSTREAM_MUTEX_NUM]);

		if(result && addCount) {
			atomic64_inc(&total_count_);
		}

		return result;
	}

	bool puts(int mod, const std::vector<FeedData>& feeds, bool addCount = true) {
		//校验元素
		for (typename std::vector<FeedData>::const_iterator it = feeds.begin();
				it != feeds.end(); ++it) {
			int uid = (*it).userid;
			typename UserMapType::const_iterator it;
			{
				IceUtil::RWRecMutex::RLock lock(mutex_);
				it = usermap_.find(uid);
			}

			if(it == usermap_.end()) {
				IceUtil::RWRecMutex::WLock lock(mutex_);
				usermap_.insert(std::make_pair(uid, (MutexDataType*)(new MutexDataType)));
			}
		}

		//成批插入OA
		pthread_rwlock_wrlock(&user_mutexes_[mod%FEEDSTREAM_MUTEX_NUM]);
		for (typename std::vector<FeedData>::const_iterator it = feeds.begin();
				it != feeds.end(); ++it) {
			int uid = (*it).userid;
			MutexDataType* md = NULL;
			typename UserMapType::const_iterator im;
			{
				IceUtil::RWRecMutex::RLock lock(mutex_);
				im = usermap_.find(uid);
			}

			if (im != usermap_.end()) {
				md = im->second;
				md->add(*it);
			}
		}
		pthread_rwlock_unlock(&user_mutexes_[mod%FEEDSTREAM_MUTEX_NUM]);
		return true;
	}

	typename std::vector<FeedData> get(int uid) const {
		MutexDataType* md;
		{
			IceUtil::RWRecMutex::RLock lock(mutex_);
			typename UserMapType::const_iterator it = usermap_.find(uid);
			if(it == usermap_.end()) {
				return std::vector<FeedData>();
			}
			md = it->second;
		}
		assert(md);

		if(md) {
			pthread_rwlock_rdlock(&user_mutexes_[uid%FEEDSTREAM_MUTEX_NUM]);
			std::vector<FeedData> result = std::vector<FeedData>(md->data.rbegin(), md->data.rend());
			pthread_rwlock_unlock(&user_mutexes_[uid%FEEDSTREAM_MUTEX_NUM]);
			return result;
		}
		return std::vector<FeedData>();
	}

	typename std::vector<std::vector<FeedData> > gets(const std::vector<int>& uids, size_t limit = 0xffffffff) {
		typedef std::vector<int> UidsType;
		typedef std::map<int, MutexDataType*> MdsMapType;
		MdsMapType mds;
		{
			IceUtil::RWRecMutex::RLock lock(mutex_);
			for(typename UidsType::const_iterator i = uids.begin(); i != uids.end(); ++i) {
				typename UserMapType::const_iterator it = usermap_.find(*i);
				mds[*i] = (it != usermap_.end() ? it->second : NULL);
			}
		}

		std::vector<std::vector<FeedData> > v;
		v.reserve(uids.size());
		for(typename MdsMapType::const_iterator i = mds.begin(); i != mds.end(); ++i) {
			MutexDataType* md = i->second;

			if(NULL != md) {
				int ret = pthread_rwlock_rdlock(&user_mutexes_[i->first%FEEDSTREAM_MUTEX_NUM]);
				if (0 != ret) continue;
				//if (md->data.size() > limit && !md->order()) {
				//	md->check_order();
				//}

				v.push_back(typename std::vector<FeedData>(md->data.rbegin(), md->data.rbegin() + std::min(limit, md->data.size())));
				pthread_rwlock_unlock(&user_mutexes_[i->first%FEEDSTREAM_MUTEX_NUM]);
			} else {
				v.push_back(std::vector<FeedData>());
			}
		}
		return v;
	}

	template<typename TypeFilter, typename IdsFilter>
	typename std::vector<std::vector<FeedData> > gets_with_filter(const std::vector<int>& uids, TypeFilter types, IdsFilter ids, size_t limit = 0xffffffff) {

		typedef std::vector<int> UidsType;
		typedef std::map<int, MutexDataType*> MdsMapType;
		MdsMapType mds;
		{
			IceUtil::RWRecMutex::RLock lock(mutex_);
			for(typename UidsType::const_iterator i = uids.begin(); i != uids.end(); ++i) {
				typename UserMapType::const_iterator it = usermap_.find(*i);
				mds[*i] = (it != usermap_.end() ? it->second : NULL);
			}
		}
		
		std::vector<std::vector<FeedData> > v;
		v.reserve(uids.size());

		for(typename MdsMapType::const_iterator i = mds.begin(); i != mds.end(); ++i) {
			MutexDataType* md = i->second;

			if(NULL != md) {
				int res = pthread_rwlock_rdlock(&user_mutexes_[i->first%FEEDSTREAM_MUTEX_NUM]);
				if (0 != res) continue; 
				//if (md->data.size() > limit && !md->order()) {
				//	md->check_order();
				//}

				size_t lim = std::min(limit, md->data.size());
				std::vector<FeedData> vf;
				vf.reserve(lim);
				for(typename MutexDataType::DataDeqType::const_reverse_iterator it = md->data.rbegin(); lim > 0 && it != md->data.rend(); ++it) {
					if(types(*it) && !ids(*it)) {
						vf.push_back(*it);
						--lim;
					}
				}
				v.push_back(vf);
				pthread_rwlock_unlock(&user_mutexes_[i->first%FEEDSTREAM_MUTEX_NUM]);
			} else {
				v.push_back(std::vector<FeedData>());
			}
		}

		return v;
	}

	template<typename TypeFilter, typename IdsFilter>
	typename std::vector<std::vector<FeedData> > gets_less_feedid_with_filter(const std::vector<int>& uids, TypeFilter types, IdsFilter ids, ::Ice::Long feedid, size_t limit = 0xffffffff) {

		typedef std::vector<int> UidsType;
		typedef std::map<int, MutexDataType*> MdsMapType;
		MdsMapType mds;
		{
			IceUtil::RWRecMutex::RLock lock(mutex_);
			for(typename UidsType::const_iterator i = uids.begin(); i != uids.end(); ++i) {
				typename UserMapType::const_iterator it = usermap_.find(*i);
				mds[*i] = (it != usermap_.end() ? it->second : NULL);
			}
		}
		
		std::vector<std::vector<FeedData> > v;
		v.reserve(uids.size());

		for(typename MdsMapType::const_iterator i = mds.begin(); i != mds.end(); ++i) {
			MutexDataType* md = i->second;

			if(NULL != md) {
				int ret = pthread_rwlock_rdlock(&user_mutexes_[i->first%FEEDSTREAM_MUTEX_NUM]);
				if (0 != ret) continue;
				//if (md->data.size() > limit && !md->order()) {
				//	md->check_order();
				//}

				size_t lim = std::min(limit, md->data.size());
				std::vector<FeedData> vf;
				vf.reserve(lim);
				for(typename MutexDataType::DataDeqType::const_reverse_iterator it = md->data.rbegin(); lim > 0 && it != md->data.rend(); ++it) {
					if(((*it).feedid < feedid) && types(*it) && !ids(*it)) {
						vf.push_back(*it);
						--lim;
					}
				}

				v.push_back(vf);
				pthread_rwlock_unlock(&user_mutexes_[i->first%FEEDSTREAM_MUTEX_NUM]);
			} else {
				v.push_back(std::vector<FeedData>());
			}
		}

		return v;
	}

	template<typename TypeFilter, typename IdsFilter>
	typename std::vector<std::vector<FeedData> > gets_bigger_feedid_with_filter(const std::vector<int>& uids, TypeFilter types, IdsFilter ids, ::Ice::Long feedid, size_t limit = 0xffffffff) {

		typedef std::vector<int> UidsType;
		typedef std::map<int, MutexDataType*> MdsMapType;
		MdsMapType mds;
		{
			IceUtil::RWRecMutex::RLock lock(mutex_);
			for(typename UidsType::const_iterator i = uids.begin(); i != uids.end(); ++i) {
				typename UserMapType::const_iterator it = usermap_.find(*i);
				mds[*i] = (it != usermap_.end() ? it->second : NULL);
			}
		}
		
		std::vector<std::vector<FeedData> > v;
		v.reserve(uids.size());

		for(typename MdsMapType::const_iterator i = mds.begin(); i != mds.end(); ++i) {
			MutexDataType* md = i->second;

			if(NULL != md) {
				int ret = pthread_rwlock_rdlock(&user_mutexes_[i->first%FEEDSTREAM_MUTEX_NUM]);
				if (0 != ret) continue; 
				//if (md->data.size() > limit && !md->order()) {
				//	md->check_order();
				//}

				size_t lim = std::min(limit, md->data.size());
				std::vector<FeedData> vf;
				vf.reserve(lim);
				for(typename MutexDataType::DataDeqType::const_reverse_iterator it = md->data.rbegin(); lim > 0 && it != md->data.rend(); ++it) {
					if(((*it).feedid > feedid) && types(*it) && !ids(*it)) {
						vf.push_back(*it);
						--lim;
					}
				}

				v.push_back(vf);
				pthread_rwlock_unlock(&user_mutexes_[i->first%FEEDSTREAM_MUTEX_NUM]);
			} else {
				v.push_back(std::vector<FeedData>());
			}
		}

		return v;
	}

	inline void del(int userid, long feedid) {
		MutexDataType* md = NULL;
		{
			IceUtil::RWRecMutex::WLock lock(mutex_);
			typename UserMapType::const_iterator it = usermap_.find(userid);
			if(it != usermap_.end()) {
				md = it->second;
				assert(md);
			}
		}

		if(md == NULL) {
			return ;
		}

		int ret = pthread_rwlock_wrlock(&user_mutexes_[userid%FEEDSTREAM_MUTEX_NUM]);
		if (0 != ret) 
			return ;
		int result = md->del(feedid);
		pthread_rwlock_unlock(&user_mutexes_[userid%FEEDSTREAM_MUTEX_NUM]);

		if(result > 0) {
			add_count(-result);
		}
	}
private:

	atomic64_t total_count_;

	IceUtil::RWRecMutex mutex_;
	
	std::vector<pthread_rwlock_t> user_mutexes_;

	UserMapType usermap_;

  inline int getsmalltype(int type) {
    return type&0xffff;
  }

  inline int getbigtype(int type) {
    return (int)((type&0xffff)/100*100);
  }

};//end FeedStreams

}
}

#endif // __FEEDSTREAM_H__
