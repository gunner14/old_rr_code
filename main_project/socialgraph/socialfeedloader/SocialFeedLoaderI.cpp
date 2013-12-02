/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file socialfeedloader.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/10/19 10:33:59
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
#include "SocialFeedLoaderI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "Singleton.h"

#include "tripod2/cc/core/tripod_client.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"

#include "OceCxxAdapter/src/FeedCreatorAdapter.h"
#include "OceCxxAdapter/src/UserNameTripodAdapter.h"
#include "OceCxxAdapter/src/UserWorkplaceTripodAdapter.h"
#include "OceCxxAdapter/src/UserUrlTripodAdapter.h"
#include "OceCxxAdapter/src/UserUniversityTripodAdapter.h"
//#include "OceCxxAdapter/src/UserConfigTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"

using namespace MyUtil;
using namespace xce::usercache;
using namespace xce::serverstate;
using namespace com::xiaonei::xce;
using namespace xce::tripod;
using namespace xce::socialgraph;
using namespace xce::adapter;
using namespace xce::adapter::username;
using namespace xce::adapter::userurl;

static std::map<int, struct UserInfo>				_user_map;
static pthread_mutex_t								_global_mutex;
static pthread_t									_threadid;

static std::string									_reload_user_id_time;
static uint32_t										_thread_sleep_interval;
static int											_recommend_time_per_day;
static int											_recommend_num;
static std::string									_userid_file;

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

static std::string loadexplain(int srcid, int targetid)
{
	mop::hi::svc::model::UniversityInfoSeq ptrme = UserUniversityTripodAdapter::instance().getUniversityInfoSeq(srcid);
	mop::hi::svc::model::UniversityInfoSeq ptruser = UserUniversityTripodAdapter::instance().getUniversityInfoSeq(targetid);
	int  max_enrollYear1 = 0;//same univercity, default is latest
	int  max_enrollYear2 = 0;
	std::string value = "";
	for (mop::hi::svc::model::UniversityInfoSeq::const_iterator it1 = ptruser.begin(); it1!= ptruser.end(); it1++)
	{
		if ((*it1)->enrollYear > max_enrollYear1)
		{//default is latest
			max_enrollYear1 = (*it1)->enrollYear;
			value = (*it1)->universityName;
		}   
		for (mop::hi::svc::model::UniversityInfoSeq::const_iterator it2 = ptrme.begin(); it2 != ptrme.end(); it2++)
		{
			if (((*it1)->universityId == (*it2)->universityId) && ((*it2)->enrollYear > max_enrollYear2))
			{//some time in same univercity
				max_enrollYear2 = (*it2)->enrollYear;
				value = (*it2)->universityName; 
			}       
		}       
	}       
	return value;
}

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

/*
 * @brief:		加载userid_file文件
 * @param；		[in]userid_file:文件地址
 * @return:		true:加载成功，false:加载失败
 * */
static bool loaduserid(const std::string &userid_file)
{

	time_t begintime = time(NULL);
	FILE *fp = fopen(userid_file.c_str(), "r");
	if (NULL == fp)
	{
		MCE_ERROR("Userid File is not exist.");
		return false;
	}

	char *charptr = NULL;
	ssize_t read;
	size_t len;
	uint32_t count = 0;

	while ((read = getline(&charptr, &len, fp)) != -1)
	{
		std::string buffer = charptr;
		size_t bufferlen = buffer.size();
		if (bufferlen == 0)
		{
			continue;
		}
		if (buffer[bufferlen - 1] == '\r')
		{   
			buffer.resize (bufferlen - 1); 
		}

		int tempuserid = (int)atoi(buffer.c_str());
		struct UserInfo tempuserinfo;
		tempuserinfo.lastrecommendtime = 0;
		std::pair<std::map<int, struct UserInfo>::iterator, bool> pr = 
											_user_map.insert(pair<int, struct UserInfo>(tempuserid, tempuserinfo));
		if (pr.second)
		{
			//MCE_ERROR("Userid File load failed : " << tempuserid);
			//return false;
			count ++;
		}
		if (charptr)
		{
			free(charptr);
			charptr = NULL;
		}
	}

	fclose(fp);

	time_t endtime = time(NULL);
	MCE_INFO("load userid file success : " << count << " cost time : " << endtime - begintime);
	return true;
}

