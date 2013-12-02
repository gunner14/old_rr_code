/*
 * Common.h
 *
 *  Created on: Mar 27, 2012
 *      Author: rong.tang
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <list>
#include <set>
#include <Ice/Object.h>
#include <Singleton.h>
#include <LogWrapper.h>
#include <boost/foreach.hpp>
#include <boost/bimap.hpp>
#include <boost/typeof/typeof.hpp>
#include <math.h>
#include <boost/tuple/tuple.hpp>
#include "Date.h"

#include "RFeed.h"
#include "UserInfo.h"
#include "SharedFeedCache.h"

using namespace std;
using namespace MyUtil;
using boost::tuple;
using boost::make_tuple;
using boost::bimap;

namespace xce {
namespace feed {


class MergeConfig: public MyUtil::Singleton<MergeConfig>{
public:
  /*
   * sort   主类型 评论类型
   *   0    601  2901    日志
   *   0    701  2902    评论单张照片
   *   1    709  2906    评论多张照片(相册)
   *   1    712  2905    评论终端页中的单张照片
   */
	MergeConfig(){
		typedef bimap<int,int>::value_type vt;
		bm_.insert(vt(601 , 2901));
		bm_.insert(vt(701 , 2902));
		bm_.insert(vt(709 , 2906));
		bm_.insert(vt(712 , 2905));
	}

	bool IsMain(int stype){ return bm_.left.count(stype); }
	bool IsComment(int stype){ return bm_.right.count(stype); }

	int Main2Comment(int stype){
		BOOST_AUTO(it,bm_.left.find(stype));
		if(it != bm_.left.end()) return it->second;
		else return stype;
	}
	int Comment2Main(int stype){
		BOOST_AUTO(it,bm_.right.find(stype));
		if(it != bm_.right.end()) return it->second;
		else return stype;
	}

private:
	bimap<int,int> bm_;//main comment bimap
};

class TypeUtil{
public:
	static int GetStype(int stype){ return stype & 0xFFFF; }
};

//typedef IceUtil::Handle<HeavyItem> HeavyItemPtr;
class HeavyFeed;
typedef HeavyFeed* HeavyFeedPtr;
typedef vector<HeavyFeedPtr> HFeed1D;
typedef vector<HFeed1D> HFeed2D;
typedef vector<HFeed2D> HFeed3D;

typedef map<pair<int, long> , HFeed1D> StypemergeHFeed1DMap;
typedef map<pair<int,long >, double> StypemergeScoreMap;
typedef multimap<double, StypemergeHFeed1DMap::iterator> ScoreIteratorMultiMap;

typedef map<pair<int, long> , SFeed1D> StypemergeSFeed1DMap;

class GetRequest {
public:
	int uid_;
	bool sort_;
	bool by_stype_not_btype_;
	int begin_;
	int limit_;
	enum FilterStyle{
		kNone,kInclude,kExclude,kError
	};
	set<int> include_types_,exclude_types_;
	set<long> marked_fids_;

	StypemergeHFeed1DMap comment_map_; 
	StypemergeSFeed1DMap showed_comment_map_; 

	StypemergeHFeed1DMap stypemerge_item1d_map_;
	ScoreIteratorMultiMap score_iterator_multimap_;

	HFeed3D hfeed3d_;

	HFeed3D showed_hfeed3d_;
	SFeed3D showed_sfeed3d_;
	HeavyFeedPtr random_;
	set<long> showed_fids_;


	bool getshow_;
	time_t nowtime_;
	int older_count_;
	int useless_count_;//由于merge策略，不需要取本体的数量
	FilterStyle filter_style_;

public:
	GetRequest(int uid, const vector<int>& types, bool stype, int begin, int limit, bool hot) {
		uid_ = uid;
		sort_ = hot;
		begin_ = begin;
		limit_ = limit;
		by_stype_not_btype_ = stype;
		random_ = 0;
		getshow_ = false;
		nowtime_ = time(NULL);
		older_count_ = 0;
		useless_count_ = 0;
		BOOST_FOREACH(int type,types){
			if(type > 0)
				include_types_.insert(type);
			else if(type < 0)
				exclude_types_.insert(type);
			//TODO. if type == 0
		}
		bool include = !include_types_.empty();
		bool exclude = !exclude_types_.empty();
		if(include and exclude){
			filter_style_ = kError;
		} else if(!include and !exclude){
			filter_style_ = kNone;
		} else if(include){
			filter_style_ = kInclude;
		} else{
			filter_style_ = kExclude;
		}
	}

	bool CheckArgs();
	set<int> GetIncludeTypes() const { return include_types_;}
	set<int> GetExcludeTypes() const { return exclude_types_;}
	bool CheckType(int stype){
		int realtype = by_stype_not_btype_ ? stype : (stype / 100 * 100);

		switch (filter_style_){
		case kNone:
			return true;
		case kError:
			return false;
		case kInclude:
			return include_types_.count(realtype);
		case kExclude:
			return !exclude_types_.count(-realtype);
		default:
			return true;
		}

	}

