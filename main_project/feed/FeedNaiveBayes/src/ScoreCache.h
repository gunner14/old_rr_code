#ifndef FEEDNAIVEBAYESI_SCORECACHE_H_
#define FEEDNAIVEBAYESI_SCORECACHE_H_

#include <string>
#include <cstdio>
#include "Date.h"
#include <boost/lexical_cast.hpp>
#include "./FeedNaiveBayesI.h"
#include "IceLogger.h"
#include "CommonUtil.h"

namespace xce {
namespace feed {

using namespace xce::feed;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace std;
using MyUtil::Date;
using namespace boost;

//"/data/xce/StatUtil/data/score/score_history.2011-05-21"
const static string kHistPath = "/data/xce/StatUtil/data/score_history/";
const static int kCacheSize = 800000000;

class ScoreCache: public Singleton<ScoreCache> {
public:
	ScoreCache() :
		ready_(true), scorecnt_(0), score_map_(kCacheSize) {
	}

	bool Reload() {
		if (ready_ == false) {
      MCE_INFO("ScoreCache::Reload. already in loading or load file all fail");
      return false;
    }
    ready_ = false;
    //score_map_.clear();
    scorecnt_ = 0;
    ready_ = LoadScoreFile();
    MCE_INFO("ScoreCache::Reload  ScoreCache::LoadScore done!!! -->  size:" << score_map_.size()
        << " scorecnt:" << scorecnt_ << " ready:" << ready_);

    return ready_;
  }

  float GetScore(int to, int featureid) {
    if (!ready()) {
      return 0.25;
    }
    uint64_t key = Join2(to, featureid);
    hash_map<uint64_t, float>::iterator it = score_map_.find(key);
    if (it != score_map_.end()) {
      return it->second;
    }
    //check the aver <uid/-stype, 0  --> score>
    uint64_t key_aver = Join2(featureid, 0);
    it = score_map_.find(key_aver);
    if(it != score_map_.end()) {
      return it->second;
    }
    return 0.25;
  }

	bool ready() {
		return ready_;
	}

	void UpdateScore(int to, int featureid, float score) {
		uint64_t key = Join2(to, featureid);
		score_map_[key] = score;
	}

  void UpdateScoreBatch(const IntSeq & userids, const IntSeq & featureids,  const FloatSeq & scores) {
    if (featureids.size() != scores.size() || userids.size() != scores.size()) {
      MCE_WARN("ScoreCache::UpdateScoreBatch  --> size wrong")
        return;
    }
    int featureid, userid;
    float score;
    uint64_t key;
    for(size_t i = 0; i < featureids.size(); ++i) {
      userid = userids[i];
      featureid = featureids[i];
      score = scores[i];
      key = Join2(userid, featureid);
      score_map_[key] = score;
    }

    scorecnt_ += scores.size();
    if (scorecnt_ > 10000000) {
      MCE_INFO("ScoreCache::UpdateScoreBatch. hash_map status --> bucket_cnt:"<< score_map_.bucket_count() 
          << " max_bucket_cnt:" << score_map_.max_bucket_count() 
          << " size:" <<score_map_.size() 
          << " max_size:"<<score_map_.max_size())		 
        scorecnt_ = 0;
    }
  }

private:
  string GetScoreFileName(int i) {
    MyUtil::Date DeltaDays(i* 24 * 60 * 60);
    MyUtil::Date today = Date(time(NULL));
    string name;
    MyUtil::Date d = today - DeltaDays;
    ostringstream oss;
    oss << "score_history." << d.year() << "-" << AddZero(d.month()) << "-"	<< AddZero(d.day());
    return oss.str();
  }

  bool LoadScoreFile() {
    ifstream infile;
    string file;
    for (unsigned i = 0; i < 100; ++i) {
      file = kHistPath + GetScoreFileName(i);
      infile.open(file.c_str(), ifstream::in);
			if (!infile) {
				MCE_INFO("ScoreCache::LoadScoreFile. openfile fail:" << file );
			} else {
				MCE_INFO("ScoreCache::LoadScoreFile. openfile OK:" << file );
				break;
			}
		}
		if (!infile) {
			MCE_WARN("ScoreCache::LoadScoreFile. openfile all fail");
			return false;
		}

    MCE_INFO("ScoreCache::LoadScoreFile. before state --> bucket_cnt:"<< score_map_.bucket_count() 
        << " max_bucket_cnt:" << score_map_.max_bucket_count() 
        << " size:" <<score_map_.size() 
        << " max_size:"<<score_map_.max_size())				
    string line;
		int userid, featureid, duplicatekey=0;
		float score;
    uint64_t key;
    while(getline(infile, line)) {
      if (++scorecnt_ % 10000000 == 0) {
        MCE_INFO("-----------------ScoreCache::LoadScoreFile. loading  --> cnt:" << scorecnt_);
      }
      sscanf(line.c_str(), "%d\t%d\t%f", &userid, &featureid,  &score);
      key = Join2(userid, featureid);
      if (score_map_.find(key) != score_map_.end()) {
        ++duplicatekey;
        continue;
      }
      score_map_[key] = score;
    }
    MCE_INFO("ScoreCache::LoadScoreFile. after state --> bucket_cnt:"<< score_map_.bucket_count() 
        <<" max_bucket_cnt:" << score_map_.max_bucket_count() 
        <<" size:" <<score_map_.size() 
        << " max_size:"<<score_map_.max_size()
        << " duplicateKey:" << duplicatekey)				
    return true;
	}

	bool ready_;
	int scorecnt_;
  hash_map<uint64_t, float> score_map_;
};

}
}
#endif


