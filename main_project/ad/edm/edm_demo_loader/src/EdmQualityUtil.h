/*
 * Util.h
 *
 *  Created on: 2012-09-22
 *      Author: yu.fu 
 */

#ifndef EDM_QUALITY_UTIL_H_
#define EDM_QUALITY_UTIL_H_

#include <Ice/Ice.h>
#include "Date.h"
#include "Singleton.h"
#include <string>
#include <set>
namespace xce{
namespace ad{
	using namespace std;
	using namespace MyUtil;
	Ice::Long TimeForm2Long(string time);
	string GetKey(Ice::Long zone_id, int pos);
	Ice::Long GetTime(int year, int mon, int day, int hour);
	Ice::Long GetTime(MyUtil::Date & day);
	Ice::Long GetDay(int year, int mon, int day);
	Ice::Long GetDay(MyUtil::Date & day);
	string TransformForSql(set<Ice::Long>& gids);
}
}
#endif /* UTIL_H_ */
