/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file crawler.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/13 15:03:22
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __CRAWLER_H__
#define  __CRAWLER_H__

#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include "netclient.h"
#include "common.h"

using namespace std;
using namespace comlog;

#define HTTP_REQUEST "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\nHost: %s:%d\r\nConnection: keep-alive\r\nCookie: anonymid=h4qnpbtx-81g9kw; _r01_=1; mop_uniq_ckid=10.10.16.62_1342519591_716949375; __utma=10481322.1394162837.1343979082.1343979082.1343979082.1; __utmz=10481322.1343979082.1.1.utmcsr=photo.renren.com|utmccn=(referral)|utmcmd=referral|utmcct=/photo/230494992/photo-6367887322; depovince=BJ; jebecookies=81ba0b09-5a41-4d85-8970-768daa347ad9|||||; loginfrom=null; XNESSESSIONID=abc9mwGjlp1D7cEwCDFKt; vip=1; ick_login=e8328246-c47e-469f-a579-644f9643ea13; idc=tel; _de=C8AADDB1154FA63EE8C176EE92A49B70FA025FA9A94C8260; p=a83ab5934c674168748d17c6ae41078a5; t=0a0bfb5461e6fcc0f057c0607acfacbd5; societyguester=0a0bfb5461e6fcc0f057c0607acfacbd5; id=224357645; xnsid=d656207b\r\n\r\n"

struct tochange
{
	uint32_t to_uint32()
	{
		return atoi(item.c_str());
	}

	const char* to_cstr()
	{
		return item.c_str();
	}

	string to_str()
	{
		return item;
	}

	string item;
};

class crawler : public NetClient
{

	public:

		/*
		 * @brief:	构造函数
		 * */
		crawler()
		{
			pthread_rwlock_init( &_user_response_lock, NULL );
		}

		/*
		 * @brief:	析构函数
		 * */
		~crawler()
		{
			pthread_rwlock_destroy( &_user_response_lock );
		}

		/*
		 * @brief:	初始化
		 * return:	true:初始化成功，false:初始化失败
		 * */
		virtual bool init()
		{
			if( !addclient(_crawler_name) )
			{
				ERROR("addclient failed.");
				return false;
			}
			setretry(_max_retry_time, _max_wait_time);
			if( !setserverinfo(_crawler_name, _server_name.c_str(), _server_port) )
			{
				ERROR("setserverinfo failed.");
				return false;
			}
			if( !loaduserid() )
			{
				ERROR("loaduserid failed.");
				return false;
			}

			INFO("init success, socket has been created and connencted.");
			return true;
		}

		/*
		 * @brief:	启动服务
		 * */
		virtual bool start()
		{
			if( !createsocket(_crawler_name) )
			{
				ERROR("createsocket failed.");
				return false;
			}
			if( !connect_retry(_crawler_name) )
			{
				ERROR("connect failed.");
				return false;
			}

			uint32_t sleep_time = 86400 / _user_vector.size();
			for(vector<string>::iterator iter = _user_vector.begin(); iter != _user_vector.end(); ++iter )
			{
				sendhttprequest((*iter));
				gethttpresponse((*iter));
				checkresponse();
				//sleep(_send_interval);
				//sleep(sleep_time);
			}
			
			return true;
		}

		/*
		 * @brief:	加载配置
		 * @param:	[in]confpath:配置路径
		 * return:	true:加载成功，false:加载失败
		 * */
		virtual bool loadconf(const string &confpath)
		{
			if(confpath.empty())
			{
				ERROR("confpath is NULL.");
				return false;
			}

			if(access(confpath.c_str(), R_OK) !=0 )
			{
				ERROR("open confpath [%s] failed.", confpath.c_str());
				return false;
			}

			FILE *fp = fopen(confpath.c_str(), "r");

			if(NULL == fp)
			{
				ERROR("cannot open conffile [%s].", confpath.c_str());
				return false;
			}

			char *buffer = NULL;
			ssize_t read;
			size_t len;

			while((read = getline(&buffer, &len, fp)) != -1)
			{
				string templine = buffer;
				tochange* confValue = NULL;
				if( -1 != templine.find("max_retry_time") )
				{
					confValue = getconfValue(templine);
					_max_retry_time = confValue->to_uint32();
				}
				if( -1 != templine.find("max_wait_time") )
				{
					confValue = getconfValue(templine);
					_max_wait_time = confValue->to_uint32();
				}
				if( -1 != templine.find("crawler_name") )
				{
					confValue = getconfValue(templine);
					_crawler_name = confValue->to_uint32();
				}
				if( -1 != templine.find("server_name") )
				{
					confValue = getconfValue(templine);
					_server_name = confValue->to_str();
				}
				if( -1 != templine.find("server_port") )
				{
					confValue = getconfValue(templine);
					_server_port = confValue->to_uint32();
				}
				if( -1 != templine.find("userid_conf") )
				{
					confValue = getconfValue(templine);
					_userid_conf = confValue->to_str();
				}
				if( -1 != templine.find("send_interval") )
				{
					confValue = getconfValue(templine);
					_send_interval = confValue->to_uint32();
				}
				if( -1 != templine.find("recommendfriend_num") )
				{
					confValue = getconfValue(templine);
					_rcd_friend = confValue->to_uint32();
				}
				if( -1 != templine.find("commonfriend_num") )
				{
					confValue = getconfValue(templine);
					_cm_friend = confValue->to_uint32();
				}

				if (NULL != confValue)
				{
					delete confValue;
					confValue = NULL;
				}
				if(buffer)
				{
					free(buffer);
					buffer = NULL;
				}
			}

			fclose(fp);
			return true;
		}

