/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file NameCardExplanationI.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/11/30 13:32:59
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  _NAMECARDEXPLAINATIONI_H__
#define  _NAMECARDEXPLAINATIONI_H__

#include <map>
#include <vector>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <QueryRunner.h>
#include <IceExt/src/ServiceI.h>
#include <NameCardExplanation.h>
#include <boost/shared_ptr.hpp>
#include "OceCxxAdapter/src/UserCacheAdapter.h"

namespace xce
{
	namespace socialgraph
	{
		enum SocialFeedAdapter
		{
			SF_UserUrlTripodAdapter,
			SF_UserNameTripodAdapter,
			SF_UserUniversityTripodAdapter,
			SF_UserWorkplaceTripodAdapter
		};

		typedef vector<unsigned char> BinaryData;  
		typedef std::map<int, xce::usercache::UserCachePtr> UserCacheMap;

		class NameCardExplanationI : public NameCardExplanation, 
								 virtual public MyUtil::Singleton<NameCardExplanationI>
		{
			public:

				friend class MyUtil::Singleton<NameCardExplanationI>;

				/*
				 * @brief:		构造函数
				 * */
				NameCardExplanationI();

				/*
				 * @brief:		析构函数
				 * */
				virtual ~NameCardExplanationI();

				/*
				 * @brief:		获取推荐信息的名片解释
				 * */
				virtual Explanation getRecommendExplanation(::Ice::Int userid, const ::std::string &targetid, 
																	const ::std::string &type, const ::Ice::Current& current);

				/*
				 * @brief:		处理数据函数
				 * */
				virtual CommFriendInfoSeq getCommonFriend(::Ice::Int userid, ::Ice::Int targetid, 
													const ::std::string &type, const ::Ice::Current& current);
		};
	}
}



















#endif  //_NAMECARDEXPLAINATIONI_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
