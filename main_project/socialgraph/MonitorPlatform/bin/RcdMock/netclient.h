/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file netclient.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/09 17:08:33
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __NETCLIENT_H__
#define  __NETCLIENT_H__

#include <map>
#include <netdb.h>
#include <sys/socket.h>

using namespace std;

class client
{

	public:
		/*
		 * @brief:	构造函数
		 * */
		client();

		/*
		 * @brief:	析构函数
		 * */
		~client();

		/*
		 * @brief:	初始化client
		 * @param:	[in]server:服务器信息
		 * */
		virtual bool init(struct sockaddr_in *server);

		/*
		 * @brief:	返回socket句柄
		 * return:	返回_socketfd描述符
		 * */
		virtual int getsocket();

		/*
		 * @brief:	设置server地址信息
		 * @param:	[in]sockaddr_in地址信息指针
		 * return:	true or false
		 * */
		virtual bool setserver(struct sockaddr_in *server);

		/*
		 * @brief:	返回server地址信息
		 * return:	struck sockaddr_in *
		 * */
		virtual struct sockaddr_in *getserver();

		/*
		 * @brief:	设置socket的句柄
		 * @param:	[in]socketfd
		 * return:	true:设置成功，false:设置失败
		 * */
		virtual bool setsockfd(int socketfd);

	private:

		struct sockaddr_in			_server_addr;
		int							_socketfd;
};


class NetClient
{
	friend class client;

	public:

		/*
		 * @brief:	构造函数
		 * */
		NetClient();

		/*
		 * @brief:	析构函数
		 * */
		~NetClient();

		/*
		 * @brief:	获得client指针
		 * @param:	[in]clientname
		 * return:	client*
		 * */
		virtual client* getclient(const string &clientname);

		/*
		 * @brief:	建立连接
		 * @param:	[in]clientname
		 * return:	true:建立成功，false:建立失败
		 * */
		virtual bool createsocket(const string &clientname);

		/*
		 * @brief:	重试连接
		 * @param:	[in]clientname,需要重试的client句柄
		 * return:	true:连接成功，false:重试失败
		 * */
		virtual bool connect_retry(const string &clientname);

		/*
		 * @brief:	阻塞式发送数据
		 * @param:	[in]clientname
		 * @param:	[in]待发送的数据
		 * return:	true:发送成功，false：发送失败
		 * @note:	先进性connect操作，阻塞式发送数据，采用write进行发送，即：发送数据没有发送完成时，会一直阻塞send
		 * */
		virtual bool senddata(const string &clientname, const char *msgdata);

		/*
		 * @brief:	阻塞式读取数据
		 * @param:	[in]clientname
		 * return:	返回数据的头指针，失败返回NULL
		 * @note:	阻塞式读取数据，采用recv进行读取
		 *			调用者需要将数据copy后，进行本地delete，防止内存泄露
		 * */
		virtual char *recvdata(const string &clientname);

		/*
		 * @brief:	添加client
		 * @param:	[in]clientname:客户端的名称
		 * return:	true:添加成功，false:添加失败	
		 * */
		virtual bool addclient(const string &clientname);

		/*
		 * @brief:	删除client
		 * @param:	[in]clientname:客户端名称
		 * return:	true:删除成功，false:删除失败
		 * */
		virtual bool deleteclient(const string &clientname);

		/*
		 * @brief:	设置重连参数
		 * @param:	[in]retry_time:重连的最大次数
		 * @param:	[in]wait_time:重连的最大间隔
		 * return:	void
		 * */
		virtual void setretry(uint32_t retry_time, uint32_t wait_time);

		/*
		 * @brief:	解析服务器信息
		 * @param:	[in]server名字
		 * @param:	[out]serverinfo:struct hostent*
		 * return:	void
		 * */
		virtual void parseweb(const char *server, struct hostent *host);

		/*
		 * @brief:	设置client对应的服务器
		 * @param:	[in]clientname
		 * @param:	[in]server:server信息
		 * @param:	[in]port:server的端口号
		 * return:	true:设置成功，false:设置失败
		 * */
		virtual bool setserverinfo(const string &clientname, const char *server, uint32_t port);

		/*
		 * @brief:	设置client对应的服务器
		 * @param:	[in]server:server信息
		 * @param:	[in]port:server的端口号
		 * return:	true:设置成功，false:设置失败
		 * */
		virtual bool setserverinfo(const char *server, uint32_t port);

	private:

		/*
		 * @brief:	在堆上开辟新的空间
		 * @param:	[in]extend_size:空间长度
		 * return:	char *空间首指针
		 * */
		char* extend_buffer(size_t extend_size);

		//client端
		map<string, client*>			_client_map;		//client映射map表
		pthread_rwlock_t				_client_map_lock;	//map使用锁

		//server端
		map<string, struct sockaddr_in>	_server_map;		//server映射表

		uint32_t						_max_retry_time;
		uint32_t						_max_wait_time;
};


















#endif  //__NETCLIENT_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
