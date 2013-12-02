/*
 * =====================================================================================
 *
 *       Filename:  UserIdsProducer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月15日 16时01分10秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __USERIDS_PRODUCER_H__
#define __USERIDS_PRODUCER_H__

#include "TripodProducer.h"
#include <string>

namespace com {
namespace renren {
namespace tripod {

using namespace std;

const string DB_DESC_USERPASSPORT = "user_passport";
const string TABLE_USER_PASSPORT = "user_passport";
static const string COLUMN_USERID = "id";

class UserIdsProducer {
public:
  static KeySeq createKeys(const string& invokerInfo, const string& beginKey, int limit);
};

}
}
}

#endif
