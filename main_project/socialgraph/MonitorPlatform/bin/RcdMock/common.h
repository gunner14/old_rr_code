/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file common.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/07 17:01:45
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __COMMON_H__
#define  __COMMON_H__

#include <string>
#include "comlog.h"

namespace comlog
{

	static comlog::configure* configure_ptr;

	inline void initialize(const string &conf_path)
	{
		try
		{
			if( conf_path.empty() )
			{
				throw "initialize confpath is NULL.";
			}

			configure_ptr = new comlog::configure(conf_path);
			if( !configure_ptr->init() )
			{
				throw "initialize init configure failed.";
			}
		}
		catch(...)
		{
			cout << "configure initialize error." << endl;
		}
	}

#define COMLOG_INIT(confpath) \
	comlog::initialize(confpath)

#define DEBUG(msg, args ...)	\
	do {\
		comlog::configure_ptr->print(COMLOG_DEBUG, "[%s:%d] "msg"\n", __FILE__, __LINE__, ##args); \
	} while(0)

#define TRACE(msg, args...)	\
	do {\
		comlog::configure_ptr->print(COMLOG_TRACE, "[%s:%d] "msg"\n", __FILE__, __LINE__, ##args); \
	} while(0)

#define INFO(msg, args...)	\
	do {\
		comlog::configure_ptr->print(COMLOG_INFO, "[%s:%d] "msg"\n", __FILE__, __LINE__, ##args); \
	} while(0)

#define WARN(msg, args...)	\
	do {\
		comlog::configure_ptr->print(COMLOG_WARN, "[%s:%d] "msg"\n", __FILE__, __LINE__, ##args); \
	} while(0)

#define ERROR(msg, args...)	\
	do {\
		comlog::configure_ptr->print(COMLOG_ERROR, "[%s:%d] "msg"\n", __FILE__, __LINE__, ##args); \
	} while(0)

}













#endif  //__COMMON_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
