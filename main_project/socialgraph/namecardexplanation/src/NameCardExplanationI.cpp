/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file NameCardExplanationI.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/11/30 13:33:08
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <boost/lexical_cast.hpp>

#include "album_util.h"
#include "item.pb.h"
#include "NameCardExplanationI.h"
#include "FriendExplanation.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "Singleton.h"

#include "tripod2/cc/core/tripod_client.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"

#include "OceCxxAdapter/src/UserUrlTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphFFUCRTripodAdapter.h"
#include "OceCxxAdapter/src/UserBasicAdapter.h"
#include "OceCxxAdapter/src/BuddyFriendsAdapter.h"
#include "OceCxxAdapter/src/UserNameTripodAdapter.h"
#include "OceCxxAdapter/src/UserWorkplaceTripodAdapter.h"
#include "OceCxxAdapter/src/UserUniversityTripodAdapter.h"

using namespace MyUtil;
using namespace xce::usercache;
using namespace xce::serverstate;
using namespace com::xiaonei::xce;
using namespace xce::tripod;
using namespace xce::socialgraph;
using namespace xce::adapter;
using namespace xce::adapter::userbasic;
using namespace xce::adapter::username;
using namespace xce::adapter::userurl;
using namespace xce::userbase;
using namespace xce::buddy::adapter;

static std::string loadfromusercache(std::string key, int userid, SocialFeedAdapter adapter)
{
	string emptystr;
	switch (adapter)
	{
		case SF_UserUrlTripodAdapter:
			{
				if (-1 != key.find("tinyimg"))
				{
					UserUrlInfoPtr urlPtr;
					string url;
					try
					{
						urlPtr = UserUrlTripodAdapter::instance().Get(userid);
						url = urlPtr->tinyUrl();
					}
					catch(...)
					{
						MCE_WARN("UserUrlTripodAdapter::instance() failed.");
						return emptystr;
					}
					return album_util::AlbumUtil::getPhotoURL(url.c_str());
				}
				else if (-1 != key.find("mainimg"))
				{
					UserUrlInfoPtr urlPtr;
					string url;
					try
					{
						urlPtr = UserUrlTripodAdapter::instance().Get(userid);
						url = urlPtr->mainUrl();
					}
					catch(...)
					{
						MCE_WARN("UserUrlTripodAdapter::instance() failed.");
						return emptystr;
					}
					return album_util::AlbumUtil::getPhotoURL(url.c_str());
				}
				else
				{
					MCE_WARN("Wrong Key : " << key);
				}
				break;
			}
		case SF_UserNameTripodAdapter:
			{
				UserNameInfoPtr namePtr;
				try
				{
					namePtr = UserNameTripodAdapter::instance().Get(userid);
				}
				catch(...)
				{
					MCE_WARN("UserNameTripodAdapter::instance() failed.");
					return emptystr;
				}
				return namePtr->name();
			}
		case SF_UserUniversityTripodAdapter:
			{
				//最新的学校
				std::string value;
				try
				{
					mop::hi::svc::model::UniversityInfoSeq ptrme = UserUniversityTripodAdapter::instance().getUniversityInfoSeq(userid);
					int  max_enrollYear1 = 0;
					for (mop::hi::svc::model::UniversityInfoSeq::const_iterator it1 = ptrme.begin(); it1!= ptrme.end(); it1++)
					{
						if ((*it1)->enrollYear > max_enrollYear1)
						{
							value = (*it1)->universityName;
							max_enrollYear1 = (*it1)->enrollYear;
						}   
					}
				}
				catch(...)
				{
					MCE_WARN("UserUniversityTripodAdapter::instance() failed.");
					return emptystr;
				}
				return value;
			}
		case SF_UserWorkplaceTripodAdapter:
			{
				//最新的公司
				std::string value;
				try
				{
					mop::hi::svc::model::WorkplaceInfoSeq wk = UserWorkplaceTripodAdapter::instance().getWorkplaceInfoSeq(userid);
					int max_joinYear = 0;
					for (mop::hi::svc::model::WorkplaceInfoSeq::const_iterator it = wk.begin(); it != wk.end(); it++)
					{
						if ((*it)->joinYear > max_joinYear)
						{ 
							value = (*it)->workplaceName;
							max_joinYear = (*it)->joinYear;
						}       
					}
				}
				catch(...)
				{
					MCE_WARN("UserWorkplaceTripodAdapter::instance() failed.");
					return emptystr;
				}
				return value;
			}
		default:
			break;
	}
}


