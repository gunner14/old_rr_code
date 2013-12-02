/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogaddpender.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/17 10:10:02
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __COMLOGADDPENDER_H__
#define  __COMLOGADDPENDER_H__

#include <map>

#include <pthread.h>

#include <log4cplus/config.hxx>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/appender.h>
#include <log4cplus/fstreams.h>
#include <log4cplus/helpers/timehelper.h>
#include <fstream>
#include <locale>
#include <memory>

namespace comlog
{

//记录的最大文件数(RF_MAX_BACKIP_INDEX + 1)
#define RF_MAX_BACKUP_INDEX 1
//文件最大存储大小(1G)
#define RF_MAX_FILE_SIZE 1024 * 1024L

//记录的最大文件数(DRF_MAX_BACKIP_INDEX + 1)
#define DRF_MAX_BACKUP_INDEX 6

//默认file的记录方式
#define F_FILEMODE std::ios::trunc 

#define ADDPENDER_COMLOGROLLINGFILE "ComlogRollingFile"
#define ADDPENDER_DAILYROLLINGFILE "DailyRollingFile"
#define ADDPENDER_ROLLINGFILE "RollingFile"
#define ADDPENDER_FILE "File"


	typedef struct AddpenderType
	{
		std::string						addpendertype;
		log4cplus::SharedAppenderPtr	addpenderptr;
	};

	class LOG4CPLUS_EXPORT ComlogRollingAppender : public log4cplus::FileAppender
	{
		public:

			/*
			 * @brief:		构造函数
			 * @param:		[in]logfile:文件名称	第一次处理的文件名
			 * @param:		[in]rawlogfile:文件名称
			 * @param:		[in]immediateFlush:标志位，是否立刻进行缓存文件写
			 * */
			ComlogRollingAppender(const log4cplus::tstring &logfile, const log4cplus::tstring &rawlogfile, bool immediateFlush = true);

			/*
			 * @brief:		析构函数
			 * */
			virtual ~ComlogRollingAppender();

			/*
			 * @brief:		关闭文件
			 * @return:		void
			 * */
			void close();

		protected:

			/*
			 * @brief:		append
			 * @param:		[in]event:日志打印事件
			 * @return:		void
			 * */
			virtual void append(const log4cplus::spi::InternalLoggingEvent &event);
	
			/*
			 * @brief:		日志是否需要更换
			 * @return:		true:是，false:否
			 * @note:		用当前时间与rollinglogfile进行对比
			 * */
			bool checklogfile();

			/*
			 * @brief:		open文件
			 * @param:		[in]mode
			 * @return:		void
			 * */
			void open(std::ios::openmode mode);

			/*
			 * @brief:		日志翻滚
			 * @param:		[in]alreadyLocked:是否加锁
			 * @return:		void
			 * */
			void rollover(bool alreadyLocked = false);

			log4cplus::tstring				logdir;
			log4cplus::tstring				rollinglogfile;

		private:

			/*
			 * @brief:		初始化
			 * @param:		[in]logfile:初始化文件名称格式
			 * @return:		void
			 * */
			void init(const log4cplus::tstring &logfile);

			/*
			 * @brief:		检查当前文件夹是否存在
			 * @param:		[in]:logfile:文件名称格式
			 * @return:		void
			 * */
			void checklogdir(const log4cplus::tstring &logfile);

			log4cplus::tstring				_raw_filename;		//<没有修改的日志名称格式
	};

	class ComlogAddpender
	{
		public:

			/*
			 * @biref:		构造函数
			 * */
			ComlogAddpender();

			/*
			 * @biref:		析构函数
			 * */
			virtual ~ComlogAddpender();

