/*************************************************
 *
 *  @Copyright    Renren inc.
 *  @Brief        AdUseCacheLoader模块的查询队列接口层
 *  @Filename     AdUserList.h
 *  @Author       chaojie.liu
 *  @Email        chaojie.liu@renren-inc.com
 *  @Date         2012-09-08
 *  @History
 *
 *************************************************/

#ifndef __ADUSER_LIST_H__
#define __ADUSER_LIST_H__
#include <map>
#include <set>
#include <list>
#include "ServiceI.h"
#include "Singleton.h"
#include <IceUtil/Monitor.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Handle.h>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Thread.h>
#include <QueryRunner.h>
using namespace std;


namespace xce {
namespace ad {

//查询队列类
class AdUserList : public IceUtil::Monitor<IceUtil::Mutex> {
  public:
    void Put(int &uid);
    int Get(); 
  private:
    list<int> q_;
};

//异步更新用户信息类
class AdUserUpdate:public IceUtil::Thread, virtual public ::MyUtil::Singleton<AdUserUpdate> {
  public:
    int Init();
    virtual void run();
    void PutId(int uid);
  private:
    AdUserList user_list_;
};  

}
}

#endif
