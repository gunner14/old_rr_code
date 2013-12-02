/*
 * =====================================================================================
 *
 *       Filename:  HomeUserVisitCountPreloader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年11月17日 17时35分19秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __HOME_USERVISITCOUNT_PRELOADER_H__
#define __HOME_USERVISITCOUNT_PRELOADER_H__
#include <FootprintUnite.h>
#include "FootprintShare.h"
#include <boost/lexical_cast.hpp>
#include <Monitor/client/service/ServiceMonitor.h>
#include "ActiveTrackAdapter.h"
#include "app/Utils/src/DBUtils.h"

#include "QueryRunner.h"
#include "GeneralPreloader/src/GeneralPreloaderFactory.h"
#include "GeneralPreloader/src/GeneralPreloaderProducer.h"

namespace xce {
namespace footprint {

using namespace xce::utils;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::footprint;
using namespace xce::log;

class HomeUserVisitCountPreloader : virtual public Ice::Object, virtual public MyUtil::Singleton<HomeUserVisitCountPreloader> {
public:
  HomeUserVisitCountPreloader() {}
  virtual ~HomeUserVisitCountPreloader() {}
};

class HomeUserVisitCountPreloaderFactory : virtual public xce::generalpreloader::IdListFactory {
public:
  virtual MyUtil::ObjectResultPtr createWithLongIds(const MyUtil::LongSeq& ids);
  //下面这个函数不会被调用到的,但是必须实现，否则编译不过
  virtual MyUtil::ObjectResultPtr create(const MyUtil::IntSeq& ids) {
    return 0;
  }
};

class BatchHomeUserVisitCountIdProducer : public virtual xce::generalpreloader::Producer {
public:
  virtual std::vector<long> produceLongIds(long beginId, int load_batch_size, long& maxId, int& tableMod);
  //下面这个函数是不会被调到的,但是必须实现，否则编译不过
  virtual std::vector<int> produce(int, int, int&, int&) {
    vector<int> ids;
    return ids;
  }
};
MyUtil::ObjectResultPtr HomeUserVisitCountPreloaderFactory::createWithLongIds(const MyUtil::LongSeq& ids) {
  MCE_INFO("[HomeUserVisitCountPreloaderFactory::createWithLongIds] ids.size = " << ids.size());
  MyUtil::ObjectResultPtr result = new MyUtil::ObjectResult;
  if (ids.empty()) {
    return result;
  }
  vector<UserVisitCountInfoPtr> ptrs;
  UserVisitCountInfoResultHandlerI handler(ptrs);
  Statement sql;
  sql << "select " << COLUMN_ID << ", " << COLUMN_VIEWCOUNT << " from " << TABLE_VIEW_COUNT
    << " where " << COLUMN_ID << " in " << DBUtils::getIdsStr<long>(ids);
  com::xiaonei::xce::QueryRunner(DESC_HOME_FOOTPRINT_HEAD, com::xiaonei::xce::CDbRServer).query(sql, handler);
  MyUtil::ObjectResultPtr resultPtr = UserVisitCountInfo::changeToUserVisitCountPtrMap(ptrs);
  MCE_INFO("[HomeUserVisitCountPreloaderFactory::createWithLongIds] ids.size = " << ids.size() << " finished");
  return resultPtr;
}

std::vector<long> BatchHomeUserVisitCountIdProducer::produceLongIds(long beginId, int load_batch_size, long& maxId, int& tableMod) {
  vector<long> ids;
  MCE_INFO("[BatchHomeUserVisitCountIdProducer::produce] beginId = " << beginId << ", maxId = " << maxId << ", tableMod = " << tableMod);
  com::xiaonei::xce::Statement sql;
  sql << "SELECT distinct(" << COLUMN_ID << ") FROM " << TABLE_VIEW_COUNT << " WHERE " << COLUMN_ID << " >= "
    << beginId << " order by " << COLUMN_ID << " limit " << load_batch_size;
  BatchIdResultHandler<long> handler(COLUMN_ID, ids);
  com::xiaonei::xce::QueryRunner(DESC_VIEW_COUNT, com::xiaonei::xce::CDbRServer).query(sql, handler);
  if (!ids.empty()) {
    maxId = *(ids.end() - 1);
  }
  return ids;
}

}
}

#endif
