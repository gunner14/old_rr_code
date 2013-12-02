/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file FriendRecommendAdapter.h
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
#include "FriendRecommend.h"

namespace xce
{
	namespace socialgraph
	{
		class FriendRecommendAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::FriendRecommendPrx>,
											public MyUtil::Singleton<FriendRecommendAdapter>
		{
			public:

				/*
				 * @brief:		构造函数
				 * */
				FriendRecommendAdapter() : 
					MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::FriendRecommendPrx>("ControllerFriendRecommend", 120, 1000)
				{}

				RecommendItemSeq getFriendRecommendation(int userid, int limit);

				RecommendItemSeq getInfoRecommendation(int userid, int limit);
		};
	}
}












#endif  //__NAMECARDEXPLAINATIONADAPTER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