/*
 * @brief:		获取共同好友
 * */
static bool getcommonfriend(int userid, int targetid, CommFriendInfoSeq &commonfriendinfoseq)
{
	if (userid <= 0 || targetid <= 0)
	{
		MCE_ERROR("fail to get valid argvs.");
		return false;
	}

	MyUtil::IntSeq  com_friend_ids;
	try
	{
		com_friend_ids = BuddyFriendsCacheAdapter::instance().getCommonFriends(userid, targetid);
		if (com_friend_ids.empty())
		{
			MCE_ERROR("get explanation for ffucr failed, userid : " << userid << ", targetid : " << targetid);
			return false;
		}
	}
	catch(...)
	{
		MCE_ERROR("BuddyFriendsCacheAdapter::instance().getCommonFriends failed for userid : " << userid << ", targetid : " << targetid);
		return false;
	}

	int com_num = 0;
	MyUtil::StrSeq com_friend_img;
	try
	{
		vector<int>::iterator iter = com_friend_ids.begin();
		while (iter != com_friend_ids.end())
		{
			CommonFriendInfo comfrienditem;
	
			//get tinyimg
			string tinyimg = loadfromusercache("tinyimg", *(iter), SF_UserUrlTripodAdapter);
			if (tinyimg.empty())
			{
				com_friend_ids.erase(iter++);
				continue;
			}
	
			//get name
			string name = loadfromusercache("name", *(iter), SF_UserNameTripodAdapter);
			if (name.empty())
			{
				com_friend_ids.erase(iter++);
				continue;
			}

			comfrienditem.id = *(iter);
			comfrienditem.head = tinyimg;
			comfrienditem.name = name;
			
			commonfriendinfoseq.push_back(comfrienditem);
		
			iter ++;
		}
	}
	catch (...)
	{
		MCE_ERROR("Get common friend tinyurl failed for userid : " << userid << ", targetid : " << targetid);
		return false;
	}

	return true;
}

/*
 * @brief:		获取好友簇数据
 * @param:		[in]userid
 * @return:		RecommendItemSeq
 * */
static RecommendItemSeq getfriendclusterdata(int userid)
{
	RecommendItemSeq item_list;                                                     //result of recommendation
	string value;
	BinaryData raw_data;
	int rank = 0;

	string key = boost::lexical_cast<std::string>(userid);
	bool fetch;
	try
	{
		fetch = SocialGraphFFUCRTripodAdapter::instance().Get(key, &value);
	}
	catch(std::exception e)
	{
		MCE_ERROR("SocialGraphFFUCRTripodAdapter::instance().Get Failed : " << userid);
		return item_list;
	}

	if (!fetch) 
	{
		MCE_ERROR("SocialGraphFFUCRTripodAdapter::instance().Get Failed : " << userid);
		return item_list;
	}   

	if (!value.empty())
	{
		BinaryData raw_data(value.begin(), value.end());
		Items raw_items;
		if (!raw_items.ParseFromArray(raw_data.data(), raw_data.size())) 
		{
			MCE_ERROR("ParseFromArray Failed : " << userid);
			return item_list;
		}   

		BOOST_FOREACH(const Item& it, raw_items.items()) 
		{
			RecommendItem r;
			r.id = it.id();
			r.count = it.field();
			BOOST_FOREACH(int i, it.fields()) 
			{
				r.commons.push_back(i);
			}   
			r.weight = (Ice::Long)0.0l;
			r.type = "FFUCR";

			for (int i=0; i < it.messages_size(); i++)
			{
				r.explanations.push_back(it.messages(i));
			}
			item_list.push_back(r);
		}   

		MCE_DEBUG("getfriendclusterdata success : " << userid);
	}
	
	return item_list;
}

static void getrealexplanation(UserBasicDataPtr userbasicinfo, int tag, string university, string &explanation)
{
	switch (tag)
	{
		case 1:
			explanation += boost::lexical_cast<std::string>(userbasicinfo->univYear); 
			break;
		case 2:
			explanation += userbasicinfo->highSchool;
			break;
		case 4:
			explanation += userbasicinfo->department;
		case 8:
			explanation += university;
			break;
		default:
			break;
	}
}

