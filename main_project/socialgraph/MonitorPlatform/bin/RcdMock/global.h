/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file global.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/09 17:08:33
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __GLOBAL_H__
#define  __GLOBAL_H__

#include <pthread.h>

using namespace std;

class AutoRWlock
{

	public:
		AutoRWlock(pthread_rwlock_t *lock)
		{
			p_lock = lock;
			pthread_rwlock_wrlock(p_lock);
		}

		/*
		 * @brief:构造函数
		 * */
		~AutoRWlock()
		{
			pthread_rwlock_unlock(p_lock);
		}

	private:

		pthread_rwlock_t *p_lock;
};

class AutoPtr
{

	public:
		/*
		* @brief:构造函数
		* */
		AutoPtr(char *pr)
		{
			_pr = pr;
		}

		/*
		* @brief:析构函数
		* */
		~AutoPtr()
		{
			delete _pr;
		}

		/*
		 * @brief:获取当前指针
		 * */
		char * getPr()
		{
			return _pr;
		}

	private:
		char * _pr;

};



















#endif  //__GLOBAL_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
