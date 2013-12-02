/*************************************************
 *
 *  @Copyright    Renren Inc.
 *  @Brief        AdUseCache模块的服务接口层
 *  @Filename     AdUserCacheLoaderI.h
 *  @Author       xiaoqiang.zhu
 *  @Email        xiaoqiang.zhu@renren-inc.com
 *  @Date         2012-09-09
 *  @History
 *
 *************************************************/


#ifndef ADUSERCACHELOADERI_H_
#define ADUSERCACHELOADERI_H_

#include "AdUserCache.h"
#include "AdUserDataWrapper.h"
#include "FeedMemcProxy/client/user_profile_client.h"

#include <iostream>
#include <string>
#include <vector>

#include "Singleton.h"
#include <QueryRunner.h>
#include "ServiceI.h"

using namespace std;
using namespace MyUtil;

namespace xce {
namespace ad {


/*
 *  @Class    AdUserCacheLoaderI
 *  @Brief    封装了所有的AdUserCache服务对外提供的接口,是ICE接口AdUserCacheLoader的servant类
 *  @Author   xiaoqiang.zhu
 *  @Date     2012-09-09
 *
 */
class AdUserCacheLoaderI: virtual public AdUserCacheLoader, virtual public MyUtil::Singleton<AdUserCacheLoaderI> {
public:
  //基础函数
  AdUserCacheLoaderI(){
    initialize();
  }

  virtual ~AdUserCacheLoaderI(){}

  /*
   * @Brief:                   初始化函数，把与uid独立的公共信息在服务起来时调用时预加载到内存
   * @Return        0          初始化正常
   *                !0         初始化异常
   */
  int initialize();

  /*
   * @Brief:                   线上查询接口：查询单个用户的定向信息，返回结果是序列化的字符串
   *                               具体过程：
   *                                   首先查询二级缓存DB ad_user_info，如果存在该用户，则取出并更新一级缓存memcache；
   *                                   否则，生成一个缺省的用户定向信息，同时将该用户加入到查询队列
   *                                   查询队列异步地从源DB加载用户的基础信息，进行推断后获得用户的定向信息，
   *                                   然后将该信息更新到二级缓存DB ad_user_info和一级缓存memcache
   *                                   最后将用户的定向信息进行序列化，返回序列化后的字符串
   * @Param[in]:     userId    待查询的单用户id
   * @Return                   序列化的用户定向信息UserProfile字符串
   */
  virtual string GetById(int userId, const Ice::Current& = Ice::Current());

  /*
   * @Brief:                   线下加载接口：批量从二级缓存DB ad_user_info加载数据，更新一级缓存memcache
   * @Param[in]:     userIds   待加载的批量用户id的列表
   * @Return         0         正常加载
   *                 !0        异常加载
   */
  virtual int LoadFromAduserinfoDBByIdSeq(const vector<int>& userIds, const Ice::Current& = Ice::Current());

  /*
   * @Brief:                   线下加载接口：单用户从二级缓存DB ad_user_info加载数据，更新一级缓存memcache
   * @Param[in]:     userId    待加载的单用户id
   * @Return         0         正常加载
   *                 !0        异常加载
   */
  virtual int LoadFromAduserinfoDBById(int userId, const Ice::Current& = Ice::Current());

  /*
   * @Brief:                   线下加载接口：批量从源DB加载用户基础信息，经过逻辑推断，获得用户的定向信息，
   *                                         同时更新二级缓存DB ad_user_info和一级缓存memcache
   * @Param[in]:     userIds   待加载的批量用户id的列表
   * @Return         0         正常加载
   *                 !0        异常加载
   */
  virtual int LoadFromSourceDBByIdSeq(const vector<int>& userIds, const Ice::Current& = Ice::Current());

  /*
   * @Brief:                   线下加载接口：单用户从源DB加载用户基础信息，经过逻辑推断，获得用户的定向信息
   *                                         同时更新二级缓存DB ad_user_info和一级缓存memcache
   * @Param[in]:     userId    待加载的单用户id
   * @Return         0         正常加载
   *                 !0        异常加载
   */
  virtual int LoadFromSourceDBById(int userId, const Ice::Current& = Ice::Current());