static bool getexplainfortype(int userid, int targetid, string type, Explanation &explanation)
{

	UserBasicDataPtr userbasicinfo;
	try
	{
		userbasicinfo = UserBasicAdapter::instance().getUserBasic(targetid);
		if (0 == userbasicinfo)
		{
			MCE_ERROR("UserBasicAdapter::instance().getUserBasic failed for :  " << targetid);
			return false;
		}
	}
	catch(...)
	{
		MCE_ERROR("UserBasicAdapter::instance().getUserBasic failed for :  " << targetid);
		return false;
	}

	string company = loadfromusercache("company", targetid, SF_UserWorkplaceTripodAdapter);
	string university = loadfromusercache("university", targetid, SF_UserUniversityTripodAdapter);

	if (-1 != type.find("FFUCR"))
	{
		RecommendItemSeq fcdata = getfriendclusterdata(userid);
		for (RecommendItemSeq::const_iterator iter = fcdata.begin(); iter != fcdata.end(); ++iter)
		{
			//find the targetid in fcdata which is the recommend friedn cluster information
			if (iter->id == targetid)
			{
				//find the explanation from explanations
				//and make the real explanation
				int ex_num = atoi(iter->explanations[0].c_str());
				for (int pos = 0; pos < 30; pos++)
				{
					int temp = ex_num & (1 << pos);
					if (temp)
					{
						getrealexplanation(userbasicinfo, temp, university, explanation.region);
					}
				}
			}
		}
	}
	if (-1 != type.find("FOF") || -1 != type.find("FFW"))
	{
		if (company.empty())
		{
			if (university.empty())
			{
				if (userbasicinfo->homeProvince.empty())
				{
					explanation.region = "";
				}
				else
				{
					explanation.region = userbasicinfo->homeProvince;
				}
			}
			else
			{
				explanation.region = university;
			}
		}
		else
		{
			explanation.region = company;
		}
	}
	if (-1 != type.find("NO_RECOMMEND"))
	{
		if (company.empty())
		{
			if (university.empty())
			{
				if (userbasicinfo->homeProvince.empty())
				{
					explanation.region = "";
				}
				else
				{
					explanation.region = userbasicinfo->homeProvince;
				}
			}
			else
			{
				explanation.region = university;
			}
		}
		else
		{
			explanation.region = company;
		}
	}
	
	MCE_INFO("explanation.region : " << explanation.region);
	return true;
}

static bool getbasicinfo(int userid, int targetid, string type, Explanation &explanation)
{
	string mainimg;
	try
	{
		mainimg = loadfromusercache("mainimg", targetid, SF_UserUrlTripodAdapter); 
	}
	catch (...)
	{
		MCE_ERROR("UserUrlTripodAdapter::instance.Get Failed.");
		return false;
	}

	string tinyimg;
	try
	{
		tinyimg = loadfromusercache("tinyimg", targetid, SF_UserUrlTripodAdapter); 
	}
	catch (...)
	{
		MCE_ERROR("UserUrlTripodAdapter::instance.Get Failed.");
		return false;
	}

	string name;
	try
	{
		name = loadfromusercache("name", targetid, SF_UserNameTripodAdapter);
	}
	catch (...)
	{
		MCE_ERROR("UserNameTripodAdapter::instance.Get Failed.");
		return false;
	}

	//get basic information
	explanation.userId = userid;
	explanation.ownerId = targetid;
	explanation.name = name; 
	explanation.head = mainimg;
	explanation.tinyHead = tinyimg;
	explanation.isFriend = false;

	//get common friend information
	if (!getcommonfriend(userid, targetid, explanation.relationInfo))
	{
		MCE_ERROR("get common friend failed.");
		return false;
	}
	
	//get recommend explanation information
	int comm_num = explanation.relationInfo.size();
	string value = boost::lexical_cast<std::string>(comm_num);
	explanation.relation = value + "个共同好友";

	return true;
}

/*
 * @brief:		产生推荐解释
 * */
static bool generateinfo(int userid, int targetid, string type, Explanation &explanation)
{
	if (userid <= 0 || targetid <= 0)
	{
		MCE_ERROR("fail to get valid argvs.");
		return false;
	}

	//common information for targetid
	//the friend cluster
	if (!type.empty())
	{
		if (!getbasicinfo(userid, targetid, type, explanation)
				|| !getexplainfortype(userid, targetid, type, explanation))
		{
			MCE_ERROR("get common friend info failed, userid : " << userid << ", targetid : " << targetid << ", type : " << type);
			return false;
		}
	}
	else
	{
		if (!getbasicinfo(userid, targetid, type, explanation)
				|| !getexplainfortype(userid, targetid, "NO_RECOMMEND", explanation))
		{
			MCE_ERROR("get common friend info failed, userid : " << userid << ", targetid : " << targetid << ", type : " << type);
			return false;
		}
	}

	return true;
}

