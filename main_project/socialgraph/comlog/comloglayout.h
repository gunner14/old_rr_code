/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comloglayout.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/17 10:09:40
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __COMLOGLAYOUT_H__
#define  __COMLOGLAYOUT_H__

#include <map>

#include <pthread.h>

#include <log4cplus/layout.h>

namespace comlog
{

	class ComlogLayout
	{
		public:

			/*
			 * @brief:		构造函数
			 * */
			ComlogLayout();

			/*
			 * @brief:		析构函数
			 * */
			virtual ~ComlogLayout();

			/*
			 * @brief:		添加设备
			 * @param:		[in]name:设备名
			 * @param:		[in]pattern:添加设备的日志格式
			 * @return:		true:添加成功，false:添加失败
			 * */
			bool addpattern(const std::string &name, std::string pattern);

			/*
			 * @brief:		删除设备
			 * @param:		[in]name:设备名称
			 * @return:		true:删除成功，false:删除失败
			 * */
			bool delpattern(const std::string &name);

			/*
			 * @brief:		获得Layout指针
			 * @param:		[in]name:设备名称
			 * @return:		log4cplus::Layout*:Layout指针 or NULL
			 * */
			std::auto_ptr<log4cplus::Layout> getlayout(const std::string &name);

			/*
			 * @brief:		获得当前的设备个数
			 * @return:		返回个数
			 * */
			uint32_t getdevicenum();

		private:

			std::map<std::string, log4cplus::Layout*>		device_pattern_map;		
			pthread_rwlock_t								device_pattern_lock;
	};

}















#endif  //__COMLOGLAYOUT_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
