#ifndef _TRIPODCHECKER_ICE
#define _TRIPODCHECKER_ICE

#include <TripodData.ice>

module com {
module renren {
module tripod {


const int FINISHED = 0;
const int NORMAL = 1;

class KeyMsg {
  int flag;
  KeySeq keys;
};

class StatisData {
  long checkedSize; //所有检查过的
  long inconsistentSize; //Cache和DB不一致的
  long missedSize; //Cache中没有DB中有的
  long invalidSize; //Cache中没有DB中有的
  long timeCost; //时间消耗，单位微秒
};

/**
 * @description 控制key的生成和worker的开始和结束
 *
 */
interface CheckerManager {
  /**
   * @description 获取一批key
   * @return key的数组，当忙时返回空数组
   * @param businessId 业务名字
   * @param count key的数量
   */
  KeyMsg getKey(string businessId, int batch);

  void restart(string businessId, string namespaceId, string zkAddress);
  //void destroy(string businessId);

  /**
   * @description 结束后汇总错误信息
   */
  void statis(string businessId, StatisData data);

  bool isValid();
  void setValid(bool value);
};

interface CheckerWorker {
  /**
   * @description 通知某个业务的Check开始
   * @return 是否成功开始
   * @param businessId 业务名字
   */
  void start(string businessId, string namespaceId, string zkAddress);

  /**
   * @description 通知某个业务的Check完成
   * @return 返回不一致数，调用成功后不再向manager发送getKey请求
   * @param businessId 业务名字
   */
  void stop(string businessId);
 
  bool isValid();
  void setValid(bool value);
};

};
};
};

#endif