	bool SeperateComment(HeavyFeedPtr & item){
		int stype = item->GetStype();
		if(!(MergeConfig::instance().IsComment(stype))) return false;
		if(!ShowComment()) return true;
		int main_stype = MergeConfig::instance().Comment2Main(stype);
		int version = item->GetVersion();
		int main_vstype = (version << 16) | main_stype;
		comment_map_[make_pair(main_vstype,item->GetMerge())].push_back(item);
		return true;
	}
	bool SeperateRandom(HeavyFeedPtr & item){
		if (ShowRandom()
				&& item->IsRandomType()
				&& !random_) {
			random_ = item;
			return true;
		}
		return false;
	}

	void PushBackRandom();

	set<long> & GetShowedFids();
	int GetShowedFidsFromHFeed1D(HFeed1D & hfeed1d,SFeed2D & sfeed2d);
	SFeed3D & GetShowedSharedFeed3D();
	set<long> & GetMarkedFids() { return marked_fids_;}

	time_t GetNowTime() const { return nowtime_; }
	int GetOlderCount();
	int GetUselessCount() const { return useless_count_; }
	bool ShowComment(){return sort_;}
	bool ShowRandom(){return sort_ && begin_ ==0 ;}
	bool IsSort(){return sort_;};

	bool GetComments(pair<int,long> stypemerge,HFeed1D & comments);

	void MakeItem2dAndPush(const HeavyFeedPtr & item){
		HFeed1D item1d;
		item1d.push_back(item);
		HFeed2D item2d;
		item2d.push_back(item1d);
		hfeed3d_.push_back(item2d); //放到顶端
	}

	bool InRange(int idx){
		return idx >= begin_ && idx< begin_+limit_;
	}

	string ToString();
	void DumpComments();

	bool MustRaiseUgc(){
		int idx = uid_ % 10;
		return (idx == 5 or uid_ == 220678672 or uid_ == 239556923) and sort_ and 0 == begin_;
	}
	void RaiseUgc();
};

inline static ostream &operator<<(ostream &stream, const FeedItem & i) {
  stream << "[id:" << i.feed << " t:" << (i.type & 0xFFFF)<< " a:" << i.actor << " t:" << i.time << " mg:"<< i.merge << "]";
  return stream;
}


//------------------------------------------

class PrintHelper {
public:
//  template<typename T>
//  static string Print(const T & container) {
//    ostringstream ios;
//    ios << "(";
//    bool first_item = true;
//    int i = 0;
//    for (typename T::const_iterator itr = container.begin(); itr
//        != container.end(); ++itr, ++i) {
//      if (first_item == true) first_item = false;
//      else ios << ",";
//      ios << *itr;
//    }
//    ios << ")";
//    return ios.str();
//  }

  template<typename T>
  static string Print(const T & container,int limit = 30) {
    ostringstream ios;

    ios << "(";
    int idx = 0;
    typedef typename T::value_type vt;
    BOOST_FOREACH(const vt & v,container){
    	if(idx++ > 0) ios << ",";

    	ios << v;

      if(idx >= limit) break;
    }
    ios << ")";

    return ios.str();
  }

  template<typename T>
  static string PrintKeys(const T & container) {
    ostringstream ios;

    ios << "(";
    int idx = 0;
    typedef typename T::value_type vt;
    BOOST_FOREACH(const vt & v,container){
    	if(idx++ > 0) ios << ",";
      ios << v.first;
    }
    ios << ")";

    return ios.str();
  }

  static void Print( const HFeed3D & item3d, int uid = 0 ){
  	int totalcnt = 0;
  	MCE_INFO("uid:" << uid << "item3d--> 2d count:" << item3d.size());
  	BOOST_FOREACH(const HFeed2D & item2d,item3d){
  		MCE_INFO("uid:" << uid << "\titem2d--> 1d count:" << item2d.size());
  		ostringstream os;
  		BOOST_FOREACH(const HFeed1D & item1d, item2d){
  			MCE_INFO("uid:" << uid <<"\t\titem1d--> item count:" << item1d.size());
  			BOOST_FOREACH(const HeavyFeedPtr & item,item1d){
  					MCE_INFO("uid:" << uid <<"\t\t\tfid:" << item->GetFid()
  							<< " stype:" << item->GetStype()
  							<< " actor:" << item->GetActor()
  							<< " merge:" << item->GetMerge());
  					++totalcnt;
  			}
  		}
  	}
  	MCE_INFO("totalcnt:" << totalcnt);
  }
};

class TimeStat {
public:
  TimeStat() {  reset(); }
  void reset() { gettimeofday(&_begin, NULL); }
  float Get(bool clear = true) {
    gettimeofday(&_end, NULL);
    float timeuse = 1000000 * (_end.tv_sec - _begin.tv_sec)
    								+( _end.tv_usec - _begin.tv_usec);
    if(clear) reset();
    return timeuse / 1000;
  }
private:
  timeval _begin, _end;
};

class Trace{
public:
	Trace(const string & name):name_(name){
		MCE_INFO( "@@<<<< : " << name_  );
	}
	~Trace(){
		MCE_INFO( "@@>>>> : " << name_  );
	}
private:
	string name_;
};

}
}
#endif /* COMMON_H_ */