			/*
			 * @brief:		添加fileappender
			 * @param:		[in]name:设备名称
			 * @param:		[in]logname:展示的log名称
			 * @param:		[in]type:添加的addpender类型
			 * @param:		[in]logpath:log打印的路径
			 * @param:		[in]DRFschedule:存储频度，默认为HOUTLY
			 * @param:		[in]max_file_size:存储log的最大文件大小
			 * @param:		[in]mode:文件类型，默认为trunc
			 * @param:		[in]immediateFlush:缓冲刷新标志
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addaddpender(const std::string &name, const std::string &logname, const std::string &type, const std::string logpath="./",
							  log4cplus::DailyRollingFileSchedule DRFschedule=log4cplus::DAILY, int drf_max_backup_index=DRF_MAX_BACKUP_INDEX,
							  long rf_max_file_size=RF_MAX_FILE_SIZE, int rf_max_backup_index=RF_MAX_BACKUP_INDEX,
							  std::ios_base::openmode mode=std::ios::trunc, bool immediateFlush=true);

			/*
			 * @biref:		删除addpender
			 * @param:		[in]name:设备名称
			 * @return:		true:删除成功，false:删除失败
			 * */
			bool deladdpender(const std::string &name);

			/*
			 * @brief:		获取addpender
			 * @param:		[in]name:设备名称
			 * @param:		[in]addpenderptr
			 * @return:		true:成功，false:失败
			 * */
			bool getaddpender(const std::string &name, log4cplus::SharedAppenderPtr &addpenderptr);

			/*
			 * @brief:		为addpender设置layout
			 * @param:		[in]name:设备名称
			 * @param:		[in]layoutptr:layout的指针
			 * @return:		true:设置成功，设置失败
			 * */
			bool setlayout(const std::string &name, std::auto_ptr<log4cplus::Layout> layoutptr);

			/*
			 * @brief:		设置过滤器filter
			 * @param:		[in]name:设备名称
			 * @param:		[in]filterptr:过滤器指针
			 * @return:		true:设置成功，false:设置失败
			 * */
			bool setfilter(const std::string &name, const log4cplus::spi::FilterPtr &filterptr);

			/*
			 * @brief:		获取addpender的filter
			 * @param:		[in]:name:设备名称
			 * @param:		[out]filterptr:filter指针
			 * @return:		true:获取成功，false:获取失败
			 * */
			bool getfilter(const std::string &name, log4cplus::spi::FilterPtr &filterptr);

		private:

			/*
			 * @brief:		添加ComlogRollingAppender
			 * @param:		[in]name:设备名称
			 * @param:		[in]logname:展示的log名称
			 * @param:		[in]logpath:log打印的路径
			 * @param:		[in]immediateFlush:缓冲刷新标志
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addcomlogrollingfile(const std::string &name, const std::string &logname, std::string logpath, bool immediateFlush);

			/*
			 * @brief:		添加新的DailyRollingFileAppender
			 * @param:		[in]name:设备名称
			 * @param:		[in]logname:展示的log名称
			 * @param:		[in]logpath:log打印的路径
			 * @param:		[in]DRFschedule:存储频度，默认为HOUTLY
			 * @param:		[in]max_back_index:存储文件的最大个数，缺省为24(23+1)
			 * @param:		[in]immediateFlush:缓冲刷新标志
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool adddailyrollingfile(const std::string &name, const std::string &logname, std::string logpath, 
									 log4cplus::DailyRollingFileSchedule DRFschedule, uint32_t max_backup_index, bool immediateFlush);

			/*
			 * @brief:		添加新的RollingFileAppender
			 * @param:		[in]name:设备名称
			 * @param:		[in]logname:展示的log名称
			 * @param:		[in]logpath:log打印的路径
			 * @param:		[in]max_file_size:存储log的最大文件大小
			 * @param:		[in]max_backup_index:最大文件保存数
			 * @param:		[in]immediateFlush:缓冲刷新标志
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addrollingfile(const std::string &name, const std::string &logname, std::string logpath,
								long max_file_size, uint32_t max_backup_index, bool immediateFlush);

			/*
			 * @brief:		添加新的FileAppender
			 * @param:		[in]name:设备名称
			 * @param:		[in]logname:展示的log名称
			 * @param:		[in]logpath:log打印的路径
			 * @param:		[in]mode:文件类型，默认为trunc
			 * @param:		[in]immediateFlush:缓冲刷新标志
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addfile(const std::string &name, const std::string &logname, std::string logpath,
						 std::ios_base::openmode mode, bool immediateFlush);

			std::map<std::string, struct AddpenderType>					device_addpender_map;
			pthread_rwlock_t											device_addpender_lock;
	};
}
















#endif  //__COMLOGADDPENDER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
