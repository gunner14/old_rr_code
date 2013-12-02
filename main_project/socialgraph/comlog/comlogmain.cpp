/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogmain.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/24 10:19:17
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <assert.h>
#include <string.h>

#include <string>

#include <log4cplus/loggingmacros.h>

#include "comlogmain.h"

using namespace log4cplus;
using namespace comlog;

ComlogApp* ComlogApp::comlogapp_ptr = NULL;

ComlogApp::ComlogApp()
{
	_rootpattern = "%d %p [%l] %m%n";
}

ComlogApp::~ComlogApp()
{}

ComlogApp* ComlogApp::instance()
{
	if (!comlogapp_ptr)
	{
		comlogapp_ptr = new	ComlogApp();
	}

	return comlogapp_ptr;
}

bool ComlogApp::init(const std::string &confpath)
{
	assert(!confpath.empty());
	
	//加载配置
	comlogconfig = new ComlogConfig();
	if (!comlogconfig->init(confpath))
	{
		return false;
	}

	//不使用父logger进行打印，为每个配置设置一个logger，通过getInstance来创建
	//通过总的loglevel为每个子logger进行loglevel配置
	//initrootlogger(comlogconfig->_comlog_name, comlogconfig->_comlog_level);

	comlogfilter = new ComlogFilter();
	comloglayout = new ComlogLayout();
	comlogaddpender = new ComlogAddpender();

	//设置addpender，filter，layout
	for(int i = 0; i < comlogconfig->_comlog_device_num; i++)
	{
		struct ConfigItem configitem;
		bzero(&configitem, 0);

		if (!comlogconfig->getItem(i, configitem))
		{
			return false;
		}
	
		//添加layout
		if (!comloglayout->addpattern(configitem._name, configitem._layout))
		{
			return false;
		}

		//设置filter
		if (!comlogfilter->addfilter(configitem._name, configitem._filter, configitem._filter_loglevel, 
									 configitem._filter_loglevelmin, configitem._filter_loglevelmax, 
									 configitem._filter_stringmatch))
		{
			return false;
		}

		//添加appender
		if (!comlogaddpender->addaddpender(configitem._name, configitem._filename, configitem._addpender, 
										   configitem._path, configitem._addpender_DRFscheduler, configitem._addpender_DRFbackupindex, 
										   configitem._addpender_RFfilesize, configitem._addpender_RFbackupindex, configitem._addpender_Filemode))
		{
			return false;
		}
		//将layout设置给appender
		if (!comlogaddpender->setlayout(configitem._name, comloglayout->getlayout(configitem._name)))
		{
			return false;
		}
		//将filter设置给appender
		if (!comlogaddpender->setfilter(configitem._name, comlogfilter->getfilter(configitem._name)))
		{
			return false;
		}

		//配置子logger,每个子logger的级别通过总的loglevel来进行配置
		if (!addlogger(configitem._name, configitem._selflevelset, comlogconfig->_comlog_level))
		{
			return false;
		}
		log4cplus::SharedAppenderPtr addpenderptr;
		if (!comlogaddpender->getaddpender(configitem._name, addpenderptr))
		{
			return false;
		}
		if (!addloggeraddpender(configitem._name, addpenderptr))
		{
			return false;
		}
	}

	return true;
}

bool ComlogApp::addlogger(const std::string &name, std::set<log4cplus::LogLevel> loglevelset, log4cplus::LogLevel loglevel)
{
	assert(!name.empty());

	struct LoggerType loggertype;
	loggertype.logger = log4cplus::Logger::getInstance(name);
	loggertype.logger.setLogLevel(loglevel);
	loggertype.loglevelset = loglevelset;
	std::pair<std::map<std::string, struct LoggerType>::iterator, bool> pr = _sublogger_map.insert(std::pair<std::string, struct LoggerType>(name, loggertype));

	return pr.second;
}

bool ComlogApp::addloggeraddpender(const std::string name, log4cplus::SharedAppenderPtr addpenderptr)
{
	assert(!name.empty());

	std::map<std::string, struct LoggerType>::iterator iter = _sublogger_map.find(name);
	if (iter == _sublogger_map.end())
	{
		return false;
	}

	iter->second.logger.addAppender(addpenderptr);
	return true;
}

log4cplus::Logger ComlogApp::getloggerfromloglevel(log4cplus::LogLevel loglevel)
{
	for(std::map<std::string, struct LoggerType>::iterator iter = _sublogger_map.begin(); iter != _sublogger_map.end(); ++iter)
	{
		std::set<log4cplus::LogLevel>::iterator subiter = iter->second.loglevelset.find(loglevel);
		if (subiter != iter->second.loglevelset.end())
		{
			return iter->second.logger;
		}
	}
}

void ComlogApp::initrootlogger(const std::string &name, log4cplus::LogLevel loglevel)
{
	_root = Logger::getInstance(name);
	_root.setLogLevel(loglevel);

	//root默认采用fileappender
	std::string rootfile = "./" + name + ".log"; 
	std::ios_base::openmode mode = std::ios_base::app;
	bool immediateFlush = true;
	log4cplus::SharedAppenderPtr _rootaddpender(new FileAppender(rootfile, mode, immediateFlush));

	log4cplus::Layout* _rootlayout = new log4cplus::PatternLayout(_rootpattern);
	_rootaddpender->setLayout((std::auto_ptr<log4cplus::Layout>)_rootlayout);

	_root.addAppender(_rootaddpender);
}













/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
