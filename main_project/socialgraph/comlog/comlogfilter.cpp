/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogfilter.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/24 09:27:41
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <assert.h>

#include <string>

#include "comlogfilter.h"
#include "global.h"

using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace log4cplus::spi;
using namespace comlog;

ComlogNoneFilter::ComlogNoneFilter()
{}

ComlogNoneFilter::ComlogNoneFilter(const log4cplus::helpers::Properties &properties)
{}

log4cplus::spi::FilterResult ComlogNoneFilter::decide(const log4cplus::spi::InternalLoggingEvent& event) const
{
	return log4cplus::spi::ACCEPT;
}

ComlogLogLevelMatchFilter::ComlogLogLevelMatchFilter()
{
	init();
}

ComlogLogLevelMatchFilter::ComlogLogLevelMatchFilter(const log4cplus::helpers::Properties &properties)
{
	init();

	if ("true" == properties.getProperty(LOG4CPLUS_TEXT("AcceptOnMatch")))
	{
		acceptOnMatch = true;
	}
	else
	{
		acceptOnMatch = false;
	}
	log4cplus::tstring const &log_level_to_match = properties.getProperty( LOG4CPLUS_TEXT("LogLevelToMatch") );
	logLevelToMatch = getLogLevelManager().fromString(log_level_to_match);
}

void ComlogLogLevelMatchFilter::init()
{
	acceptOnMatch = true;
	logLevelToMatch = log4cplus::NOT_SET_LOG_LEVEL;
}

log4cplus::spi::FilterResult ComlogLogLevelMatchFilter::decide(const log4cplus::spi::InternalLoggingEvent& event) const
{
	if (logLevelToMatch == log4cplus::NOT_SET_LOG_LEVEL)
	{
		return log4cplus::spi::NEUTRAL;
	}

	bool matchOccured = (logLevelToMatch == event.getLogLevel());

	if (matchOccured && acceptOnMatch)
	{
		return log4cplus::spi::ACCEPT;
	}
	else
	{
		return log4cplus::spi::DENY;
	}
}

ComlogStringMatchFilter::ComlogStringMatchFilter()
{
	init();
}

ComlogStringMatchFilter::ComlogStringMatchFilter(const log4cplus::helpers::Properties &properties)
{
	init();

	if ("true" == properties.getProperty(LOG4CPLUS_TEXT("AcceptOnMatch")))
	{
		acceptOnMatch = true;
	}
	else
	{
		acceptOnMatch = false;
	}
	stringToMatch = properties.getProperty( LOG4CPLUS_TEXT("StringToMatch") );
}

void ComlogStringMatchFilter::init()
{
	acceptOnMatch = true;
}

log4cplus::spi::FilterResult ComlogStringMatchFilter::decide(const log4cplus::spi::InternalLoggingEvent& event) const
{
	const log4cplus::tstring& message = event.getMessage();
	if (stringToMatch.empty () || message.empty ()) 
	{
		return log4cplus::spi::DENY;
	}
	
	if (message.find(stringToMatch) == tstring::npos) 
	{
		return log4cplus::spi::DENY;
	}
	else
	{
		return (acceptOnMatch ? log4cplus::spi::ACCEPT : log4cplus::spi::DENY);
	}
}

ComlogFilter::ComlogFilter()
{
	pthread_rwlock_init(&device_filter_lock, NULL);
}

ComlogFilter::~ComlogFilter()
{
	pthread_rwlock_destroy(&device_filter_lock);
}

