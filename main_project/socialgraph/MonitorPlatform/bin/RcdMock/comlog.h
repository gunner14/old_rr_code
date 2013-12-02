/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlog.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/06 16:13:40
 * @version $Revision$ 
 * @brief 
 *  
 **/

#ifndef  __COMLOG_H__
#define  __COMLOG_H__

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

namespace comlog
{

	typedef enum LOG_LEVEL
	{
		COMLOG_DEBUG = 1,
		COMLOG_TRACE = 2,
		COMLOG_INFO = 4,
		COMLOG_WARN = 8,
		COMLOG_ERROR = 16,
	};

	class logger
	{
		public:

			/*
			 * @brief:		构造函数
			 * */
			logger();

			/*
			 * @brief:		析构函数
			 * */
			~logger();

			/*
			 * @brief:		输出日志
			 * @param:		[in]fp输入文件句柄
			 * @param:		[int]msg为输入日志信息
			 * return:		void
			 * */
			virtual void printlog(FILE *fp, const char *msg);

			/*
			 * @brief:		获取日志格式
			 * @param:		[in]loglevel日志级别
			 * @param:		[out]msgformat日志格式
			 * return:		void
			 * */
			void getformat(LOG_LEVEL loglevel, string &msgformat);
		
	};

	class configure
	{

		friend class logger;

		public:

			/*
			 * @brief:		构造函数
			 * @param:		配置路径
			 * */
			configure(const string &confpath);

			/*
			 * @brief:		析构函数
			 * */
			~configure();

			/*
			 * @brief:		加在配置
			 * */
			virtual bool loadconf();

			/*
			 * @brief:		初始化
			 * @			主要是初始化线程进行fp的句柄观察
			 * */
			virtual bool init();

			/*
			 * @brief:		打印日志
			 * @param:		[in]loglevel,日志级别
			 * @param:		[in]msg,日志内容
			 * @param:		[in]...变长日志内容
			 * @			对外提供的直接打印接口
			 * @			实质调用logger类中的printlog实现
			 * */
			virtual void print(LOG_LEVEL loglevel, const char *msg, ...);

		private:

			/*
			 * @brief:		获取配置值
			 * @param:		[in]配置信息
			 * @param:		[out]结果引用
			 * */
			void getconfValue(const string confline, LOG_LEVEL &result);

			/*
			 * @brief:		获取配置值
			 * @param:		[in]配置信息
			 * @param:		[out]结果引用
			 * */
			void getconfValue(const string confline, string &result);

			/*
			 * @brief:		设置正常日志完整路径
			 * @param:		[in]日志的文件夹
			 * @param:		[in]日志的名称
			 * return:		true or false
			 * */
			bool setlogfile(const string &logdir, const string &logname);

			/*
			 * @brief:		设置错误日志完整路径
			 * @param:		[in]错误日志的文件夹
			 * @param:		[in]错误日志的名称
			 * return:		true of false
			 * */
			bool setlogwffile(const string &logdir, const string &logwfname);

			/*
			 * @brief:		是否需要创建文件夹
			 * @param:		[int]logdir日志文件路径
			 * return:		void
			 * */
			void ismkdir(const string &logdir);

			/*
			 * @brief:		设置写入日志文件句柄
			 * return:		void
			 * */
			void setFILE();

			/*
			 * @brief:		设置写入报警日志文件句柄
			 * return:		void
			 * */
			void setWFFILE();

			logger*					_logger;			//logger句柄
			string					_conf_path;			//日志路径

			string					_logfile;			//正常日志路径名称
			string					_logwffile;			//错误日志路径名称
		
			FILE*					_fp;				//正常日志句柄
			FILE*					_wf_fp;				//错误日志句柄

			string					_loglevel;			//正常日志级别
			string					_logwflevel;		//错误日志级别
	};

}


























#endif  //__MYLOG_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
