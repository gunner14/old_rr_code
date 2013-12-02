/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file FriendExplanation.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/12/13 14:15:09
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __FRIENDEXPLANATION_H__
#define  __FRIENDEXPLANATION_H__

#include <string>
#include <map>

using namespace std;

inline bool initExplanation(map<int, string> &result)
{
	pair<map<int, string>::iterator, bool> pr;

	pr = result.insert(pair<int, string>(1, "共同年级"));
	if (!pr.second)
		return false;
	pr = result.insert(pair<int, string>(2, "共同高中"));
	if (!pr.second)
		return false;
	pr = result.insert(pair<int, string>(4, "共同学院"));
	if (!pr.second)
		return false;
	pr = result.insert(pair<int, string>(8, "共同大学"));
	if (!pr.second)
		return false;
	pr = result.insert(pair<int, string>(16, "共同家乡市"));
	if (!pr.second)
		return false;
	pr = result.insert(pair<int, string>(32, "共同家乡省"));
	if (!pr.second)
		return false;
	pr = result.insert(pair<int, string>(64, "共同出生日"));
	if (!pr.second)
		return false;
	pr = result.insert(pair<int, string>(128, "共同出生月"));
	if (!pr.second)
		return false;	
	pr = result.insert(pair<int, string>(256, "共同出生年"));
	if (!pr.second)
		return false;

	return true;
}






#endif  //__FRIENDEXPLANATION_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