bool ComlogFilter::addfilter(const std::string &name, const std::string &type, log4cplus::LogLevel loglevel,
			   log4cplus::LogLevel loglevelmin, log4cplus::LogLevel loglevelmax, std::string stringmatch)
{
	assert(!name.empty());
	assert(!type.empty());

	if (type == FILTER_NONEFILTER)
	{
		if (!addnonefilter(name))
		{
			return false;
		}
	}
	else if (type == FILTER_DENYFILTER)
	{
		if (!adddenyfilter(name))
		{
			return false;
		}
	}
	else if (type == FILTER_LOGLEVELMATCHFILTER)
	{
		if (!addloglevelmatchfilter(name, DEFAULT_ACCEPTONMATCH, loglevel))
		{
			return false;
		}
	}
	else if (type == FILTER_LOGLEVELRANGEFILTER)
	{
		if (!addloglevelrangefilter(name, DEFAULT_ACCEPTONMATCH, loglevelmin, loglevelmax))
		{
			return false;
		}
	}
	else if (type == FILTER_STRINGMATCHFILTER)
	{
		if (stringmatch == DEFAULT_STRINGMATCH)
		{
			return false;
		}
		if (!addstringmatchfilter(name, DEFAULT_ACCEPTONMATCH, stringmatch))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool ComlogFilter::addnonefilter(const std::string &name)
{
	assert(!name.empty());
	
	AutoRWlock _adddenyfilter(&device_filter_lock);
	struct FilterType filtertype;
	filtertype.type = FILTER_NONEFILTER;
	filtertype.filterptr = new ComlogNoneFilter();

	std::pair<std::map<std::string, struct FilterType>::iterator, bool> pr = device_filter_map.insert(std::pair<std::string, struct FilterType>(name, filtertype));

	return pr.second;
}

bool ComlogFilter::adddenyfilter(const std::string &name)
{
	assert(!name.empty());

	AutoRWlock _adddenyfilter(&device_filter_lock);
	struct FilterType filtertype;
	filtertype.type = FILTER_DENYFILTER;
	filtertype.filterptr = new log4cplus::spi::DenyAllFilter();

	std::pair<std::map<std::string, struct FilterType>::iterator, bool> pr = device_filter_map.insert(std::pair<std::string, struct FilterType>(name, filtertype));

	return pr.second;
}

bool ComlogFilter::addloglevelmatchfilter(const std::string &name, std::string acceptonmatch, log4cplus::LogLevel logleveltomatch)
{
	assert(!name.empty());

	AutoRWlock _addloglevelmatchfilter(&device_filter_lock);
	struct FilterType filtertype;
	filtertype.type = FILTER_LOGLEVELMATCHFILTER;

	log4cplus::helpers::Properties properties = log4cplus::helpers::Properties();
	properties.setProperty("AcceptOnMatch", acceptonmatch);
	std::string logleveltomatchstr;
	if (!logleveltransfer(logleveltomatch, logleveltomatchstr))
	{
		return false;
	}
	properties.setProperty("LogLevelToMatch", logleveltomatchstr);

	//本应该是log4cplus中的LogLevelMatchFilter，现在重写
	//filtertype.filterptr = new log4cplus::spi::LogLevelMatchFilter(properties);
	filtertype.filterptr = new ComlogLogLevelMatchFilter(properties);

	std::pair<std::map<std::string, struct FilterType>::iterator, bool> pr = device_filter_map.insert(std::pair<std::string, struct FilterType>(name, filtertype));

	return pr.second;
}

bool ComlogFilter::addloglevelrangefilter(const std::string &name, std::string acceptonmatch,
										  log4cplus::LogLevel loglevelmin, log4cplus::LogLevel loglevelmax)
{
	assert(!name.empty());

	AutoRWlock _addloglevelrangefilter(&device_filter_lock);
	struct FilterType filtertype;
	filtertype.type = FILTER_LOGLEVELRANGEFILTER;

	log4cplus::helpers::Properties properties = log4cplus::helpers::Properties();
	properties.setProperty("AcceptOnMatch", acceptonmatch);
	std::string loglevelminstr, loglevelmaxstr;
	if (!logleveltransfer(loglevelmin, loglevelminstr) || !logleveltransfer(loglevelmax, loglevelmaxstr))
	{
		return false;
	}
	properties.setProperty("LogLevelMin", loglevelminstr);
	properties.setProperty("LogLevelMax", loglevelmaxstr);
	filtertype.filterptr = new log4cplus::spi::LogLevelRangeFilter(properties);

	std::pair<std::map<std::string, struct FilterType>::iterator, bool> pr = device_filter_map.insert(std::pair<std::string, struct FilterType>(name, filtertype));

	return pr.second;
}

bool ComlogFilter::addstringmatchfilter(const std::string &name, std::string acceptonmatch, const std::string &stringmatch)
{
	assert(!name.empty());

	AutoRWlock _addstringmatchfilter(&device_filter_lock);
	struct FilterType filtertype;
	filtertype.type = FILTER_STRINGMATCHFILTER;

	log4cplus::helpers::Properties properties = log4cplus::helpers::Properties();
	properties.setProperty("AcceptOnMatch", acceptonmatch);
	properties.setProperty("StringToMatch", stringmatch);
	//filtertype.filterptr = new log4cplus::spi::StringMatchFilter(properties);
	filtertype.filterptr = new ComlogStringMatchFilter(properties);

	std::pair<std::map<std::string, struct FilterType>::iterator, bool> pr = device_filter_map.insert(std::pair<std::string, struct FilterType>(name, filtertype));

	return pr.second;
}

bool ComlogFilter::delfilter(const std::string &name)
{
	assert(!name.empty());

	AutoRWlock _delfilter(&device_filter_lock);
	std::map<std::string, struct FilterType>::iterator iter = device_filter_map.find(name);
	if (iter == device_filter_map.end())
	{
		return false;
	}

	device_filter_map.erase(iter);
	return true;
}

log4cplus::spi::FilterPtr ComlogFilter::getfilter(const std::string &name)
{
	assert(!name.empty());

	AutoRWlock _getfilter(&device_filter_lock);
	std::map<std::string, struct FilterType>::iterator iter = device_filter_map.find(name);

	return iter->second.filterptr;
}

bool ComlogFilter::logleveltransfer(log4cplus::LogLevel loglevel, std::string &loglevelstr)
{
	switch (loglevel)
	{
		case log4cplus::TRACE_LOG_LEVEL:
			loglevelstr = "TRACE";
			break;
		case log4cplus::DEBUG_LOG_LEVEL:
			loglevelstr = "DEBUG";
			break;
		case log4cplus::INFO_LOG_LEVEL:
			loglevelstr = "INFO";
			break;
		case log4cplus::WARN_LOG_LEVEL:
			loglevelstr = "WARN";
			break;
		case log4cplus::ERROR_LOG_LEVEL:
			loglevelstr = "ERROR";
			break;
		case log4cplus::FATAL_LOG_LEVEL:
			loglevelstr = "FATAL";
			break;
		default:
			return false;
	}

	return true;
}














/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