  /*
   * @Brief:                                线下修改接口：批量将手动设置的用户定向信息插入到二级缓存DB ad_user_info和一级缓存memcache
   *                                                      同时将修改的用户id列表写入到Debug DB
   * @Param[in]:     userProfileStrings     批量用户定向信息UserProfile的列表，经过了序列化
   * @Return         0                      正常加载
   *                 !0                     异常加载
   */
//  virtual int SetSeq(const vector<UserProfile>& userinfoseq);

  /*
   * @Brief:                                线下修改接口：将手动设置的单个用户定向信息插入到二级缓存DB ad_user_info和一级缓存memcache
   *                                                      同时将修改的用户id写入到Debug DB
   * @Param[in]:     userProfileString      单个用户的定向信息UserProfile，经过了序列化
   * @Return         0                      正常
   *                 !0                     异常
   */
//  virtual int Set(const string& userProfileString, const Ice::Current& = Ice::Current());
  virtual int Set(const UserInfo& userinfo, int& flag, const Ice::Current& = Ice::Current());

  /*
   * @Brief:                   线下修改接口：批量从源DB加载用户基础信息，经过逻辑推断，获得用户的定向信息，
   *                                         同时更新二级缓存DB ad_user_info和一级缓存memcache
   *                                         并且将这批用户的id从Debug DB中擦除掉
   * @Param[in]:     userIds   待恢复的批量用户id的列表
   * @Return         0         正常
   *                 !0        异常
   */
//  virtual int ResetSeq(const vector<int>& userIds);

  /*
   * @Brief:                   线下修改接口：从源DB加载单个用户的基础信息，经过逻辑推断，获得用户的定向信息，
   *                                         同时更新二级缓存DB ad_user_info和一级缓存memcache
   *                                         并且将这个用户的id从Debug DB中擦除掉
   * @Param[in]:     userIds   待恢复的用户id的列表
   * @Return         0         正常
   *                 !0        异常
   */
  virtual int Reset(int userId, const Ice::Current& = Ice::Current());

   /*
   * @Brief:                   线下修改接口：查询Debug DB，获取手动设置过的用户的id列表
   * @Param[out]:    debugList 待返回的修改过信息的用户id列表
   * @Return         0         正常
   *                 !0        异常
   */
//  virtual int GetDebuggerList(vector<int>& debugList, const Ice::Current& = Ice::Current());
  virtual int GetDebuggerList(vector<UserInfo>& debugList, const Ice::Current& = Ice::Current());
  virtual int GetByIdOffline(int userId, int& flag, UserInfo& userinfo, const Ice::Current& = Ice::Current());

   /*
   * @Brief:                   线下推断接口：根据用户所有好友的基础信息，推断用户的定向信息，并且写入Inference DB 
   * @Param[in]:     userId    待推断的单用户id
   * @Return         0         正常
   *                 !0        异常
   */
  virtual int OfflineInference(int userId, const Ice::Current& = Ice::Current());

   /*
   * @Brief:                   线下设置用户性别接口：根据线下的算法推断结果，将推断出来的用户性别写入Inference DB 
   * @Param[in]:     userId    待推断的单用户id
   * @Param[in]:     gender    1:男生 0:女生
   * @Return         0         正常
   *                 !0        异常
   */
  
  virtual int SetOfflineGender(int userId, int gender, const Ice::Current& = Ice::Current()); 

};



/*
 *  
 *  @Class    AdUserPublicInfoResetTimer
 *  @Brief    定时加载AdUserPublicInfo
 *  @Author   xiaoqiang.zhu
 *  @Date     2012-09-09
 *
 */
class AdUserPublicInfoResetTimer: virtual public MyUtil::Timer {
public:
  /*
   * @Brief:         延迟启动的任务，实际上市继承自IceUtil::TimerTask类
   * @Param[in]:     reset_interval_   延迟启动的时间，单位为毫秒
   */
  AdUserPublicInfoResetTimer(int reset_interval_):Timer(reset_interval_){
     ;
  }

  /*
   * @Brief:         默认执行函数，会被TaskManager默认调用
   */
  virtual void handle();
};


}//end of namespace ad
}//end of namespace xce


#endif /* ADUSERCACHELOADERI_H_ */
