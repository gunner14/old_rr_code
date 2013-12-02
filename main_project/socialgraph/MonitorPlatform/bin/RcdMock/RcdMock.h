/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file RcdMock.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/14 09:16:39
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __RCDMOCK_H__
#define  __RCDMOCK_H__

#include "global.h"
#include "common.h"

class RcdMock
{

	public:

		/*
		 * @brief:	RcdMock启动过程
		 * @param:	[in]argc:命令行参数
		 * @param:	[in]argv:命令行参数
		 * */
		bool initialize(int argc, char **argv);

		/*
		 * @brief:	run启动
		 * */
		void run();

		/*
		 * @brief:	stop
		 * */
		void stop();

		/*
		 * @brief:	获取单实例
		 * */
		static RcdMock* instance();

		static void on_sig_int(int signal);

	private:

		//禁止使用构造和析构函数
		/*
		 * @brief:	构造函数
		 * */
		RcdMock();

		/*
		 * @brief:	析构函数
		 * */
		~RcdMock();

		/*
		 * @brief:	命令行处理函数
		 * @param:	[in]argc:命令行参数
		 * @param:	[in]argv:命令行参数
		 * */
		bool parse_cmdline(int argc, char **argv);

		/*
		 * @brief:	帮助信息
		 * */
		void print_help(const char* name);

		/*
		 * @brief:	打印模块信息
		 * */
		void print_version(ostream &oss);

		string				r_conf_path;
		string				r_comlog_conf;
		static RcdMock*		r_instance;
		static const char*	r_version_id;
		static const char*	r_build_date;
		static bool			r_stop_rcdmock;
};

#define g_rcdmock RcdMock::instance()












#endif  //__RCDMOCK_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
