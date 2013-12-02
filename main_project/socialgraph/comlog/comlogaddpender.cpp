/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogaddpender.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/09/18 18:04:22
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <assert.h>

#include "comlogaddpender.h"
#include "global.h"

using namespace log4cplus;
using namespace comlog;

static void makelogdir(const log4cplus::tstring &logfile)
{
	log4cplus::tstring templogfile(logfile);

	size_t pos = templogfile.find('/');
	log4cplus::tstring templogdir;

	do
	{
		pos = templogfile.find('/');
		templogdir += templogfile.substr(0, pos);
		templogdir += '/';
		
		if ((access(templogdir.c_str(), F_OK) != 0))
		{
			log4cplus::tstring mkdircmd = "mkdir -p " + templogdir;
			system(mkdircmd.c_str());
		}
		
		templogfile = templogfile.substr(pos + 1);

	} while(templogfile.find('/') != -1);
}

static log4cplus::tstring getfilename(const log4cplus::tstring &logfile)
{
	time_t timeNow;	
	struct tm *timeNow_ptr;	
	timeNow = time(NULL);	
	timeNow_ptr = localtime(&timeNow);	
	char templogfile[1024];	
	strftime(templogfile, 1024, logfile.c_str(), timeNow_ptr);

	log4cplus::tstring templogfile_str = templogfile;
	return templogfile_str;
}

static log4cplus::tstring getfiledir(const log4cplus::tstring &logfile)
{
	// ./log/TEST/%w/%H-testlog ===>  return ./log/TEST/
	if (-1 != logfile.find('%'))
	{
		size_t pos = logfile.find('%');
		return logfile.substr(0, pos);
	}

	// ./log/TEST/testlog ===> return ./log/TEST/
	log4cplus::tstring templogfile(logfile);
	log4cplus::tstring templogdir;
	size_t pos = templogfile.find('/');
	
	while (-1 != pos)
	{
		templogdir += templogfile.substr(0, pos);
		templogfile = templogfile.substr(pos + 1);
		pos = templogfile.find('/');
	}

	// .testlog ===> return ./
	if (templogdir.empty())
	{
		templogdir += "./";
	}

	return templogdir;
}

ComlogRollingAppender::ComlogRollingAppender(const log4cplus::tstring &logfile, const log4cplus::tstring &rawlogfile, bool immediateFlush) 
											: FileAppender(logfile, std::ios::app, immediateFlush)
{
	init(rawlogfile);
}

ComlogRollingAppender::~ComlogRollingAppender()
{
	destructorImpl();
}

void ComlogRollingAppender::close()
{
	FileAppender::close();
}

void ComlogRollingAppender::append(const log4cplus::spi::InternalLoggingEvent &event)
{
	if (checklogfile())
	{
		rollover(true);
	}

	FileAppender::append(event);
}

bool ComlogRollingAppender::checklogfile()
{
	//用当前时间与rollinglogfile进行对比

	log4cplus::tstring templogfile = getfilename(this->_raw_filename);
	return (0 != templogfile.compare(this->rollinglogfile) ? true : false);
}

void ComlogRollingAppender::open(std::ios::openmode mode)
{
	out.open(filename.c_str(), mode);
}

void ComlogRollingAppender::rollover(bool alreadyLocked)
{
	//close current file
	out.close();

	// Reset flags since the C++ standard specified that all the flags
	// should remain unchanged on a close.
	out.clear();

	log4cplus::tstring tempfilename = getfilename(this->_raw_filename);
	filename = tempfilename;
	makelogdir(filename);

	open(std::ios::out | std::ios::trunc);

	this->rollinglogfile = tempfilename;
}

void ComlogRollingAppender::init(const log4cplus::tstring &logfile)
{
	this->_raw_filename = logfile;

	this->logdir = getfiledir(logfile);
	this->rollinglogfile = getfilename(logfile);
}

ComlogAddpender::ComlogAddpender()
{
	pthread_rwlock_init(&device_addpender_lock, NULL);
}

ComlogAddpender::~ComlogAddpender()
{
	pthread_rwlock_destroy(&device_addpender_lock);
}

