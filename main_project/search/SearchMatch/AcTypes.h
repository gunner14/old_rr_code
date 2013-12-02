/*
 * ac_types.h
 *
 *  Created on: 2012-3-21
 *      Author: benjamin
 */

#ifndef AC_TYPES_H_
#define AC_TYPES_H_
#include <map>
#include "string.h"
#include <string>
typedef char BYTE;
static const int THRESHOLD_TO_USE_SPARSE = 3;
static const int MAX_BYTE = 256;

struct NameState
{
	std::string name;
	int st;
};

typedef std::map<int, NameState> OutputMap;
#endif /* AC_TYPES_H_ */
