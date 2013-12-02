/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file socialfeedloader.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/10/19 10:34:06
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __SOCIALFEEDLOADER_H__
#define  __SOCIALFEEDLOADER_H__

#include <set>
#include <map>
#include <time.h>
#include <pthread.h>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <QueryRunner.h>
#include <ServiceI.h>
#include <SocialFeedLoader.h>
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

		typedef struct UserInfo
		{
			time_t									lastrecommendtime;
			std::set<int>							user_black_set;
		};

		enum SocialFeedAdapter
		{
			SF_UserUrlTripodAdapter,
			SF_UserNameTripodAdapter,
			SF_UserUniversityTripodAdapter,
			SF_UserWorkplaceTripodAdapter
		};

		class SocialFeedLoaderI : public SocialFeedLoader, 
								 virtual public MyUtil::Singleton<SocialFeedLoaderI>
		{
			
			public:

				friend class MyUtil::Singleton<SocialFeedLoaderI>;

				/*
				 * @brief:		构造函数
				 * */
				SocialFeedLoaderI();

				/*
				 * @brief:		析构函数
				 * */
				virtual ~SocialFeedLoaderI();

				/*
				 * @brief:		处理数据函数
				 * */
				virtual void handler(int userid, const ::Ice::Current& current);

		};

		class AutoRWlock
		{

			public:
		
				AutoRWlock(pthread_rwlock_t *lock)
				{   
					p_lock = lock;
					pthread_rwlock_wrlock(p_lock);
				}   

				/*
				 * @brief:构造函数
				 * */
				~AutoRWlock()
				{   
					pthread_rwlock_unlock(p_lock);
				}   

			private:

				pthread_rwlock_t *p_lock;
		};
	}
}














#endif  //__SOCIALFEEDLOADER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
