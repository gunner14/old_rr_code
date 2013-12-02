/*
 * =====================================================================================
 *
 *       Filename:  User2MiniGroupTripodProducer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月13日 18时06分02秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef _USER2MINIGROUP_TRIPOD_PRODUCER_H__
#define _USER2MINIGROUP_TRIPOD_PRODUCER_H__

#include "TripodProducer.h"
#include "boost/lexical_cast.hpp"
#include "ServiceI.h"

namespace xce{
  namespace minigrouptripod {

using namespace std;
using namespace com::renren::tripod;
using namespace MyUtil;
using namespace com::xiaonei::xce;

class User2MiniGroupTripodProducer : public TripodProducer{
public:
  User2MiniGroupTripodProducer(){
  }
  virtual ~User2MiniGroupTripodProducer(){}
  virtual DataMap create(const KeySeq& keys, bool isMaster);
  virtual KeySeq createHotKeys(const string& beginKey, int limit);
  virtual KeySeq createKeys(const string& beginKey, int limit);
private:
  vector<int> parseFrom(const KeySeq& keys);
};


  }
}


#endif
