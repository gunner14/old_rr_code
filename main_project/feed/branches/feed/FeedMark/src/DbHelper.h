/*
 * DbHelper.h
 *
 *  Created on: Dec 24, 2009
 *      Author: antonio
 */

#ifndef FEEDMARK_DBHELPER_H_
#define FEEDMARK_DBHELPER_H_

#include "QueryRunner.h"
#include "Singleton.h"
#include "FeedMarkI.h"
namespace xce {
namespace feed {

using namespace com::xiaonei::xce;
using namespace MyUtil;

const string DB_INSTANCE = "feed_db_feednews_mark";
const string TAB_NAME = "feednews_mark_";

#define COL_UID "uid"
#define COL_FRIEND "friend"
#define COL_MARK "mark"
#define COL_TIME "time"

const int DBCluster = 100;

class DbHelper: public MyUtil::Singleton<DbHelper> {
public:;
  void MarkFeed(int uid, int friendid, bool mark);
	UserMarkStatusPtr GetFeedMark(int uid);
private:
	
  string GetTableName(int uid) {
    ostringstream tab;
		tab << TAB_NAME << uid % DBCluster;
		return tab.str();
	}

};


}
;
}
;

#endif /* DbHelper_H_ */
