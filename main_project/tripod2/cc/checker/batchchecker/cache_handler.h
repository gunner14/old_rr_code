/** 
 * 用于增节Cache节点后批量删除失效Cache或加载需要加载的Cache
 * xiaofeng.liang@renren-inc.com
 */
#ifndef XCE_TRIPOD_CHECKER_CACHE_HANDLER_H_
#define XCE_TRIPOD_CHECKER_CACHE_HANDLER_H_

#include <vector>

#include "tripod2/cc/core/static_cache_proxy.h"
#include "pc_model.h"
#include "config_handler.h"

using namespace xce::util;
using namespace xce::tripod;

namespace xce {
namespace tripod {
namespace checker {

//CacheHandler 
//--------------------------------------------------------------
class CacheHandler : public ConsumerThread<int> {
public:
  CacheHandler(Queue<int> *queue, const int batch, const string &ns, const string &biz,
      const string &xml_path_old, const string &xml_path_new, const int method);

  virtual bool consum(vector<int> ids);

  const static int REMOVE = 0;
  const static int LOAD = 1;
  const static int SET = 2;
private:
  string _ns;
  string _biz;
  int _method;
  Locator *_locator_old, *_locator_new;
  StaticCacheProxy *_proxy_old, *_proxy_new;
};

}}}// namespace xce::tripod

#endif
