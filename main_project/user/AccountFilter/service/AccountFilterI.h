#ifndef XCE_ACCOUNT_ACCOUNTFILTERI_H_
#define XCE_ACCOUNT_ACCOUNTFILTERI_H_

#include "AccountFilter.h"
#include <vector>
#include <map>
#include <boost/thread/shared_mutex.hpp>
#include "TaskManager.h"
#include "ServiceI.h"

namespace xce {
namespace account {
//用于setData的TaskManager配置
const static int TASK_LEVEL_FILL = 1;
const static int CORE_THREAD_FILL = 1;
const static int MAX_THREAD_FILL = 10;

//服务配置
const static string SERVICE_IDENTITY = "AF";
const static string SERVICE_PREFIX = "AccountFilter";
const static string CONTROLLER_ENDPOINT = "ControllerAccountFilter";

//帐号信息存储数据结构。
//不考虑不同account关联到同一个摘要的情况
struct AccountDataI {
  AccountDataI() {}
  AccountDataI(const AccountDataI& rhs);
  AccountDataI& operator=(const AccountDataI& rhs);
  AccountDataI(const string &account, int id);
  void AddId(int id);
  std::vector<int> ids;
};

/**
 * SHA256(account) => AccountDataI
 *
 * 10M record memory used:
 * 2727616 KB : map<string, AccountDataIPtr>
 * 2251500 KB : map<SHA256[SHA256::HEX_SIZE], AccountDataIPtr>
 * 1964500 KB : map<SHA256[SHA256::RAW_SIZE], AccountDataIPtr>
 * 1303848 KB : map<SHA256, AccountDataI>
 *
 * and I take a simple benchmark for std::map
 * 1000000 KB : map<string(1~10M), int>
 *  458000 KB : map<int, int>
 */
//SHA256键值存储结构
struct SHA256 {
  bool operator==(const SHA256 &rhs) const;
  bool operator<(const SHA256 &rhs) const;
  SHA256& operator=(const SHA256 &rhs);
  SHA256(const SHA256 &rhs);
  SHA256(const string &str);
  const static int HEX_SIZE = 64;
  const static int RAW_SIZE = 32;
  char key[RAW_SIZE];
};
typedef map<SHA256, AccountDataI> DigestMap;

//分桶函数
class BucketManager {
public:
  BucketManager(int size);
  int GetBucket(const string &key) const;
private:
  const int size_;
};

//帐号过滤服务
class AccountFilterI : virtual public AccountFilter, public MyUtil::Singleton<AccountFilterI> {
public:
  AccountFilterI();
  void AddAccount(const string &account, int id, const Ice::Current& = Ice::Current());
  MyUtil::IntSeq GetIdBySHA256(const MyUtil::StrSeq &digests, const Ice::Current& = Ice::Current());
  void setData(const MyUtil::ObjectResultPtr &data, const Ice::Current& = Ice::Current());
  bool isValid(const Ice::Current& = Ice::Current());
  void setValid(bool valid, const Ice::Current& = Ice::Current());
  void AddAccountImpl(const string &account, int id);
private:
  const static int BUCKET_COUNT = 11; //分桶数目为1000
  const BucketManager bucket_manager_;
  boost::shared_mutex mutex_[BUCKET_COUNT];
  DigestMap buckets_[BUCKET_COUNT];
}; 

//预加载Task
class FillTask: virtual public ::MyUtil::Task {
public:
  FillTask(const ::MyUtil::ObjectResultPtr& data) : data_(data) { }
  virtual void handle();
private:
  MyUtil::ObjectResultPtr data_;
};

}} //namespace xce::account

#endif
