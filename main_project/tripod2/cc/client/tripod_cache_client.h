#ifndef TRIPOD_CACHE_CLIENT_H_
#define TRIPOD_CACHE_CLIENT_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include "tripod2/cc/core/tripod_client.h"

namespace xce {
namespace tripod {
namespace client {

const int ERROR_COUNT = -1;

class TripodCacheClient : private boost::noncopyable {
 public:
  // 基本接口
  /*
   * 读接口
   * @param key
   * @param value 获取的结果存入value中
   * @return 成功返回true；若获取失败（如key在cache中不存在），返回false
   */
  bool Get(const std::string& key, std::string* value);
  /**
   * 写接口
   * @param key
   * @param value
   * @param secs  key在cache中的过期时间，必须设置，否则认为无效
   * @return
   */
  bool Set(const std::string& key, const std::string& value, int secs = 0);
  /**
   * 批量读接口
   * @param keys
   * @values 获取成功的结果按<key, value>的形式存入values中
   * @return 获取成功的<key, bool> map，其中bool应为true；若某个key获取失败，则不放入该map中
   */
  std::map<std::string, bool> MultiGet(const StringListType& keys, StringMapType* values);
  /**
   * 删除接口
   * @param key
   * @return
   */
  bool Remove(const std::string& key);

  // 计数器接口
  /**
   * 计数器设置接口
   * @key
   * @value
   * @secs  key在cache中的过期时间，必须设置，否则认为无效
   * @return
   */
  bool SetLong(const std::string& key, long value, int secs);
  /**
   * 计数器读接口
   * @param key
   * @param value 获取的结果存入value；若失败，value为ERROR_COUNT
   * @return  若成功返回true；失败返回false
   */
  bool GetLong(const std::string& key, long* value);
  /**
   * 计数器增减接口
   * 1.在调用之前，需要用[计数器设置接口]来初始化指定过期时间
   * 2.如果调用失败，需要用[计数器初始化接口setLong] 重新设值
   * @param key
   * @param step  增量。可以是负数
   * @param value 把添加增量成功后的结果放入value中
   * @return 操作成功返回true，否则返回false
   */
  bool IncLong(const std::string& key, long step, long* value);

  // 有序集合接口
  /**
   * 有序集合初始化写接口
   * @param  key
   * @param  set_with_scores，类型为StringMapType；
   *         其key(须可转化为long)为相应value的score，值其value为实际要追加的元素值
   *         set中元素的排序以其score值为参考(升序排列)
   * @param  secs key的过期时间，必须设置过期时间，不能为永不过期，否则认为无效
   * @return 操作成功返回true，否则返回false
   */
  bool ZSet(const std::string& key, const StringMapType& set_with_scores, int secs);
  /**
   * 有序集合追加元素接口
   * @param key
   * @param  set_with_scores，类型为StringMapType；
             其key(须可转化为long)为相应value的score，值其value为实际要追加的元素值
   *         set中元素的排序以其score值为参考(升序排列)
   * @return  若当前key不在cache中(可能因为a:还未对此key设置过value值;b:该key在cache中已过期)，则返回false
   */
  bool ZAdd(const std::string& key, const StringMapType& set_with_scores);
  /**
   * 有序集合删除接口
   * @param key
   * @param begin 要删除范围的起始位置，0表示有序集合中的首元素
   * @param limit 要删除元素的个数
   * @return 操作成功返回true，否则返回false
   */
  bool ZRemRangeByRank(const std::string& key, int begin, int limit);
  
  /**
   * 有序集合删除接口
   * @param key
   * @param min 要删除的score（即用于比较的值的）最小值 
   * @param max 要删除的score范围的最大值
   * @return 操作成功返回true，否则返回false
   */
  bool ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max);
 
  /**
   * 有序集合获取接口
   * @param key
   * @param begin 要获取元素范围的起始位置，0表示有序集合中的首元素
   * @param limit 要获取元素的个数
   * @param zset_value  获取的value列表存入zset_value中
   * @return 操作成功返回true，否则返回false
   */
  bool ZRange(const std::string& key, int begin, int limit, StringListType* zset_value);

  /**
   * 列表获取接口
   * @param key
   * @param begin 要获取的范围其实位置，0表示有序集合中的首元素
   * @param limit 要获取元素的个数
   * @param value 获取到的list列表
   * @return 操作成功返回true，否则返回false
   */
  bool GetList(const std::string& key, int begin, int limit, StringListType* value);

  /**
   * 列表写入接口
   * @param key
   * @param value 需要写入cache中的列表值，为覆盖方法，
   *              塞入后将覆盖原来cache存在的list数值
   * @return 操作成功返回true，否则返回false
   */
  bool SetList(const std::string& key, const StringListType& value, int secs);

  virtual ~TripodCacheClient();
 private:
  TripodCacheClient(const std::string& ns, const std::string& biz);
  friend class TripodCacheClientFactory;
  boost::shared_ptr<TripodClient> client_;
};

typedef boost::shared_ptr<TripodCacheClient> TripodCacheClientPtr;

}}} 
#endif


