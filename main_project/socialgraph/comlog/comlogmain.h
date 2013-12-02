/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogmain.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/24 10:08:18
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __COMLOGMAIN_H__
#define  __COMLOGMAIN_H__

#include <iostream>
#include <map>
#include <set>
#include <string>

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>

#include "comlogconfig.h"
#include "comlogfilter.h"
#include "comlogaddpender.h"
#include "comloglayout.h"

namespace comlog
{

	typedef struct LoggerType
	{
		log4cplus::Logger logger;
		std::set<log4cplus::LogLevel> loglevelset;
	};

	class ComlogApp
	{

		friend class ComlogConfig;
		friend class ComlogFilter;
		friend class ComlogAddpender;
		friend class ComlogLayout;

		public:

			/*
			 * @brief:		初始化
			 * @param:		[in]confpath:配置路径
			 * @return:		true:成功，false:失败
			 * */
			bool init(const std::string &confpath);

			/*
			 * @brief:		实例化
			 * @return:		ComlogApp指针
			 * */
			static ComlogApp *instance();

			/*
			 * @brief:		通过loglevel获得logger的级别集合中
			 * @param:		[in]loglevel
			 * @return:		logger:logger句柄
			 * */
			log4cplus::Logger getloggerfromloglevel(log4cplus::LogLevel loglevel);

			static ComlogApp *comlogapp_ptr;

			ComlogConfig *comlogconfig;
			ComlogFilter *comlogfilter;
			ComlogAddpender *comlogaddpender;
			ComlogLayout *comloglayout;

		private:

			//禁止使用构造和析构函数
			/*
			 * @brief:		构造函数
			 * */
			ComlogApp();

			/*
			 * @brief:		析构函数
			 * */
			~ComlogApp();

			/*
			 * @brief:		添加logger
			 * @param:		[in]name:设备名称
			 * @param:		[in]loglevelset:设备的loglevel集合
			 * @param:		[in]loglevel，日志级别
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addlogger(const std::string &name, std::set<log4cplus::LogLevel> loglevelset, log4cplus::LogLevel loglevel);

			/*
			 * @brief:		为logger添加addpender
			 * @param:		[in]name:设备名称
			 * @param:		[in]addpender
			 * @return:		true:成功，false:失败
			 * */
			bool addloggeraddpender(const std::string name, log4cplus::SharedAppenderPtr addpenderptr);

			/*
			 * @brief:		初始化root的logger
			 * @param:		[in]name:root的loggername
			 * @return:		void
			 * */
			void initrootlogger(const std::string &name, log4cplus::LogLevel loglevel);


			log4cplus::Logger									_root;
			log4cplus::SharedAppenderPtr						_rootaddpender;
			std::string											_rootpattern;

			std::map<std::string, struct LoggerType>			_sublogger_map;
	};

	inline void initialize(const std::string &confpath)
	{
		try
		{
			ComlogApp::instance();
			if (!ComlogApp::comlogapp_ptr->init(confpath))
			{
				throw "comlog initialize failed.";
			}
		}
		catch (const char *err)
		{
			std::cout << err << std::endl;
		}
	}
}
















#endif  //__COMLOGMAIN_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