		/*
		 * @brief:	创建httprequest
		 * @param:	[in]userid
		 * @param:	[out]httprequest_s:httprequest的格式，string形式
		 * return:	void
		 * */
		virtual void createhttprequest(const string &userid, string &httprequest)
		{
			if(userid.empty())
			{
				WARN("userid is empyt.");
				return;
			}

			//string userid_page = _crawler_name + userid;
			string userid_page = _crawler_name;
			char buffer[1024 * 10] = {0};
			sprintf(buffer, HTTP_REQUEST, userid_page.c_str(), _server_name.c_str(), _server_port);

			httprequest = buffer;
		}

		/*
		 * @brief:	发送数据请求
		 * @param:	[in]userid:发送某个userid的httprequest
		 * return:	true:发送成功，false:发送失败
		 * */
		virtual bool sendhttprequest(const string &userid)
		{
			if(userid.empty())
			{
				ERROR("userid is empty.");
				return false;
			}

			string httprequest;
			createhttprequest(userid, httprequest);

			if(senddata(_crawler_name, httprequest.c_str()))
			{
				INFO("send http request success, userid is [%s]", userid.c_str());
				return true;
			}
			else
			{
				WARN("send http request failed, userid is [%s]", userid.c_str());
				return false;
			}
		}

		/*
		 * @brief:	接收httpresponse
		 * @param:	[in]user:userid
		 * return:	true:接收成功，false:接收失败
		 * */
		virtual bool gethttpresponse(const string &userid)
		{
			if(userid.empty())
			{
				ERROR("uesrid is empyt.");
				return false;
			}

			char *buffer = recvdata(_crawler_name);
			string response = buffer;
			delete buffer;

			AutoRWlock getresponse_lock( &_user_response_lock ) ;
			pair<map<string,string>::iterator,bool> pr = _user_response_map.insert(pair<string,string>(userid, response));
			if(!pr.second)
			{
				INFO("the response of userid [%s] already get.", userid.c_str());
			}

			return true;
		}

		/*
		 * @brief:	检查返回结果是否正确
		 * return:	void
		 * */
		virtual void checkresponse()
		{

			AutoRWlock checkresponse_lock( &_user_response_lock );
			map<string, string>::iterator iter = _user_response_map.begin();
			while( iter != _user_response_map.end())
			{
				FILE *fp = fopen("./rcd.html", "w");
				fputs(iter->second.c_str(), fp);
				fflush(fp);
				fclose(fp);
				string html = iter->second;
				string recommendFriend("recommendFriend");
				string commonFriend("commonFriend");
				int r_sum = 0;
				int c_sum = 0;
				while(1)
				{
					int pos;
					if( (pos = html.find(recommendFriend)) != -1)
					{
						html = html.substr(pos + recommendFriend.length());
						r_sum++;
					}
					else if( (pos = html.find(commonFriend)) != -1 )
					{
						html = html.substr(pos + commonFriend.length());
						c_sum++;
					}
					else
					{
						break;
					}
				}

				if( (r_sum == _rcd_friend) && (c_sum == _cm_friend) )
				{
					INFO("userid is %s, recommendfriend num is %d, commonfriend num is %d, success.", iter->first.c_str(), r_sum, c_sum);
				}

				_user_response_map.erase(iter++);
				if( r_sum != _rcd_friend )
				{
					ERROR("the recommendfriend num is wrong, perspective:%d, actual:%d", _rcd_friend, r_sum);
				}
				if( c_sum != _cm_friend )
				{
					ERROR("the commonfriend num is wrong, perspective:%d, actual:%d", _cm_friend, c_sum);
				}
			}
			
		}

	private:

		/*
		 * @brief:	加载userid的信息
		 * */
		bool loaduserid()
		{

			if(_userid_conf.empty())
			{
				ERROR("userid_conf is empty.");
				return false;
			}

			FILE *fp = fopen(_userid_conf.c_str(), "r");
			if( NULL == fp )
			{
				ERROR("userid_conf [%s] cannot be opened.", _userid_conf.c_str());
				return false;
			}

			char *buffer = NULL;
			ssize_t read;
			size_t len;
			while((read = getline(&buffer, &len, fp )) != -1 )
			{
				string temp(buffer, strlen(buffer) - 1);
				_user_vector.push_back(temp);
				if(buffer)
				{
					free(buffer);
					buffer = NULL;
				}
			}

			return true;
		}

		/*
		 * @brief:	获取配置的值
		 * @param:	[in]confline:配置的一行
		 * return:	stuct tochange的指针，失败返回NULL
		 * */
		struct tochange *getconfValue(const string &confline)
		{

			if(confline.empty())
			{
				ERROR("confline is empty.");
				return NULL;
			}

			int pos = confline.find(":");

			string temp = confline.substr(pos+1);
			string ret;

			for(string::iterator iter = temp.begin(); iter != temp.end(); ++iter)
			{
				if( isspace(*iter) )
					continue;
				else
				{
					ret.push_back((*iter));
				}
			}

			struct tochange *confitem = new tochange();
			confitem->item = ret;

			return confitem;
		}

		string				_crawler_name;
		string				_server_name;
		uint32_t			_server_port;
		string				_userid_conf;
		
		uint32_t			_max_retry_time;
		uint32_t			_max_wait_time;
		uint32_t			_send_interval;

		uint32_t			_rcd_friend;
		uint32_t			_cm_friend;

		vector<string>		_user_vector;
		map<string, string>	_user_response_map;
		pthread_rwlock_t	_user_response_lock;
		set<string>			_right_userid;
		set<string>			_wrong_userid;

};












#endif  //__CRAWLER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
