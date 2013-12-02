/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogcommon.h
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/26 14:27:39
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __COMLOGCOMMON_H__
#define  __COMLOGCOMMON_H__

#include "comlogmain.h"

using namespace log4cplus;

namespace comlog
{

#define COMLOGINIT(confpath) \
	comlog::initialize(confpath)

#define TRACE(msg) \
	do { \
		LOG4CPLUS_TRACE(comlog::ComlogApp::comlogapp_ptr->getloggerfromloglevel(log4cplus::TRACE_LOG_LEVEL), msg); \
	}while(0)

#define DEBUG(msg) \
	do { \
		LOG4CPLUS_DEBUG(comlog::ComlogApp::comlogapp_ptr->getloggerfromloglevel(log4cplus::DEBUG_LOG_LEVEL), msg); \
	}while(0)

#define INFO(msg) \
	do { \
		LOG4CPLUS_INFO(comlog::ComlogApp::comlogapp_ptr->getloggerfromloglevel(log4cplus::INFO_LOG_LEVEL), msg); \
	}while(0)

#define WARN(msg) \
	do { \
		LOG4CPLUS_WARN(comlog::ComlogApp::comlogapp_ptr->getloggerfromloglevel(log4cplus::WARN_LOG_LEVEL), msg); \
	}while(0)

#define ERROR(msg) \
	do { \
		LOG4CPLUS_ERROR(comlog::ComlogApp::comlogapp_ptr->getloggerfromloglevel(log4cplus::ERROR_LOG_LEVEL), msg); \
	}while(0)

#define FATAL(msg) \
	do { \
		LOG4CPLUS_FATAL(comlog::ComlogApp::comlogapp_ptr->getloggerfromloglevel(log4cplus::FATAL_LOG_LEVEL), msg); \
	}while(0)
}











#endif  //__COMLOGCOMMON_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
