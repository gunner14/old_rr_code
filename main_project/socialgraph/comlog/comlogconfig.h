/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogconfig.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/24 18:38:54
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __COMLOGCONFIG_H__
#define  __COMLOGCONFIG_H__

#include <set>
#include <map>

#include <log4cplus/loglevel.h>
#include <log4cplus/fileappender.h>

namespace comlog
{

#define COMLOG_PROCNAME "COMLOG_PROCNAME"
#define COMLOG_DEVICE_NUM "COMLOG_DEVICE_NUM"
#define COMLOG_LEVEL "COMLOG_LEVEL"
#define COMLOG_DEVICE "COMLOG_DEVICE"

#define COMLOG_SUBDEVIEC_NAME "_NAME"
#define COMLOG_SUBDEVIEC_SELFLEVEL "_SELFLEVEL"
#define COMLOG_SUBDEVIEC_LAYOUT "_LAYOUT"
#define COMLOG_SUBDEVIEC_PATH "_PATH"
#define COMLOG_SUBDEVIEC_FILENAME "_FILENAME"

#define COMLOG_SUBDEVIEC_APPENDER "_APPENDER"
#define COMLOG_SUBDEVIEC_APPENDER_DRFSCHEDULER "_APPENDER_DRFSCHEDULER"
#define COMLOG_SUBDEVIEC_APPENDER_DRFBACKUPINDEX "_APPENDER_DRFBACKUPINDEX"
#define COMLOG_SUBDEVIEC_APPENDER_RFBACKUPINDEX "_APPENDER_RFBACKUPINDEX"
#define COMLOG_SUBDEVIEC_APPENDER_RFFILESIZE "_APPENDER_RFFILESIZE"
#define COMLOG_SUBDEVIEC_APPENDER_FILEMODE "_APPENDER_FILEMODE"

#define COMLOG_SUBDEVIEC_FILTER "_FILTER"
#define COMLOG_SUBDEVIEC_FILTER_LOGLEVEL "_FILTER_LOGLEVEL"
#define COMLOG_SUBDEVIEC_FILTER_LOGLEVELMIN "_FILTER_LOGLEVELMIN"
#define COMLOG_SUBDEVIEC_FILTER_LOGLEVELMAX "_FILTER_LOGLEVELMAX"
#define COMLOG_SUBDEVIEC_FILTER_STRINGMATCH "_FILTER_STRINGMATCH"

#define COMLOG_DEFAULT_LAYOUT "%D %-5p [%l] %m%n"

#define COMLOG_SEPARATOR ':'
#define COMLOG_COMMENT_CHAR '#'
#define COMLOG_R_CHAR '\r'
#define COMLOG_COMMA ','

	typedef struct ConfigItem
	{
		std::string							_name;
		std::string							_layout;
		std::set<log4cplus::LogLevel>		_selflevelset;
		std::string							_path;
		std::string							_filename;

		std::string							_addpender;
		log4cplus::DailyRollingFileSchedule	_addpender_DRFscheduler;
		int									_addpender_DRFbackupindex;
		int									_addpender_RFbackupindex;
		long								_addpender_RFfilesize;
		std::ios_base::openmode				_addpender_Filemode;
	
		std::string							_filter;
		log4cplus::LogLevel					_filter_loglevel;
		log4cplus::LogLevel					_filter_loglevelmin;
		log4cplus::LogLevel					_filter_loglevelmax;

		std::string							_filter_stringmatch;
	};

	class ComlogConfig
	{
		public:
	
			/*
			 * @brief:		构造函数
			 * */
			ComlogConfig();

			/*
			 * @brief:		析构函数
			 * */
			virtual ~ComlogConfig();

			/*
			 * @brief:		初始化
			 * @param:		[in]confpath:comlog的配置路径
			 * @return:		true:初始化成功，false:初始化失败
			 * */
			bool init(const std::string &confpath);

			/*
			 * @brief:		获得map名字
			 * @param:		[in]num:第几个设备
			 * @param:		[out]结果
			 * @return:		true:获得成功，false:获取失败
			 * */
			bool getItem(uint32_t num, ConfigItem &configitem);
		
