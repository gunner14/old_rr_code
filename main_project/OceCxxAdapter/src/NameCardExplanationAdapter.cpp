/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file NameCardExplanationAdapter.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/12/01 15:06:26
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include "NameCardExplanationAdapter.h"

using namespace xce::socialgraph;

Explanation NameCardExplanationAdapter::getExplain(::Ice::Int userid, ::Ice::Int targetid, const ::std::string &type)
{
	return getProxy(userid)->getExplain(userid, targetid, type);
}

CommFriendInfoSeq NameCardExplanationAdapter::getCommonFriend(::Ice::Int userid, ::Ice::Int targetid, const ::std::string &type)
{
	return getProxy(userid)->getCommenFriend(userid, targetid, type);
}















/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