bool ComlogAddpender::addaddpender(const std::string &name, const std::string &logname, const std::string &type, const std::string logpath,
		          log4cplus::DailyRollingFileSchedule DRFschedule, int drf_max_backup_index, long rf_max_file_size, int rf_max_backup_index,
				  std::ios_base::openmode mode, bool immediateFlush)
{
	assert(!name.empty());
	assert(!logname.empty());
	assert(!type.empty());

	if (type == ADDPENDER_COMLOGROLLINGFILE)
	{
		if (!addcomlogrollingfile(name, logname, logpath, immediateFlush))
		{
			return false;
		}
	}
	else if (type == ADDPENDER_DAILYROLLINGFILE)
	{
		if (!adddailyrollingfile(name, logname, logpath, DRFschedule, drf_max_backup_index, immediateFlush))
		{
			return false;
		}
	}
	else if (type == ADDPENDER_ROLLINGFILE)
	{
		if (!addrollingfile(name, logname, logpath, rf_max_file_size, rf_max_backup_index, immediateFlush))
		{
			return false;
		}
	}
	else if (type == ADDPENDER_FILE)
	{
		if (!addfile(name, logname, logpath, mode, immediateFlush))
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

bool ComlogAddpender::addcomlogrollingfile(const std::string &name, const std::string &logname, std::string logpath, bool immediateFlush)
{
	assert(!name.empty());
	assert(!logname.empty());

	AutoRWlock _device_lock(&device_addpender_lock);
	std::string logfile = logpath + "/" + logname;
	
	log4cplus::tstring templogfile_str = getfilename(logfile);
	makelogdir(templogfile_str);

	log4cplus::SharedAppenderPtr _addpender(new ComlogRollingAppender(templogfile_str, logfile, immediateFlush));
	_addpender->setName(name);

	struct AddpenderType _atype;
	_atype.addpendertype = "ComlogRollingFile";
	_atype.addpenderptr = _addpender;

	std::pair<std::map<std::string, struct AddpenderType>::iterator, bool> pr = 
								device_addpender_map.insert(std::pair<std::string, struct AddpenderType>(name, _atype));

	return pr.second;
}

bool ComlogAddpender::adddailyrollingfile(const std::string &name, const std::string &logname, std::string logpath,
										  log4cplus::DailyRollingFileSchedule DRFschedule, uint32_t max_backup_index, bool immediateFlush)
{
	assert(!name.empty());
	assert(!logname.empty());

	AutoRWlock _device_lock(&device_addpender_lock);
	std::string logfile = logpath + "/" + logname;
	log4cplus::SharedAppenderPtr _addpender(new DailyRollingFileAppender(logfile, DRFschedule, immediateFlush, max_backup_index));
	_addpender->setName(name);

	struct AddpenderType _atype;
	_atype.addpendertype = "DailyRollingFile";
	_atype.addpenderptr = _addpender;

	std::pair<std::map<std::string, struct AddpenderType>::iterator, bool> pr = 
								device_addpender_map.insert(std::pair<std::string, struct AddpenderType>(name, _atype));

	return pr.second;
}

bool ComlogAddpender::addrollingfile(const std::string &name, const std::string &logname, std::string logpath, 
									 long max_file_size, uint32_t max_backup_index, bool immediateFlush) 
{
	assert(!name.empty());
	assert(!logname.empty());

	AutoRWlock _device_lock(&device_addpender_lock);
	std::string logfile = logpath + "/" + logname;
	log4cplus::SharedAppenderPtr _addpender(new RollingFileAppender(logfile, max_file_size, max_backup_index, immediateFlush));
	_addpender->setName(name);

	struct AddpenderType _atype;
	_atype.addpendertype = "RollingFile";
	_atype.addpenderptr = _addpender;

	std::pair<std::map<std::string, struct AddpenderType>::iterator, bool> pr = 
								device_addpender_map.insert(std::pair<std::string, struct AddpenderType>(name, _atype));

	return pr.second;
}

bool ComlogAddpender::addfile(const std::string &name, const std::string &logname, std::string logpath,
							  std::ios_base::openmode mode, bool immediateFlush)
{
	assert(!name.empty());
	assert(!logname.empty());

	AutoRWlock _device_lock(&device_addpender_lock);
	std::string logfile = logpath + "/" + logname;
	log4cplus::SharedAppenderPtr _addpender(new FileAppender(logfile, mode, immediateFlush));

	struct AddpenderType _atype;
	_atype.addpendertype = "File";
	_atype.addpenderptr = _addpender;

	std::pair<std::map<std::string, struct AddpenderType>::iterator, bool> pr = 
								device_addpender_map.insert(std::pair<std::string, struct AddpenderType>(name, _atype));

	return pr.second;
}

bool ComlogAddpender::deladdpender(const std::string &name)
{
	assert(!name.empty());

	AutoRWlock _device_lock(&device_addpender_lock);
	std::map<std::string, struct AddpenderType>::iterator iter = device_addpender_map.find(name);

	if (iter == device_addpender_map.end())
	{
		return false;
	}
	else
	{
		device_addpender_map.erase(iter);
		return true;
	}
}

bool ComlogAddpender::getaddpender(const std::string &name, log4cplus::SharedAppenderPtr &addpenderptr)
{
	assert(!name.empty());

	AutoRWlock _getaddpender(&device_addpender_lock);
	std::map<std::string, struct AddpenderType>::iterator iter = device_addpender_map.find(name);

	if (iter == device_addpender_map.end())
	{
		addpenderptr = NULL;
		return false;
	}

	addpenderptr = iter->second.addpenderptr;
	return true;
}

bool ComlogAddpender::setlayout(const std::string &name, std::auto_ptr<log4cplus::Layout> layoutptr)
{
	assert(!name.empty());

	AutoRWlock _setlayout(&device_addpender_lock);
	std::map<std::string, struct AddpenderType>::iterator iter = device_addpender_map.find(name);

	if (iter == device_addpender_map.end())
	{
		return false;
	}
	iter->second.addpenderptr->setLayout(layoutptr);

	return true;
}

bool ComlogAddpender::setfilter(const std::string &name, const log4cplus::spi::FilterPtr &filterptr)
{
	assert(!name.empty());

	AutoRWlock _setfilter(&device_addpender_lock);
	std::map<std::string, struct AddpenderType>::iterator iter = device_addpender_map.find(name);

	if (iter == device_addpender_map.end())
	{
		return false;
	}
	iter->second.addpenderptr->setFilter(filterptr);

	log4cplus::spi::FilterPtr tempfilter;
	tempfilter = iter->second.addpenderptr->getFilter();

	return true;
}

bool ComlogAddpender::getfilter(const std::string &name, log4cplus::spi::FilterPtr &filterptr)
{
	assert(!name.empty());

	AutoRWlock _getfilter(&device_addpender_lock);
	std::map<std::string, struct AddpenderType>::iterator iter = device_addpender_map.find(name);
	if (iter == device_addpender_map.end())
	{
		filterptr = NULL;
		return false;
	}

	filterptr = iter->second.addpenderptr->getFilter();
	return true;
}













/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
