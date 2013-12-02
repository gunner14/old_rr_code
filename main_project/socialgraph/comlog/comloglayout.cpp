/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comloglayout.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/17 14:50:22
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <string>

#include <assert.h>

#include "global.h"
#include "comloglayout.h"

using namespace log4cplus;
using namespace comlog;

ComlogLayout::ComlogLayout()
{
	pthread_rwlock_init(&device_pattern_lock, NULL);
}

ComlogLayout::~ComlogLayout()
{
	pthread_rwlock_destroy(&device_pattern_lock);
}

bool ComlogLayout::addpattern(const std::string &name, std::string pattern)
{
	assert(!name.empty());

	AutoRWlock _device_lock(&device_pattern_lock);

	log4cplus::Layout *_layout = new log4cplus::PatternLayout(pattern);
	std::pair<std::map<std::string, log4cplus::Layout*>::iterator, bool> pr = 
						device_pattern_map.insert(std::pair<std::string, log4cplus::Layout*>(name, _layout));

	return pr.second;
}

bool ComlogLayout::delpattern(const std::string &name)
{
	assert(!name.empty());
	AutoRWlock _device_lock(&device_pattern_lock);
	
	std::map<std::string, log4cplus::Layout*>::iterator iter = device_pattern_map.find(name);
	if (iter != device_pattern_map.end())
	{
		delete iter->second;
		device_pattern_map.erase(name);
		return true;
	}
	else
	{
		return false;
	}
}

std::auto_ptr<log4cplus::Layout> ComlogLayout::getlayout(const std::string &name)
{
	assert(!name.empty());

	std::map<std::string, log4cplus::Layout*>::iterator iter = device_pattern_map.find(name);

	if (iter != device_pattern_map.end())
	{
		return (std::auto_ptr<log4cplus::Layout>)iter->second;
	}
	else
	{
		return (std::auto_ptr<log4cplus::Layout>)NULL;
	}
}

uint32_t ComlogLayout::getdevicenum()
{
	return device_pattern_map.size();
}








/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
