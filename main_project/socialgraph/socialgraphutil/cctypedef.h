/******************************************************************************
Function:  c/c++ typedefs of various basic types, stl/boost containers 
Developer: xiongjunwu
Email:     junwu.xiong@renren-inc.com
Date:      2012-03-22
*******************************************************************************/

#ifndef __CC_TYPEDEF_H_
#define __CC_TYPEDEF_H_

#include <vector>
#include <set>
#include <string>

using namespace std;

typedef std::vector<int> IntVec;
typedef std::vector<int>::const_iterator IntVecIter;
typedef std::set<int> IntSet;
typedef std::set<int>::const_iterator IntSetIter;


#endif
