#ifndef __AD_PINGSERVER_MANAGER_H__
#define __AD_PINGSERVER_MANAGER_H__

#include <map>
#include <vector>
#include <string>
#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Monitor.h>

#include "TaskManager.h"

using namespace std;
using namespace MyUtil;

namespace xce{
namespace ad{

/*
 *管理信息容器,负责写入和读取
 */
class PingInfoManager : public IceUtil::Shared, public IceUtil::Monitor<IceUtil::Mutex> {
public:
  PingInfoManager(const int threshold, const string& db_source);
  void Put(const vector<string>& messages);
  void Get(vector<string>& messages);

private:
  vector<string> pings_;
  int threshold_;  //容器容量大小阈值
};
typedef IceUtil::Handle<PingInfoManager> PingInfoManagerPtr;

/*
 *负责写入信息到数据库
 */
class WriteTask : public MyUtil::Task {
public:
  WriteTask(PingInfoManager* ping_manager, const string& db_source);
  void Write(const vector<string>& pings);
  void handle();
private:
  PingInfoManager* ping_manager_;
  string DbInstance_;
};

}
}

#endif