NameCardExplanationI::NameCardExplanationI()
{}

NameCardExplanationI::~NameCardExplanationI()
{}

Explanation NameCardExplanationI::getRecommendExplanation(::Ice::Int userid, const ::std::string &targetid,
									const ::std::string &type, const ::Ice::Current& current)
{
	Explanation result;
	int ownerId;

	//for recommend information 
	if (-1 != targetid.find("_"))
	{
		ownerId = atoi(targetid.substr(targetid.find("_") + 1).c_str());

		//for recommendFriend
		if (targetid.find("recommendFriend") != -1)
		{
			if (generateinfo(userid, ownerId, type, result))
			{
				result.flag = true;
				MCE_INFO("get socialfeedexplaination success, userid :  " << userid << ", targetid : " << targetid << ", type : " << type);
			}
			else
			{
				result.flag = false;
				MCE_ERROR("get socialfeedexplaination failed, userid :  " << userid << ", targetid : " << targetid << ", type : " << type);
			}
		}

		//To Do for page
	}
	//for just basic information
	else
	{
		ownerId = atoi(targetid.c_str());
		
		if (generateinfo(userid, ownerId, type, result))
		{
			result.flag = true;
			MCE_INFO("get socialfeedexplaination success, userid :  " << userid << ", targetid : " << targetid << ", type : " << type);
		}
		else
		{
			result.flag = false;
			MCE_ERROR("get socialfeedexplaination failed, userid :  " << userid << ", targetid : " << targetid << ", type : " << type);
		}
	}

	return result;
}

CommFriendInfoSeq NameCardExplanationI::getCommonFriend(::Ice::Int userid, ::Ice::Int targetid, const ::std::string &type, const ::Ice::Current& current)
{
	CommFriendInfoSeq commonfriendinfoseq;

	if (userid <= 0 || targetid <= 0)
	{
		MCE_ERROR("fail to get valid argvs.");
		return commonfriendinfoseq;
	}

	MyUtil::IntSeq  com_friend_ids;
	try
	{
		com_friend_ids = BuddyFriendsCacheAdapter::instance().getCommonFriends(userid, targetid);
		if (com_friend_ids.empty())
		{
			MCE_ERROR("get explanation for ffucr failed, userid : " << userid << ", targetid : " << targetid);
			return commonfriendinfoseq;
		}
	}
	catch(...)
	{
		MCE_ERROR("BuddyFriendsCacheAdapter::instance().getCommonFriends failed for userid : " << userid << ", targetid : " << targetid);
		return commonfriendinfoseq;
	}

	int com_num = 0;
	MyUtil::StrSeq com_friend_img;
	try
	{
		vector<int>::iterator iter = com_friend_ids.begin();
		while (iter != com_friend_ids.end())
		{
			CommonFriendInfo comfrienditem;
	
			//get tinyimg
			string tinyimg = loadfromusercache("tinyimg", *(iter), SF_UserUrlTripodAdapter);
			if (tinyimg.empty())
			{
				com_friend_ids.erase(iter++);
				continue;
			}
	
			//get name
			string name = loadfromusercache("name", *(iter), SF_UserNameTripodAdapter);
			if (name.empty())
			{
				com_friend_ids.erase(iter++);
				continue;
			}

			comfrienditem.id = *(iter);
			comfrienditem.head = tinyimg;
			comfrienditem.name = name;
			
			commonfriendinfoseq.push_back(comfrienditem);
		
			iter ++;
		}
	}
	catch (...)
	{
		MCE_ERROR("Get common friend tinyurl failed for userid : " << userid << ", targetid : " << targetid);
	}

	return commonfriendinfoseq;
}

void MyUtil::initialize() 
{
	MCE_INFO("NameCardExplanation MyUtil::initialize start.");

	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&NameCardExplanationI::instance(), service.createIdentity("NameCardExplanation", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("NameCardExplanation.Mod");
	string endpoint = service.getCommunicator()->getProperties()->getProperty("NameCardExplanation.Endpoint");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("NameCardExplanation.Interval", 5); 

	ServerStateSubscriber::instance().initialize("ControllerNameCardExplanation", &NameCardExplanationI::instance(), 
												mod, interval, new MyUtil::SocialGraphChannel);
}





















/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