			std::string								_comlog_name;			//<模块log注册的名称
			log4cplus::LogLevel						_comlog_level;			//<模块log的级别
			uint32_t								_comlog_device_num;		//<模块log的个数

		private:

			/*
			 * @brief:		读取配置,生成map
			 * @param:		[in]confpath:comlog的配置路径
			 * return:		true:加载成功，false:加载失败
			 * */
			bool genkeyvalue(const std::string &confpath);

			/*
			 * @brief:		生成结果配置
			 * @return:		true:成功，false:失败
			 * */
			bool loadkeyvalue();

			/*
			 * @brief:		删除开头空格字符
			 * @param:		[in]str:字符串
			 * @return:		void
			 * */
			void delleadingspace(std::string &str);

			/*
			 * @brief:		删除结尾空格符
			 * @param:		[in]str:字符串
			 * @return:		void
			 * */
			void deltailingspace(std::string &str);

			/*
			 * @brief:		删除开头和结尾的空格符
			 * @param:		[in]str:字符串
			 * @return:		vodi
			 * */
			void delspace(std::string &str);

			/*
			 * @brief:		通过key获得value
			 * @param:		[in]key:key值
			 * @param:		[out]value:value值
			 * @return:		true:获得成功，false:获得失败
			 * */
			bool getvalue(const std::string &key, std::string &value);

			/*
			 * @brief:		通过key获得value
			 * @param:		[in]key:key值
			 * @param:		[out]value:value值
			 * @return:		true:获得成功，false:获得失败
			 * */
			bool getvalue(const std::string &key, int &value);


			/*
			 * @brief:		通过key获得value
			 * @param:		[in]key:key值
			 * @param:		[out]value:value值
			 * @return:		true:获得成功，false:获得失败
			 * */
			bool getvalue(const std::string &key, log4cplus::DailyRollingFileSchedule &value);

			/*
			 * @brief:		通过key获得value
			 * @param:		[in]key:key值
			 * @param:		[out]value:value值
			 * @return:		true:获得成功，false:获得失败
			 * */
			bool getvalue(const std::string &key, long &value);

			/*
			 * @brief:		通过key获得value
			 * @param:		[in]key:key值
			 * @param:		[out]value:value值
			 * @return:		true:获得成功，false:获得失败
			 * */
			bool getvalue(const std::string &key, std::ios_base::openmode &value);

			/*
			 * @brief:		将字符串以指定的字符进行分割为set集合
			 * @param:		[in]stringline:字符串
			 * @param:		[in]flag:分割字符
			 * @param:		[out]输出的set集合
			 * @return:		void
			 * */
			void getsetvalue(std::string &stringline, char flag, std::set<std::string> &strset);

			/*
			 * @brief:		loglevel转换
			 * @param:		[in]level:string
			 * @param:		[out]loglevel:log4cplus形式
			 * @return:		true:转换成功，false:转换失败
			 * */
			bool transferloglevel(const std::string &level, log4cplus::LogLevel &loglevel);

			/*
			 * @brief:		loglevel切换
			 * @param:		[in]stringline:字符串
			 * @param:		[in]flag:分割字符
			 * @param:		[out]输出的LogLevel的set集合
			 * @return:		void
			 * */
			void getloglevelset(std::string &stringline, char flag, std::set<log4cplus::LogLevel> &loglevelset);

			/*
			 * @brief:		string 与 int相加
			 * @param:		[in]str:string
			 * @param:		[in]inter:in
			 * @return:		std::string
			 * */
			std::string genstr(const std::string &str, uint32_t &inter);

			/*
			 * @brief:		string 与 str相加
			 * @param:		[in]str1:string
			 * @param:		[in]str2:string
			 * @return:		std::string
			 * */
			std::string genstr(const std::string &str1, const std::string &str2);

			std::map<std::string, std::string>				comlog_key_value_map;
			std::map<std::string, struct ConfigItem>		comlog_device_map;		//<存放device的配置
	};
}













#endif  //__COMLOGCONFIG_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
