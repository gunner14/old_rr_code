/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file FriendRecommendI.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/12/13 13:40:42
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
#include "FriendRecommendI.h"
#include "FriendExplanation.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "Singleton.h"

#include "tripod2/cc/core/tripod_client.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"

#include "OceCxxAdapter/src/FeedCreatorAdapter.h"
#include "OceCxxAdapter/src/UserNameTripodAdapter.h"
#include "OceCxxAdapter/src/UserWorkplaceTripodAdapter.h"
#include "OceCxxAdapter/src/UserUrlTripodAdapter.h"
#include "OceCxxAdapter/src/UserUniversityTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphFFUCRTripodAdapter.h"

using namespace MyUtil;
using namespace xce::usercache;
using namespace xce::serverstate;
using namespace com::xiaonei::xce;
using namespace xce::tripod;
using namespace xce::socialgraph;
using namespace xce::adapter;
using namespace xce::adapter::username;
using namespace xce::adapter::userurl;

static int								_recommend_num;
static map<int, string>					friend_explanation_map;

static void delleadingspace(std::string &str)
{
	std::string::iterator iter = str.begin();
	for(; iter != str.end (); ++iter)
	{
		if (!isspace (*iter))
			break;
	}
	str.erase (str.begin (), iter);
}

static void deltailingspace(std::string &str)
{
	std::string::reverse_iterator riter = str.rbegin();
	for(; riter != str.rend (); ++riter)
	{
		if (!isspace(*riter))
			break;
	}
	str.erase(riter.base(), str.end());
}

static void delspace(std::string &str)
{
	delleadingspace(str);
	deltailingspace(str);
}

