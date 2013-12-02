/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlog.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/06 17:13:52
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "global.h"
#include "comlog.h"

using namespace std;
using namespace comlog;

logger::logger()
{}

logger::~logger()
{}

void logger::getformat(LOG_LEVEL loglevel, string &msgformat)
{

	time_t timeNow;
	struct tm *timeNow_ptr;
	timeNow = time(NULL);
	timeNow_ptr = localtime(&timeNow);
	char Time[50];
	strftime(Time, 50, "%F %T", timeNow_ptr);

	string comloglevel;
	switch (loglevel)
	{
		case COMLOG_DEBUG:
			comloglevel = "[DEBUG]";
			break;
		case COMLOG_TRACE:
			comloglevel = "[TRACE]";
			break;
		case COMLOG_INFO:
			comloglevel = "[INFO]";
			break;
		case COMLOG_WARN:
			comloglevel = "[WARN]";
			break;
		case COMLOG_ERROR:
			comloglevel = "[ERROR]";
			break;
		default:
			break;
	}

	msgformat = Time;
	msgformat += " " + comloglevel + " ";

	return;

}

#define MAX_SIZE_PRINT_BUFFER	1024*10

void logger::printlog(FILE *fp, const char *msg)
{
	try
	{
		if( NULL == fp )
		{
			throw "logger::printlog fp is NULL.";
		}
		if( NULL == msg )
		{
			throw "logger::printlog msg is NULL.";
		}

		fputs(msg, fp);        /* 输出到文件流 */
		fflush(fp);
	}
	catch(...)
	{
		cout << "logger::printlog exception happened." << endl;
		return;
	}

	return;
}

void configure::setFILE()
{
	time_t timeNow;
	struct tm *timeNow_ptr;
	timeNow = time(NULL);
	timeNow_ptr = localtime(&timeNow);
	char logname[1024] = {0};
	char Time[50] = {0};
	char nowsecond[1024] = {0};
	strftime(logname, 1024, _logfile.c_str(), timeNow_ptr);
	strftime(Time, 50, "%F %T", timeNow_ptr);
	strftime(nowsecond, 1024, "%s", timeNow_ptr);

	//创建日志的最后一级目录文件夹
	string tempdir;
	string templogname = logname;
	for(string::iterator iter = templogname.begin(); iter != templogname.end(); ++iter)
	{
		tempdir.push_back((*iter));
		if('/' != (*iter))
		{
			continue;
		}
		else
		{
			ismkdir(tempdir);
		}
	}

	//判断日志是否是当前的日志
	//是的话则附加的形式打开
	//不是的话清空从头开始输入
	if( NULL == _fp )
	{
		//第一次使用init的时候的分支
		_fp = fopen(logname, "a");
		assert(NULL != _fp);
		string msg = Time;
		msg += "--------------------------------------INIT-------------------------------------------\n";
		_logger->printlog(_fp, msg.c_str());
		_logger->printlog(_fp, msg.c_str());
		_logger->printlog(_fp, msg.c_str());
		return;
	}
	else
	{
		if((access(logname, F_OK) != 0))
		{
			fclose(_fp);
			_fp = fopen(logname, "a");
		}
		struct stat fileinfo;
		stat(logname, &fileinfo);
		long *filesecond = &fileinfo.st_atime;
		if(atoi(nowsecond) - *filesecond >= 3600)
		{
			fclose(_fp);
			_fp = fopen(logname, "w+");
		}
	}

	try
	{
		if( NULL == _fp )
		{
			throw "configure::logfilethread open file failed.";
		}
	}
	catch(...)
	{
		cout << "open file error." << endl;
	}
}

void configure::setWFFILE()
{

	time_t timeNow;
	struct tm *timeNow_ptr;
	timeNow = time(NULL);
	timeNow_ptr = localtime(&timeNow);
	char logwfname[1024] = {0};
	char Time[50] = {0};
	char nowsecond[1024] = {0};
	strftime(logwfname, 1024, _logwffile.c_str(), timeNow_ptr);
	strftime(Time, 50, "%F %T", timeNow_ptr);
	strftime(nowsecond, 1024, "%s", timeNow_ptr);
	
	//创建日志的最后一级目录文件夹
	string tempdir;
	string templogwfname = logwfname;
	for(string::iterator iter = templogwfname.begin(); iter != templogwfname.end(); ++iter)
	{
		tempdir.push_back((*iter));
		if('/' != (*iter))
		{
			continue;
		}
		else
		{
			ismkdir(tempdir);
		}
	}

	//判断日志是否是当前的日志
	//是的话则附加的形式打开
	//不是的话清空从头开始输入
	if( NULL == _wf_fp )
	{
		//第一次使用init的时候的分支
		_wf_fp = fopen(logwfname, "a");
		assert(NULL != _wf_fp);
		string msg = Time;
		msg += "--------------------------------------INIT-------------------------------------------\n";
		_logger->printlog(_wf_fp, msg.c_str());
		_logger->printlog(_wf_fp, msg.c_str());
		_logger->printlog(_wf_fp, msg.c_str());
		return;
	}
	else
	{
		if((access(logwfname, F_OK) != 0))
		{
			fclose(_wf_fp);
			_wf_fp = fopen(logwfname, "a");
		}
		struct stat fileinfo;
		stat(logwfname, &fileinfo);
		long *filesecond = &fileinfo.st_atime;
		if(atoi(nowsecond) - *filesecond >= 3600)
		{
			fclose(_wf_fp);
			_wf_fp = fopen(logwfname, "w+");
		}
	}

	try
	{
		if( NULL == _wf_fp )
		{
			throw "configure::logwffilethread open file failed.";
		}
	}
	catch(...)
	{
		cout << "open file error." << endl;
	}
}

