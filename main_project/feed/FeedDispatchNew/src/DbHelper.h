/*
 * DBHelper.h
 *
 *  Created on: Dec 24, 2009
 *      Author: antonio
 */

#ifndef DBHELPER_H_
#define DBHELPER_H_

#include "QueryRunner.h"
#include "Singleton.h"
#include "RFeed.h"
#include "IceLogger.h"
#include "Common.h"
namespace xce {
namespace feed {

const int DB_CONTENT_ZONE=1000000000;
const static int DB_ZONE_SPLIT=18;
const int DB_TASK_LEVEL = 1004;
using namespace com::xiaonei::xce;
using namespace MyUtil;

static string DB_INSTANCE = "feed_db";
static string DB_BC_INSTANCE = "feed_content_db";
static string DB_ZONE_CONTENT_INSTANCE = "feed_content_db";

const string GROUP_DB_INSTANCE = "feed_minigroup";
const string TAB_ID_SEQ = "feed_id_seq";

#define COL_FEED "feed"
#define COL_SOURCE "source"
#define COL_SMALLTYPE "small_type"
#define COL_PSOURCE "psource"
#define COL_PTYPE "ptype"
#define COL_BIG_TYPE "big_type"
#define COL_ACTOR "actor"
#define COL_NEWS_MERGE "news_merge"
#define COL_MINI_MERGE "mini_merge"
#define COL_TIME "time"

const int LOAD_TIMEOUT = 1000; //200ms
const int ID_SEQ_STEP=1000;

class DbHelper: public MyUtil::Singleton<DbHelper> {
public:

  DbHelper():_curIndex(-1), _topIndex(-1) {};
  FeedIndexSeq getFeedIndex(int stype, Ice::Long source, int actor);
  FeedIndexSeq getFeedIndexByTime(Ice::Long source, int stype, int actor, int time);
  FeedIndexSeq getFeedIndexBySource(int stype, Ice::Long source);
  FeedIndexSeq getFeedIndexByPsource(int ptype, Ice::Long psource);
  FeedIndexPtr getFeedIndex(Ice::Long feed);
  FeedIndexSeq getFeedIndexBC(int stype,Ice::Long source,int actor);
  map<string,string> getFeedContent(Ice::Long feed_id);
  map<string,string> getFeedZoneContent(Ice::Long feed_id);
  bool copyFeedContent(Ice::Long old_feed_id, Ice::Long new_feed_id, int new_type, const string& new_props);
  bool copyFeedZoneContent(Ice::Long old_feed_id, Ice::Long new_feed_id, int new_type, const string& new_props);
  map<string,string> getFeedConfigProp(Ice::Long fid);
  map<string,string> getFeedZoneConfigProp(Ice::Long fid);
  
  bool isNewFeedidForDb(long feedid) {
    return (feedid / DB_CONTENT_ZONE) >= DB_ZONE_SPLIT || feedid < 0;
  }

  //拆分新旧新鲜事feedid列表
  void SplitFeedidsForDb(const MyUtil::LongSeq & feedids, MyUtil::LongSeq & oldids, MyUtil::LongSeq & newids) {
    for(MyUtil::LongSeq::const_iterator it = feedids.begin(); it != feedids.end(); ++it) {
      if (isNewFeedidForDb(*it)) {
        newids.push_back(*it);
      } else {
        oldids.push_back(*it);
      }
    }
  }

  string getIndexTab(Ice::Long feed) {
    ostringstream tab;
    tab << "feed_index_" << feed % 10;
    return tab.str();
  }
  string getIndexBCTab(Ice::Long feed) {
    ostringstream tab;
    tab << "feed_index_bc_" << (-feed) % 10;
    return tab.str();
  }

  string getContentTab(Ice::Long feed) {
    ostringstream tab;
    tab << "feed_content_" << feed % 100;
    return tab.str();
  }
  string getZoneContentTab(Ice::Long feed) {
    ostringstream tab;
    //tab << "feed_content_" << feed % 100 << "_" << feed/DB_CONTENT_ZONE;
    if (feed > 0) {
                    tab << "feed_content_" << feed % 100 << "_" << feed/DB_CONTENT_ZONE;
                } else {
                    tab << "feed_content_bc_" << abs(feed)%100 << "_" << abs(feed)/DB_CONTENT_ZONE;
                }
                MCE_DEBUG("DbHelper::getZoneContentTable --> feed:" << feed << " tab:"<< tab.str());
    return tab.str();
  }
  string getZoneContentTab(int mod, int zone) {
    ostringstream tab;
    //tab << "feed_content_" << mod << "_" << zone;
    if (zone > 0) {
                     tab << "feed_content_" << mod << "_" << zone;
                } else {
                     tab << "feed_content_bc_" << abs(mod) << "_" << abs(zone);
                }
                MCE_DEBUG("DbHelper::getZoneContentTable --> mod:" << mod << " zone:" << zone << " tab:"<< tab.str());
    return tab.str();
  }
  string getMiniTab(int user,bool after_2009) {
    ostringstream tab;
    tab << "feed_mini_";
                if(!after_2009) {
                    tab<<"bc_";
                }
                tab<< user % 100;
    return tab.str();
  }
        string getMiniDB(bool after_2009) {
                if(after_2009)
                    return "feed_db";
                else
                    return "feed_content_db";
        }
  string getSchoolTab(int schoolid) {
    ostringstream tab;
    tab << "feed_school_" << schoolid % 10;
    return tab.str();
  }
  string getGroupTab(int groupid) {
    ostringstream tab;
    tab << "feed_group_" << groupid % 100;
    return tab.str();
  }
  Ice::Long _curIndex;
  Ice::Long _topIndex;
  IceUtil::Mutex _mutex;
};

class IndexLoader;
typedef IceUtil::Handle<IndexLoader> IndexLoaderPtr;

class IndexLoader: public IceUtil::Shared, public IceUtil::Monitor<
    IceUtil::Mutex> {
public:
  IndexLoader() :
    _count(0) {
  }
  ;

  FeedIndexSeq query();

  void addQuery(const string& dbInstance, const string& sql,
      const string& pattern, bool isSelect = true);

  void finish(FeedIndexSeq& indexSeq) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    _indexSeq.insert(_indexSeq.end(), indexSeq.begin(), indexSeq.end());
    if (--_count == 0) {
      //MCE_INFO("IndexLoader --> finish  res.size:"<<_indexSeq.size());
      notify();
    };
  }
  void finish() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    if (--_count == 0) {
      notify();
    };
  }
private:
  int _count;
  vector<MyUtil::TaskPtr> _taskSeq;
  FeedIndexSeq _indexSeq;

  class SqlExecuteTask: public MyUtil::Task {
  public:
    SqlExecuteTask(const IndexLoaderPtr& loader, bool isSelect) :
      Task(DB_TASK_LEVEL), _loader(loader), _isSelect(isSelect) {
    }
    ;

    void init(const string& dbInstance, const string& sql,
        const string& pattern) {
      _sql << sql;
      _dbInstance = dbInstance;
      _pattern = pattern;
    }
    ;

    virtual void handle();

  private:
    IndexLoaderPtr _loader;
    bool _isSelect;
    Statement _sql;
    string _dbInstance;
    string _pattern;
  };
};

}
;
}
;

#endif /* DBHELPER_H_ */