static bool init(const string &confpath)
{
	FILE *fp = fopen(confpath.c_str(), "r");
	if (NULL == fp)
	{
		MCE_ERROR("read conf file failed : " << confpath);
		return false;
	}

	char *charptr = NULL;
	ssize_t read;
	size_t len;

	while ((read = getline(&charptr, &len, fp)) != -1) 
	{   
		std::string buffer = charptr;
		delleadingspace(buffer);

		size_t bufferlen = buffer.size();
		if (bufferlen == 0)
		{   
			continue;
		}   

		if (buffer[bufferlen - 1] == '\r')
		{   
			buffer.resize (bufferlen - 1);
		}   

		size_t pos = buffer.find(":");
		if (-1 != pos)
		{
			std::string key = buffer.substr(0, pos);
			std::string value = buffer.substr(pos + 1);
			deltailingspace(key);
			delspace(value);

			if (key == "RECOMMEND_NUM")
			{
				_recommend_num = atoi(value.c_str());
			}
			else
			{
				MCE_ERROR("wrong key in conf : " << confpath << " key : " << key );
				return false;
			}
		}   

		if (charptr)
		{
			free(charptr);
			charptr = NULL;
		}   
	}   

	fclose(fp);

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

/*
 * @brief:		获取二度好友数据
 * */
static RecommendItemSeq getrcddata(int userid)
{
	RecommendItemSeq item_list;							//result of recommendation
	srand(unsigned (time (NULL)));
	std::string value;
	string key;
		
	key = boost::lexical_cast<std::string>(userid);
	try
	{
		SocialGraphRcdTripodAdapter::instance().Get(key, &value);
	}
	catch(...)
	{
		MCE_ERROR("SocialGraphRcdTripodAdapter::instance() failed");
	}

	if (!value.empty()) 
	{
		BinaryData fof_raw_data(value.begin(), value.end());
		Items fof_raw_items;
		if (fof_raw_items.ParseFromArray(fof_raw_data.data(), fof_raw_data.size())) 
		{
			BOOST_FOREACH(const Item& it, fof_raw_items.items()) 
			{
				if (it.message() == "FOF") 
				{		//new fof friends
					RecommendItem r;
					r.id = it.id();	
					r.count = it.field();
					BOOST_FOREACH(int i, it.fields()) 
					{
						r.commons.push_back(i);
					}
					try 
					{
						r.weight = boost::lexical_cast<float>(it.messages(0));
					} 
					catch (boost::bad_lexical_cast& e) 
					{
						MCE_WARN("boost::lexical_cast error : " << e.what()); 
					}
					r.type = it.message();
					item_list.push_back(r);
				}
			}
		}
	} 
	else 
	{
		const std::string biz_rcd = "RCD";
		RecommendItemSeq tmp_list;
		BinaryData rcd_raw_data;
		bool fetch_rcd;
		try
		{
			fetch_rcd = BusinessCacheAdapter::GetInstance().Get(biz_rcd, userid, rcd_raw_data);
		}
		catch(...)
		{
			MCE_ERROR("BusinessCacheAdapter::GetInstance() failed.");
			return tmp_list;
		}

		if (!fetch_rcd) 
		{
			MCE_ERROR("BusinessCacheAdapter get data failed.");
			return tmp_list; 
		}
		Items rcd_raw_items;
		if (!rcd_raw_items.ParseFromArray(rcd_raw_data.data(), rcd_raw_data.size())) 
		{
			MCE_ERROR("ParseFromArray Error.");
			return tmp_list; 
		}
		BOOST_FOREACH(const Item& it, rcd_raw_items.items()) 
		{
			if (it.message() == "FFW" ||			//common friends
					it.message() == "FFUCR") 
			{		//cluster friends
				RecommendItem r;
				r.id = it.id();	
				r.count = it.field();
				BOOST_FOREACH(int i, it.fields()) 
				{
					r.commons.push_back(i);
				}
				r.weight = -1.0f;
				r.type = it.message();

				tmp_list.push_back(r);
			}
		}
		item_list.insert(item_list.end(), tmp_list.begin(), tmp_list.end()); 
	}

	MCE_DEBUG("getrcddata success.");
	return item_list;
}

/*
 * @brief:		删除相同好友
 * */
static RecommendItemSeq deleteduplicate(RecommendItemSeq &itemseq1, RecommendItemSeq &itemseq2)
{
	int ffucrEndPos = 0;
	std::vector<RecommendItem>::iterator Iter;
	RecommendItemSeq resultseq;

	if (itemseq1.empty() && itemseq2.empty())
	{
		return resultseq;
	}

	if (itemseq1.empty() && !itemseq2.empty())
	{
		std::random_shuffle(itemseq2.begin(), itemseq2.end());
		return itemseq2;
	}
	if (itemseq2.empty() && !itemseq1.empty())
	{
		std::random_shuffle(itemseq1.begin(), itemseq1.end());
		return itemseq1;
	}

	std::map<int, RecommendItem> resultmap;
	for (std::vector<RecommendItem>::iterator iter = itemseq1.begin(); iter != itemseq1.end(); ++iter)
	{
		// this is for cluster friend
		resultmap.insert(std::pair<int,RecommendItem>((*iter).id, *iter));
		//resultseq.push_back(*iter);
	}
	for (std::vector<RecommendItem>::iterator iter = itemseq2.begin(); iter != itemseq2.end(); ++iter)
	{
		//this is for FOF friend
		/*if (resultmap.find((*iter).id) == resultmap.end())
		{
			resultseq.push_back(*(iter));
		}
		*/
		resultmap.insert(std::pair<int,RecommendItem>((*iter).id, *iter));
	}

	for (std::map<int, RecommendItem>::iterator iter = resultmap.begin(); iter != resultmap.end(); ++iter)
	{
		resultseq.push_back(iter->second);
	}
	//std::random_shuffle(resultseq.begin(), resultseq.end());

	MCE_DEBUG("deleteduplicate success.");
	return resultseq;
}

/*
 * @brierf:		删除注销用户
 * */
static void deleteillergaluser(int userid, xce::socialgraph::RecommendItemSeq &itemseq)
{
	//delete 注销用户
	UserCacheMap user_map;
	std::vector<int> user_v;
	for (std::vector<RecommendItem>::iterator iter = itemseq.begin(); iter != itemseq.end(); ++iter)
	{
		user_v.push_back(iter->id);
	}

	try
	{
		user_map = xce::usercache::UserCacheAdapter::instance().GetUserCache(user_v);
	}
	catch (...)
	{
		MCE_ERROR("xce::usercache::UserCacheAdapter::instance().GetUserCache() exception : " << userid);
	}

	std::vector<RecommendItem>::iterator iter = itemseq.begin(); 

	try
	{
		while (iter != itemseq.end())
		{
			UserCacheMap::iterator it = user_map.find(iter->id);
			int status = it->second->status();
			if (status == 6 || status == 7 || status == 8)
			{
				iter = itemseq.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	catch (...)
	{
		MCE_WARN("get UserCacheMap status error.");
	}

	//delete 有隐私设置的用户
	/*
	iter = itemseq.begin();
	while (iter != itemseq.end())
	{
		try
		{
			xce::adapter::UserConfigInfoPtr iter_cfg = xce::adapter::UserConfigTripodAdapter::instance().getUserConfig(iter->id);
			if (iter_cfg->requestFriendConfig() == 3)
			{
				iter = itemseq.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		catch (...)
		{
			MCE_WARN("xce::adapter::UserConfigTripodAdapter::instance().getUserConfig error.");
		}
	}
	*/
}

/*
 * @brief:		通过解释索引获得解释
 * */
static vector<string> getfriendexplanation(int explanation)
{
	map<int, string>::iterator iter;
	vector<string> result;
	
	for (int pos = 0; pos < 30; pos++)
	{
		int temp = explanation & (1 << pos);
		if (temp)
		{
			iter = friend_explanation_map.find(temp);
			result.push_back(iter->second);
		}
	}

	return result;
}

/*
 * @brief:		通过解释索引获得资料推荐的解释
 * */
static vector<string> getfriendexplanationbyinfo(int explanation)
{
	map<int, string>::iterator iter;
	vector<string> result;
	
	if (explanation & (1 << 31))
	{
		result = getfriendexplanation(explanation); 
	}

	MCE_INFO("get friend explanation for FFUCR success : " << result.size());
	return result;
}

FriendRecommendI::FriendRecommendI()
{}

FriendRecommendI::~FriendRecommendI()
{}

RecommendItemSeq FriendRecommendI::getFriendRecommendation(int userid, int limit, const ::Ice::Current& current)
{
	RecommendItemSeq finaldata;
	RecommendItemSeq fcdata = getfriendclusterdata(userid);
	RecommendItemSeq rcddata = getrcddata(userid);
	RecommendItemSeq tempdata = deleteduplicate(fcdata, rcddata);
	deleteillergaluser(userid, tempdata);

	if (tempdata.size() > limit)
	{
		finaldata.assign(tempdata.begin(), tempdata.begin() + limit);
	}

	//get explanation
	for (RecommendItemSeq::iterator iter = finaldata.begin(); iter != finaldata.end(); ++iter)
	{
		if (iter->explanations.size() == 0)
		{
			string explain_str = "共同";
			explain_str += boost::lexical_cast<std::string>(iter->count);
			explain_str += "个好友";
			iter->explanations.push_back(explain_str);
		}
		else
		{
			int explain_int = atoi(iter->explanations[0].c_str());
			iter->explanations = getfriendexplanation(explain_int);
		}
	}

	MCE_INFO("getFriendRecommendation success for : " << userid << " with " << finaldata.size() << " friends.");
	return finaldata;
}

RecommendItemSeq FriendRecommendI::getInfoRecommendation(int userid, int limit, const ::Ice::Current& current)
{
	RecommendItemSeq finaldata;
	RecommendItemSeq fcdata = getfriendclusterdata(userid);
	deleteillergaluser(userid, fcdata);

	if (fcdata.size() > limit)
	{
		finaldata.assign(fcdata.begin(), fcdata.begin() + limit);
	}

	//get explanation
	RecommendItemSeq::iterator iter = finaldata.begin();
	while (iter != finaldata.end())
	{
		//delete the explanation is empty
		if (iter->explanations.size() == 0)
		{
			finaldata.erase(iter++);
			continue;
		}
		else
		{
			int explain_int = atoi(iter->explanations[0].c_str());
			iter->explanations = getfriendexplanationbyinfo(explain_int);
		}
		iter ++;
	}

	MCE_INFO("getInfoRecommendation success for : " << userid << " with " << finaldata.size() << " friend.");
	return finaldata;
}

void MyUtil::initialize() 
{
	MCE_INFO("FriendRecommend MyUtil::initialize start.");

	if (!initExplanation(friend_explanation_map))
	{
		MCE_ERROR("FriendRecommend Init Failed.");
		return;
	}

	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&FriendRecommendI::instance(), service.createIdentity("FriendRecommend", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FriendRecommend.Mod");
	string endpoint = service.getCommunicator()->getProperties()->getProperty("FriendRecommend.Endpoint");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FriendRecommend.Interval", 5); 

	ServerStateSubscriber::instance().initialize("ControllerFriendRecommend", &FriendRecommendI::instance(), mod, interval, new MyUtil::SocialGraphChannel);
}





















/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