configure::configure(const string &confpath)
{
	try
	{
		if( confpath.empty() )
		{
			throw "configure::configure confpath is NULL";
		}

		_fp = NULL;
		_wf_fp = NULL;
		_conf_path = confpath;
		_logger = new logger();
	}
	catch (...)
	{
		cout << "configure::configure error." << endl;
	}
}

configure::~configure()
{
	if(NULL != _fp)
		fclose(_fp);
	if(NULL != _wf_fp)
		fclose(_wf_fp);
}

bool configure::init()
{
	try
	{
		if( !loadconf() )
		{
			throw "configure::inti loadconf error.";
		}	
	}
	catch(...)
	{
		cout << "configure::init error." << endl;
		return false;
	}

	return true;
}

void configure::getconfValue(const string confline, LOG_LEVEL &result)
{
	assert( !confline.empty() );

	int pos = confline.find(":");
	string temp = confline.substr(pos+1);

	string ret;
	for(string::iterator iter = temp.begin(); iter != temp.end(); ++iter)
	{
		if( isspace(*iter) )
			continue;
		else
		{
			ret.push_back((*iter));
		}
	}

	result = (LOG_LEVEL)atoi(ret.c_str());

	return;

}

void configure::getconfValue(const string confline, string &result)
{

	assert(!confline.empty());

	int pos = confline.find(":");
	string temp = confline.substr(pos+1);

	for(string::iterator iter = temp.begin(); iter != temp.end(); ++iter)
	{
		if( isspace(*iter) )
			continue;
		else
		{
			result.push_back( (*iter) );
		}
	}

	return;

}

bool configure::loadconf()
{
	char *buffer = NULL;

	try
	{
		FILE *fp;
		fp = fopen(_conf_path.c_str(), "r");

		if(NULL == fp)
		{
			throw "configure::loadconf open conf file error.";
		}

		string logdir;
		string logname;
		string logwfname;
		ssize_t read;
		size_t	len = 0;
		while( (read = getline(&buffer, &len, fp)) != -1 )
		{

			string tempstring = buffer;

			if( -1 != tempstring.find("COMLOG_DIR"))
			{
				getconfValue(tempstring, logdir);
			}
			if( -1 != tempstring.find("COMLOG_LOGNAME"))
			{
				getconfValue(tempstring, logname);
			}
			if( -1 != tempstring.find("COMLOG_WF_LOGNAME"))
			{
				getconfValue(tempstring, logwfname);
			}
			if( -1 != tempstring.find("COMLOG_LEVEL"))
			{
				getconfValue(tempstring, _loglevel);
			}
			if( -1 != tempstring.find("COMLOG_WF_LOGLEVEL"))
			{
				getconfValue(tempstring, _logwflevel);
			}

			if(buffer)
			{
				free(buffer);
				buffer = NULL;
			}
		}

		fclose(fp);

		setlogfile(logdir, logname);
		setlogwffile(logdir, logwfname);
		ismkdir(logdir);
	}
	catch(...)
	{
		cout << "configure::loadconf error happened." << endl;
		return false;
	}

	return true;

}

bool configure::setlogfile(const string &logdir, const string &logname)
{
	try
	{
		if(logdir.empty() || logname.empty())
		{
			throw "configure::setlogfile logdir or logname is NULL.";
		}

		_logfile = logdir + "/" + logname;
	}
	catch(...)
	{
		cout << "configure::setlogfile error happened." << endl;
		return false;
	}

	return true;

}

bool configure::setlogwffile(const string &logdir, const string &logwfname)
{
	try
	{
		if(logdir.empty() || logwfname.empty())
		{
			throw "configure::setlogwffile logdir or logwfname is NULL.";
		}

		_logwffile = logdir + "/" + logwfname;
	}
	catch(...)
	{
		cout << "configure::setlogwffile error happened." << endl;
		return false;
	}

	return true;
}

void configure::ismkdir(const string &logdir)
{
	if ( (access(logdir.c_str(), F_OK) != 0) )
	{
		string mkdircmd = "mkdir -p " + logdir;
		system(mkdircmd.c_str());
	}

	return;
}

void configure::print(LOG_LEVEL loglevel, const char *msg, ...)
{
	try
	{
		string msgformat;
		_logger->getformat(loglevel, msgformat);

		char szBuf[MAX_SIZE_PRINT_BUFFER];
		va_list _va_list;
		msgformat += msg;
		va_start(_va_list, (const char*)msgformat.c_str());      /* 初始化变长参数列表 */
		vsprintf(szBuf, msgformat.c_str(), _va_list);    /* 传递变长参数 */
		va_end(_va_list);         /* 结束使用变长参数列表 */

		switch(loglevel)
		{
			case COMLOG_DEBUG:
				setFILE();
				_logger->printlog(_fp, szBuf);
				break;
			case COMLOG_TRACE:
				setFILE();
				_logger->printlog(_fp, szBuf);
				break;
			case COMLOG_INFO:
				setFILE();
				_logger->printlog(_fp, szBuf);
				break;
			case COMLOG_WARN:
				setWFFILE();
				_logger->printlog(_wf_fp, szBuf);
				break;
			case COMLOG_ERROR:
				setWFFILE();
				_logger->printlog(_wf_fp, szBuf);
				break;
			default:
				throw "configure::print loglevel error.";
		}
	}
	catch(...)
	{
		cout << "configure::print error happened.";
		return;
	}

	return;
}























/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
