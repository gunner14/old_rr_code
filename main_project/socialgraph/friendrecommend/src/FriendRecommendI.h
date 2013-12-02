/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file FriendRecommendI.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/12/13 13:40:35
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __FRIENDRECOMMENDI_H__
#define  __FRIENDRECOMMENDI_H__

#include <set>
#include <map>
#include <time.h>
#include <pthread.h>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <QueryRunner.h>
#include <ServiceI.h>
#include <FriendRecommend.h>
#include <boost/shared_ptr.hpp>
#include "OceCxxAdapter/src/UserCacheAdapter.h"

namespace xce
{
	namespace socialgraph
	{

		const int TASK_LEVEL_ACCESS = 1;
		const int TASK_LEVEL_UPDATE = 2;

		typedef std::vector<unsigned char> BinaryData;
		typedef std::map<int, xce::usercache::UserCachePtr> UserCacheMap;

		class FriendRecommendI : public FriendRecommend, 
								 virtual public MyUtil::Singleton<FriendRecommendI>
		{
			
			public:

				friend class MyUtil::Singleton<FriendRecommendI>;

				/*
				 * @brief:		构造函数
				 * */
				FriendRecommendI();

				/*
				 * @brief:		析构函数
				 * */
				virtual ~FriendRecommendI();

				/*
				 * @brief:		获得好友推荐的数据
				 * */
				RecommendItemSeq getFriendRecommendation(int userid, int limit, const ::Ice::Current& current);

				/*
				 * @brief:		获得资料推荐的数据
				 * */
				RecommendItemSeq getInfoRecommendation(int userid, int limit, const ::Ice::Current& current);
		};
	}
}





















#endif  //__FRIENDRECOMMENDI_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
