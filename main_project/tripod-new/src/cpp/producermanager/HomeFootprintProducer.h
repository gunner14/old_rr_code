/*
 * =====================================================================================
 *
 *       Filename:  HomeFootprintProducer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月15日 14时36分26秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */

#ifndef __HOME_FOOTPRINT_PRODUCER_H__
#define __HOME_FOOTPRINT_PRODUCER_H__

#include "app/FootprintUnite/share/FootprintShare.h"
#include "boost/lexical_cast.hpp"
#include "ServiceI.h"
#include "TripodProducer.h"

namespace xce {
namespace footprint {

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace com::renren::tripod;

class HomeFootprintProducer : virtual public TripodProducer {
public:
  HomeFootprintProducer(int footprintSize);
  virtual ~HomeFootprintProducer() {}
  virtual DataMap create(const KeySeq& keys, bool isMaster);
  virtual KeySeq createHotKeys(const string& beginKey, int limit);
  virtual KeySeq createKeys(const string& beginKey, int limit);
private:
  void fillDataMap(DataMap& dataMap, const Statement& sql, const string& tableName);
  string getTablePrefix();
  int footprintSize_;
};

}
}

#endif