static void cleanrecommendinfo()
{
	pthread_mutex_lock(&_global_mutex);
	std::map<int, struct UserInfo>::iterator iter = _user_map.begin();
	while (iter != _user_map.end())
	{
		_user_map.erase(iter++);
	}

	if (!loaduserid(_userid_file))
	{
		MCE_ERROR("cleanrecommendinfo loaduserid failed.");
	}
	pthread_mutex_unlock(&_global_mutex);
}

void *loaduseridthread(void *argv)
{
	while (1)
	{
		time_t timeNow;
		struct tm *timeNow_ptr;
		timeNow = time(NULL);
		timeNow_ptr = localtime(&timeNow);
		char buffer[1024];
		strftime(buffer, 1024, "%H", timeNow_ptr);

		std::string nowtime_str = buffer;
		if (0 == nowtime_str.compare(_reload_user_id_time))
		{
			cleanrecommendinfo();
			MCE_INFO("load userid thread running in " << nowtime_str);
		}
		else
		{
			MCE_DEBUG("load userid thread do not load userid in " << nowtime_str);
		}

		sleep(_thread_sleep_interval);
	}
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

			if (key == "RECOMMEND_PER_DAY")
			{
				_recommend_time_per_day = atoi(value.c_str());
			}
			else if (key == "USERID_FILE")
			{
				_userid_file = value;
			}
			else if (key == "RECOMMEND_NUM")
			{
				_recommend_num = atoi(value.c_str());
			}
			else if (key == "THREAD_SLEEP_INTERVAL")
			{
				_thread_sleep_interval = atoi(value.c_str());
			}
			else if (key == "RELOAD_USERID_TIME")
			{
				_reload_user_id_time = value;
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

	int ret = pthread_mutex_init(&_global_mutex, NULL);
	if (ret)
	{
		MCE_ERROR("pthread mutex init failed.");
		return false;
	}
	
	if (!loaduserid(_userid_file))
	{
		return false;
	}

	ret = pthread_create(&_threadid, NULL, loaduseridthread, NULL);
	if (ret)
	{
		MCE_ERROR("create loaduserid thread failed.");
		return false;
	}

	return true;
}

/*
 * @brief:		计算下次推荐的时间
 * @param:		[in]userid
 * @return:		true:是否需要推荐
 * */
static bool calculatenexttime(const int userid, struct UserInfo &userinfo)
{

	assert(_recommend_time_per_day > 0);
	uint32_t interval_time = 86400 / _recommend_time_per_day;
	time_t timenow = time(NULL);
	if (0 == userinfo.lastrecommendtime)
	{
		userinfo.lastrecommendtime = timenow;
		MCE_DEBUG("calculatenexttime true : " << userinfo.lastrecommendtime << " for " << userid);
		return true;
	}
	else
	{
		if (userinfo.lastrecommendtime + interval_time >= timenow)
		{
			MCE_DEBUG("calculatenexttime false, lasttime : " << userinfo.lastrecommendtime << ", now : " << timenow << " for " << userid);
			return false;
		}
		else
		{
			MCE_DEBUG("calculatenexttime true, lasttime : " << userinfo.lastrecommendtime << ", now : " << timenow << " for " << userid);
			userinfo.lastrecommendtime = timenow;
			return true;
		}
	}
}

/*
 * @brief:		获取好友簇数据
 * @param:		[in]userid
 * @return:		RecommendItemSeq
 * */
static RecommendItemSeq getfriendclusterdata(int userid)
{
	const std::string biz = "FFUCR";
	RecommendItemSeq item_list;                                                     //result of recommendation
	BinaryData raw_data;
	int rank = 0;

	int64_t useridcode_ = userid + 100000000000000;
	bool fetch;
	try
	{
		fetch = BusinessCacheAdapter::GetInstance().Get(biz, useridcode_, raw_data);
	}
	catch(...)
	{
		MCE_ERROR("BusinessCacheAdapter::GetInstance() failed.");
		return item_list;
	}

	if (!fetch) 
	{
		MCE_ERROR("BusinessCacheAdapter Get Failed : " << userid);
		return item_list;
	}   

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
		item_list.push_back(r);
	}   

	MCE_DEBUG("getfriendclusterdata success : " << userid);
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
 * @brief:		产生推荐数据
 * */
static bool genrecommendinfo(int userid)
{
	
	std::string key, value;
	std::pair<std::map<std::string, std::string>::iterator, bool> pr;
	std::map<std::string, std::string> result_map;

	//time
	key = "time";
	time_t nowtime = time(NULL);
	nowtime *= 1000;
	value = boost::lexical_cast<std::string>(nowtime);
	pr = result_map.insert(std::pair<std::string,std::string>(key, value));
	MCE_DEBUG("recommendinfo : <time:value>(" << key << " : " << value << ")");

	//from.link
	key = "from.link";
	//人人主页的链接
	value = "http://page.renren.com/renren";
	pr = result_map.insert(std::pair<std::string,std::string>(key, value));
	MCE_DEBUG("recommendinfo : <from.link:value>(" << key << " : " << value << ")");

	//from.url
	key = "from.tinyimg";
	//人人主页的头像
	value = "http://hdn.xnimg.cn/photos/hdn421/20090923/1935/tiny_ODII_19242g019116.jpg";
	pr = result_map.insert(std::pair<std::string,std::string>(key, value));
	MCE_DEBUG("recommendinfo : <from.tinyimg:value>(" << key << " : " << value << ")");

	//from.id
	//人人网页面的id
	key = "from.id";
	value = "600002246";
	pr = result_map.insert(std::pair<std::string,std::string>(key, value));
	MCE_DEBUG("recommendinfo : <from.id:value>(" << key << " : " << value << ")");

	//from.name
	//人人网名称
	key = "from.name";
	value = "人人网";
	pr = result_map.insert(std::pair<std::string,std::string>(key, value));
	MCE_DEBUG("recommendinfo : <from.name:value>(" << key << " : " << value << ")");

	key = "title.comment";
	value = "猜您可能认识这些人";
	pr = result_map.insert(std::pair<std::string,std::string>(key, value));
	MCE_DEBUG("recommendinfo : <from.name:value>(" << key << " : " << value << ")");

	//RecommendItemSeq fcdata = getfriendclusterdata(userid);
	RecommendItemSeq fcdata;
	RecommendItemSeq rcddata = getrcddata(userid);
	RecommendItemSeq finaldata = deleteduplicate(fcdata, rcddata);

	if (finaldata.empty())
	{
		return false;
	}
	deleteillergaluser(userid, finaldata);

	int num = 0;
	std::map<int, struct UserInfo>::iterator it = _user_map.find(userid);

	for (RecommendItemSeq::const_iterator iter = finaldata.begin(); iter != finaldata.end(); ++iter)
	{
		if (num >= _recommend_num)
		{
			break;
		}

		if (it->second.user_black_set.find(iter->id) != it->second.user_black_set.end())
		{
			continue;
		}
		it->second.user_black_set.insert((int)iter->id);

		//target.id
		key = "target." + boost::lexical_cast<std::string>(num) + ".id";
		value = boost::lexical_cast<std::string>(iter->id);
		result_map.insert(std::pair<std::string, std::string>(key, value));
		MCE_DEBUG("recommendinfo : <" << key << ":value>(" << key << " : " << value << ")");

		//target.name
		key = "target." + boost::lexical_cast<std::string>(num) + ".name";
		value = loadfromusercache(key, iter->id, SF_UserNameTripodAdapter);
		if (value.empty())
		{
			break;
		}
		result_map.insert(std::pair<std::string, std::string>(key,value));
		MCE_DEBUG("recommendinfo : <" << key << ":value>(" << key << " : " << value << ")");

		//target.mainimg
		key = "target." + boost::lexical_cast<std::string>(num) + ".mainimg";
		value = loadfromusercache(key, iter->id, SF_UserUrlTripodAdapter); 
		if (value == "http://hdn.xnimg.cn/photos/" || value.empty())
		{
			break;
		}
		result_map.insert(std::pair<std::string,std::string>(key, value));
		MCE_DEBUG("recommendinfo : <" << key << ":value>(" << key << " : " << value << ")");

		//target.explain
		key = "target." + boost::lexical_cast<std::string>(num) + ".explain";
		if (0 != iter->count)
		{
			value = boost::lexical_cast<std::string>(iter->count) + "个共同好友";
			result_map.insert(std::pair<std::string,std::string>(key, value));
			MCE_DEBUG("recommendinfo : <" << key << ":value>(" << key << " : " << value << ")");
		}
		else //推荐好友的公司
		{
			value = loadfromusercache(key, iter->id, SF_UserWorkplaceTripodAdapter);
			if (value.empty())
			{
				break;
			}
			result_map.insert(std::pair<std::string,std::string>(key, value));
			MCE_DEBUG("recommendinfo : <" << key << ":value>(" << key << " : " << value << ")");
		}

		//target.logTag
		key = "target." + boost::lexical_cast<std::string>(num) + ".logTag";
		value = iter->type;
		result_map.insert(std::pair<std::string,std::string>(key, value));
		MCE_DEBUG("recommendinfo : <" << key << ":value>(" << key << " : " << value << ")");

		num ++;
	}

	//to.count
	key = "to.count";
	value = boost::lexical_cast<std::string>(num);
	pr = result_map.insert(std::pair<std::string,std::string>(key, value));
	MCE_DEBUG("recommendinfo : <to.count:value>(" << key << " : " << value << ")");

	//to.id
	key = "to.id";
	value = boost::lexical_cast<std::string>(userid);
	pr = result_map.insert(std::pair<std::string,std::string>(key, value));
	MCE_DEBUG("recommendinfo : <to.id:value>(" << key << " : " << value << ")");

	try
	{
		if (!xce::feed::FeedCreatorAdapter::instance().Create(1209, 1, result_map))
		{
			MCE_ERROR("Push RecommendInfo SocialFeed Error : " << userid);
		}
		else
		{
			MCE_INFO("Push RecommendInfo SocialFeed Success : " << userid);
		}
		//MCE_DEBUG("Test for SocialFeedLoader");
	}
	catch (Ice::Exception& e) 
	{
		MCE_ERROR("SocialFeedLoaderI::genrecommendinfo Set remained feeds ice exception " << e.what());
	} 
	catch (std::exception& e) 
	{
		MCE_ERROR("SocialFeedLoaderI::genrecommendinfo Set remained feeds std exception " << e.what());
	} 
	catch (...) 
	{
		MCE_ERROR("SocialFeedLoaderI::genrecommendinfo Set remained feeds unknow exception");
	}

	return true;
}

SocialFeedLoaderI::SocialFeedLoaderI()
{}

SocialFeedLoaderI::~SocialFeedLoaderI()
{}

void SocialFeedLoaderI::handler(int userid, const ::Ice::Current& current)
{
	std::map<int, struct UserInfo>::iterator iter = _user_map.find(userid);
	if (iter == _user_map.end())
	{
		MCE_INFO("no this userid in socialfeed userid_file : " << userid );
	}
	else if(calculatenexttime(iter->first, iter->second))
	{
		if (genrecommendinfo(userid))
		{
			MCE_INFO("push socialfeed success : " << userid);
		}
		else
		{
			MCE_ERROR("push socialfeed failed : " << userid);
		}
	}
	else
	{
		MCE_INFO("cannot push socialfeed, already pushed or getinfofrom cache is empty : " << userid);
	}
}

void MyUtil::initialize() 
{
	MCE_INFO("SocialFeedLoader MyUtil::initialize start.");

	if (!init("/data/xce/XSocialGraph/sg_conf/socialfeedloader.conf"))
	{
		MCE_ERROR("SocialFeedLoader init failed.");
		return;
	}

	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&SocialFeedLoaderI::instance(), service.createIdentity("SocialFeedLoader", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("SocialFeedLoader.Mod");
	string endpoint = service.getCommunicator()->getProperties()->getProperty("SocialFeedLoader.Endpoint");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialFeedLoader.Interval", 5); 

	ServerStateSubscriber::instance().initialize("ControllerSocialFeedLoader", &SocialFeedLoaderI::instance(), mod, interval, new MyUtil::SocialGraphChannel);

}




















/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
