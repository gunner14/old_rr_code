/*
 * =====================================================================================
 *
 *       Filename:  DBWriterAdapter.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月17日 15时11分47秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __DB_WRITER_ADAPTER_H__
#define __DB_WRITER_ADAPTER_H__
#include "app/DBWriter/share/DBShare.h"
#include <AdapterI.h>
#include <DBManager.h>
#include "ServiceI.h"
#include <IceUtil/IceUtil.h>
#include <map>
#include <vector>

namespace xce {
namespace dbmanager {

using namespace std;

class DBWriterAdapter : public MyUtil::Singleton<DBWriterAdapter> {
public:
  DBWriterAdapter();
  //下面的三个写函数，返回值的sqlExecuteOk字段为true,表示执行成功
  WriteResultInfoPtr write(const string& dbSrc, const string& pattern, const string& sql);
  WriteResultInfoPtr writeWithFilter(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace,
      const string& businessName, const string& filterSql, const string& key, int op = OP_UPDATE, 
      int expireTime = NEVER_EXPIRE_TIME);
  WriteResultInfoPtr writeWithKeys(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace,
      const string& businessName, const MyUtil::StrSeq& keys, int op = OP_UPDATE, 
      int expireTime = NEVER_EXPIRE_TIME);

private:
  MaintainPrxThreadPtr maintainPrxThread_; 
};

}
}
#endif
