/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file FriendRecommendAdapter.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/12/01 15:06:26
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include "FriendRecommendAdapter.h"

using namespace xce::socialgraph;

RecommendItemSeq getFriendRecommendation(int userid, int limit)
{
	return getProxy(userid)->getFriendRecommendation(userid, limit);
}

RecommendItemSeq getInfoRecommendation(int userid, int limit)
{
	return getProxy(userid)->getInfoRecommendation(userid, limit);
}
















/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
