/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogfilter.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/17 10:09:19
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __COMLOGFILTER_H__
#define  __COMLOGFILTER_H__

#include <map>

#include "pthread.h"

#include <log4cplus/config.hxx>
#include <log4cplus/loglevel.h>
#include <log4cplus/spi/filter.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/helpers/pointer.h>
#include <log4cplus/spi/loggingevent.h>

namespace comlog
{

#define FILTER_NONEFILTER "NoneFilter"
#define FILTER_DENYFILTER "DenyFilter"
#define FILTER_LOGLEVELMATCHFILTER "LogLevelMatchFilter"
#define FILTER_LOGLEVELRANGEFILTER "LogLevelRangeFilter"
#define FILTER_STRINGMATCHFILTER "StringMatchFilter"

#define DEFAULT_STRINGMATCH "NO STRINGMATCH"
#define DEFAULT_ACCEPTONMATCH "true"

	typedef struct FilterType
	{
		std::string type;
		log4cplus::spi::FilterPtr filterptr;
	};

	//默认的日志过滤器，无任何过滤逻辑
	class LOG4CPLUS_EXPORT ComlogNoneFilter : public log4cplus::spi::Filter
	{
		public:

			/*
			 * @biref:		构造函数
			 * */
			ComlogNoneFilter();

			/*
			 * @brief:		构造函数
			 * @param:		[in]properties:filter的属性参数
			 * */
			ComlogNoneFilter(const log4cplus::helpers::Properties &properties);

			/*
			 * @brief:		判断是否进行执行过滤
			 * @param:		[in]event:日志输出事件
			 * @return:		ACCEPT
			 * */
			virtual log4cplus::spi::FilterResult decide(const log4cplus::spi::InternalLoggingEvent& event) const;
	};

	//note:重新实现LogLevelMatchFilter
	//使得只满足设置的loglevel的日志，对于!=的情况下均不进行打印
	class LOG4CPLUS_EXPORT ComlogLogLevelMatchFilter : public log4cplus::spi::Filter
	{
		public:

			/*
			 * @brief:		构造函数
			 * */
			ComlogLogLevelMatchFilter();

			/*
			 * @brief:		构造函数
			 * @param:		[in]properties:filter的属性参数
			 * */
			ComlogLogLevelMatchFilter(const log4cplus::helpers::Properties &properties);

			/*
			 * @brief:		判断是否进行执行过滤
			 * @param:		[in]event:日志输出事件
			 * @return:		DENY,ACCEPT,NEUTRAL	
			 * */
			virtual log4cplus::spi::FilterResult decide(const log4cplus::spi::InternalLoggingEvent& event) const;

		private:

			/*
			 * @brief:		初始化
			 * @return:		void
			 * */
			void init();

			bool					acceptOnMatch;
			log4cplus::LogLevel		logLevelToMatch;

	};

	//note:重新实现StringMatchFilter
	//使得日志中含有指定的字符串时才进行日志打印，对于!=的情况下均不进行打印
	class LOG4CPLUS_EXPORT ComlogStringMatchFilter : public log4cplus::spi::Filter
	{
		public:

			/*
			 * @brief:		构造函数
			 * */
			ComlogStringMatchFilter();

			/*
			 * @brief:		构造函数
			 * @param:		[in]properties:filter的属性参数
			 * */
			ComlogStringMatchFilter(const log4cplus::helpers::Properties &properties);

			/*
			 * @brief:		判断是否进行执行过滤
			 * @param:		[in]event:日志输出事件
			 * @return:		DENY,ACCEPT,NEUTRAL	
			 * */
			virtual log4cplus::spi::FilterResult decide(const log4cplus::spi::InternalLoggingEvent& event) const;

		private:

			/*
			 * @brief:		初始化
			 * @return:		void
			 * */
			void init();

			bool					acceptOnMatch;
			log4cplus::tstring		stringToMatch;
	};

	class ComlogFilter
	{
		public:

			/*
			 * @brief:		构造函数
			 * */
			ComlogFilter();

			/*
			 * @brief:		析构函数
			 * */
			virtual ~ComlogFilter();

			/*
			 * @brief:		添加filter
			 * @param:		[in]name:设备名称
			 * @param:		[in]type:filter类型
			 * @param:		[in]loglevel:用于loglevelfilter类型
			 * @param:		[in]loglevelmin:用于loglevelrangefilter类型
			 * @param:		[in]loglevelmax:用于loglevelrangefilter类型
			 * @param:		[in]stringmatch:用于stringmatchfilter类型
			 * @return:		true:添加成功，false:添加失败
			 * @note:		依据type,其所需的参数必须存在，如果是缺省值，则返回false
			 * */
			bool addfilter(const std::string &name, const std::string &type, log4cplus::LogLevel loglevel = log4cplus::ALL_LOG_LEVEL,
						   log4cplus::LogLevel loglevelmin = log4cplus::ALL_LOG_LEVEL, 
						   log4cplus::LogLevel loglevelmax = log4cplus::OFF_LOG_LEVEL, 
						   std::string stringmatch = DEFAULT_STRINGMATCH);

			/*
			 * @brief:		删除filter
			 * @param:		[in]name:设备名称
			 * @return:		true:成功，false:失败
			 * */
			bool delfilter(const std::string &name);

			/*
			 * @brief:		获取filter
			 * @param:		[in]name;
			 * @return:		log4cplus::spi::FilterPtr
			 * */
			log4cplus::spi::FilterPtr getfilter(const std::string &name);

		private:

			/*
			 * @brief:		添加nonefilter
			 * @param:		[in]name:设备名称
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addnonefilter(const std::string &name);

			/*
			 * @brief:		添加denyfilter
			 * @param:		[in]name:设备名称
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool adddenyfilter(const std::string &name);

			/*
			 * @brief:		添加loglevelmatchfilter
			 * @param:		[in]name:设备名称
			 * @param:		[in]acceptonmatch:是否启用
			 * @param:		[in]logleveltomatch:匹配的loglevel
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addloglevelmatchfilter(const std::string &name, std::string acceptonmatch, log4cplus::LogLevel logleveltomatch);

			/*
			 * @brief:		添加loglevelrangefilter
			 * @param:		[in]name:设备名称
			 * @param:		[in]acceptonmatch:是否启用
			 * @param:		[in]loglevelmin:loglevel低级别范围
			 * @param:		[in]loglevelmax:loglevel高级别范围
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addloglevelrangefilter(const std::string &name, std::string acceptonmatch, 
										log4cplus::LogLevel loglevelmin, log4cplus::LogLevel loglevelmax);

			/*
			 * @brief:		添加stringmatchfilter
			 * @param:		[in]name:设备名称
			 * @param:		[in]acceptonmatch:是否启用
			 * @param:		[in]stringmatch:匹配的字符串
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addstringmatchfilter(const std::string &name, std::string acceptonmatch, const std::string &stringmatch);
			
			/*
			 * @brief:		loglevel映射
			 * @param:		[in]loglevel
			 * @param:		[out]loglevelstr
			 * @return:		true:成功，false:失败
			 * */
			bool logleveltransfer(log4cplus::LogLevel loglevel, std::string &loglevelstr);

			pthread_rwlock_t										device_filter_lock;
			std::map<std::string, struct FilterType>				device_filter_map;
	};
}
















#endif  //__COMLOGFILTER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
