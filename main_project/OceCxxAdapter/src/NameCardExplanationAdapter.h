/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file NameCardExplanationAdapter.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/12/01 15:06:21
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __NAMECARDEXPLANATIONADAPTER_H__
#define  __NAMECARDEXPLANATIONADAPTER_H__

#include "AdapterI.h" 
#include "Channel.h"
#include "Singleton.h"
#include "NameCardExplanation.h"

namespace xce
{
	namespace socialgraph
	{
		class NameCardExplanationAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::NameCardExplanationPrx>,
											public MyUtil::Singleton<NameCardExplanationAdapter>
		{
			public:

				/*
				 * @brief:		构造函数
				 * */
				NameCardExplanationAdapter() : 
					MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::NameCardExplanationPrx>("ControllerNameCardExplanation", 120, 1000)
				{}

				Explanation getExplain(::Ice::Int userid, ::Ice::Int targetid, const ::std::string &type);
				
				CommFriendInfoSeq getCommonFriend(::Ice::Int userid, ::Ice::Int targetid, const ::std::string &type);
		};
	}
}












#endif  //__NAMECARDEXPLAINATIONADAPTER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
