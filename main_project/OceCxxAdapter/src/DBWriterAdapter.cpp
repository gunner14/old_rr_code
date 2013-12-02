/*
 * =====================================================================================
 *
 *       Filename:  DBWriterAdapter.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月17日 15时11分54秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "DBWriterAdapter.h"
#include "app/DBWriter/share/DBShare.cpp"

using namespace xce::dbmanager;

DBWriterAdapter::DBWriterAdapter() {
  maintainPrxThread_ = new MaintainPrxThread();
  maintainPrxThread_->start(128*1024).detach();
}

WriteResultInfoPtr DBWriterAdapter::write(const string& dbSrc, const string& pattern, const string& sql) {
  //cout << "[DBWriterAdapter::write] dbSrc = " << dbSrc << ", patter = " << pattern << ", sql = " << sql << endl;
  DBWriterPrx prx = maintainPrxThread_->getDBWriterPrx(dbSrc);
  if (prx == 0) {
    WriteResultInfoPtr resultInfoPtr = new WriteResultInfo();
    string info = "No DBWriter for dbSource = " + dbSrc;
    resultInfoPtr->setInfo(info);
    return resultInfoPtr;
  } else {
    //cout << "[DBWriterAdapter::write] start to execute sql " << endl;
    return new WriteResultInfo(prx->write(dbSrc, pattern, sql));
  }
}

WriteResultInfoPtr DBWriterAdapter::writeWithFilter(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace,
    const string& businessName, const string& filterSql, const string& key, int op, int expireTime) {
  DBWriterPrx prx = maintainPrxThread_->getDBWriterPrx(dbSrc);
  if (prx == 0) {
    WriteResultInfoPtr resultInfoPtr = new WriteResultInfo();
    string info = "No DBWriter for dbSource = " + dbSrc;
    resultInfoPtr->setInfo(info);
    return resultInfoPtr;
  } else {
    return new WriteResultInfo(prx->writeWithFilter(dbSrc, pattern, sql, nameSpace, businessName, filterSql, key, op, expireTime));
  }
}

WriteResultInfoPtr DBWriterAdapter::writeWithKeys(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace,
    const string& businessName, const MyUtil::StrSeq& keys, int op, int expireTime) {
  DBWriterPrx prx = maintainPrxThread_->getDBWriterPrx(dbSrc);
  if (prx == 0) {
    WriteResultInfoPtr resultInfoPtr = new WriteResultInfo();
    string info = "No DBWriter for dbSource = " + dbSrc;
    resultInfoPtr->setInfo(info);
    return resultInfoPtr;
  } else {
    return new WriteResultInfo(prx->writeWithKeys(dbSrc, pattern, sql, nameSpace, businessName, keys, op, expireTime));
  }
}
