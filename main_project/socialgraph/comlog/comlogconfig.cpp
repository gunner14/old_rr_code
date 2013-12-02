/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file comlogconfig.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/24 18:38:54
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <string>
#include <map>
#include <set>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "comlogconfig.h"
#include "comlogaddpender.h"
#include "comlogfilter.h"

using namespace log4cplus;
using namespace comlog;

ComlogConfig::ComlogConfig()
{}

ComlogConfig::~ComlogConfig()
{}

bool ComlogConfig::init(const std::string &confpath)
{
	assert(!confpath.empty());

	if (!genkeyvalue(confpath))
	{
		return false;
	}

	if (!loadkeyvalue())
	{
		return false;
	}

	return true;
}

bool ComlogConfig::getItem(uint32_t num, ConfigItem &configitem)
{
	if (num > _comlog_device_num)
	{
		return false;
	}

	//num = 0
	std::string key = genstr(COMLOG_DEVICE ,num);
	//key = COMLOG_DEVICE0
	std::string value;
	if (!getvalue(key, value))
	{
		return false;
	}

	//value = NORMLOG
	std::string name;
	if (!getvalue(genstr(value, COMLOG_SUBDEVIEC_NAME), name))
	{
		return false;
	}
	
	std::map<std::string, struct ConfigItem>::iterator iter = comlog_device_map.find(name);
	if (iter == comlog_device_map.end())
	{
		return false;
	}

	configitem = iter->second;

	return true;
}

bool ComlogConfig::loadkeyvalue()
{
	if (!getvalue(COMLOG_PROCNAME, _comlog_name))
	{
		return false;
	}
	
	std::string comlog_num_str;
	if (!getvalue(COMLOG_DEVICE_NUM, comlog_num_str))
	{
		return false;
	}
	_comlog_device_num = (uint32_t)atoi(comlog_num_str.c_str());

	std::string comlog_level_str;
	if (!getvalue(COMLOG_LEVEL, comlog_level_str))
	{
		//默认值为全部都打开
		_comlog_level = log4cplus::ALL_LOG_LEVEL;
	}
	else
	{
		if (!transferloglevel(comlog_level_str, _comlog_level))
		{
			_comlog_level = log4cplus::ALL_LOG_LEVEL;
		}
	}

	std::string tempkey, tempvalue;
	for(uint32_t i = 0; i < _comlog_device_num; i++)
	{
		tempkey = genstr(COMLOG_DEVICE, i);
		if (!getvalue(tempkey, tempvalue))
		{
			return false;
		}

		struct ConfigItem configitem; 
		//DEVICE_NAME
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_NAME);
		if (!getvalue(tempkey, configitem._name))
		{
			return false;
		}
		//DEVICE_LAYOUT
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_LAYOUT);
		if (!getvalue(tempkey, configitem._layout))
		{
			configitem._layout = COMLOG_DEFAULT_LAYOUT;
		}
		//DEVICE_PATH
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_PATH);
		if (!getvalue(tempkey, configitem._path))
		{
			return false;
		}
		//DEVICE_FILENAME
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_FILENAME);
		if (!getvalue(tempkey, configitem._filename))
		{
			return false;
		}
		//DEVICE_APPENDER
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_APPENDER);
		if (!getvalue(tempkey, configitem._addpender))
		{
			return false;
		}
		//DEVICE_APPENDER_DRFSCHEDULER
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_APPENDER_DRFSCHEDULER);
		if (!getvalue(tempkey, configitem._addpender_DRFscheduler))
		{
			configitem._addpender_DRFscheduler = log4cplus::DAILY;
		}
		//DEVICE_APPENDER_DRFBACKUPINDEX
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_APPENDER_DRFBACKUPINDEX);
		if (!getvalue(tempkey, configitem._addpender_DRFbackupindex))
		{
			configitem._addpender_DRFbackupindex = DRF_MAX_BACKUP_INDEX;
		}
		//DEVICE_APPENDER_RFFILESIZE
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_APPENDER_RFFILESIZE);
		if (!getvalue(tempkey, configitem._addpender_RFfilesize))
		{
			configitem._addpender_RFfilesize = RF_MAX_FILE_SIZE;
		}
		//DEVICE_APPENDER_RFBACKUPINDEX
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_APPENDER_RFBACKUPINDEX);
		if (!getvalue(tempkey, configitem._addpender_RFbackupindex))
		{
			configitem._addpender_RFbackupindex = RF_MAX_BACKUP_INDEX;
		}
		//DEVICE_APPENDER_FILEMODE
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_APPENDER_FILEMODE);
		if (!getvalue(tempkey, configitem._addpender_Filemode))
		{
			configitem._addpender_Filemode = F_FILEMODE;
		}
		//DEVICE_FILTER
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_FILTER);
		if (!getvalue(tempkey, configitem._filter))
		{
			configitem._filter = FILTER_NONEFILTER;
		}
		//DEVICE_FILTER_LOGLEVEL
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_FILTER_LOGLEVEL);
		std::string _filter_loglevel_str;
		if (!getvalue(tempkey, _filter_loglevel_str))
		{
			configitem._filter_loglevel = log4cplus::ALL_LOG_LEVEL;
		}
		else
		{
			if (!transferloglevel(_filter_loglevel_str, configitem._filter_loglevel))
			{
				return false;
			}
		}
		//DEVICE_FILTER_LOGLEVELMIN
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_FILTER_LOGLEVELMIN);
		std::string _filter_loglevelmin_str;
		if (!getvalue(tempkey, _filter_loglevelmin_str))
		{
			configitem._filter_loglevelmin = log4cplus::ALL_LOG_LEVEL;
		}
		else
		{
			if (!transferloglevel(_filter_loglevelmin_str, configitem._filter_loglevelmin))
			{
				return false;
			}
		}
		//DEVICE_FILTER_LOGLEVELMAX
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_FILTER_LOGLEVELMAX);
		std::string _filter_loglevelmax_str;
		if (!getvalue(tempkey, _filter_loglevelmax_str))
		{
			configitem._filter_loglevelmax = log4cplus::FATAL_LOG_LEVEL;
		}
		else
		{
			if (!transferloglevel(_filter_loglevelmax_str, configitem._filter_loglevelmax))
			{
				return false;
			}
		}
		//DEVICE_FILTER_STRINGMATCH
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_FILTER_STRINGMATCH);
		if (!getvalue(tempkey, configitem._filter_stringmatch))
		{
			configitem._filter_stringmatch = DEFAULT_STRINGMATCH;
		}
		//DEVICE_SELFLEVEL
		std::string subloglevel;
		tempkey = genstr(tempvalue, COMLOG_SUBDEVIEC_SELFLEVEL);
		if (!getvalue(tempkey, subloglevel))
		{
			return false;
		}
		getloglevelset(subloglevel, COMLOG_COMMA, configitem._selflevelset);

		std::pair<std::map<std::string, struct ConfigItem>::iterator, bool> pr = comlog_device_map.insert(std::pair<std::string, struct ConfigItem>(configitem._name, configitem));
		if (!pr.second)
		{
			return false;
		}

		//cout << configitem._name << " " << configitem._layout << " " << configitem._path << " " << configitem._filename << " " << configitem._addpender << " " << configitem._addpender_DRFscheduler << " " << configitem._addpender_DRFbackupindex << " " << configitem._addpender_RFbackupindex << " " << configitem._addpender_RFfilesize << " " << configitem._addpender_Filemode << " " << configitem._filter << " " << configitem._filter_loglevel << " " << configitem._filter_loglevelmin << " " << configitem._filter_loglevelmax << " " << configitem._filter_stringmatch << endl ;

	}

	return true;
}

bool ComlogConfig::genkeyvalue(const std::string &confpath)
{
	assert(!confpath.empty());

	FILE *fp = fopen(confpath.c_str(), "r");
	if (NULL == fp)
	{
		return false;
	}

	char *charptr = NULL;
	ssize_t read;
	size_t len;

	while ((read = getline(&charptr, &len, fp)) != -1)
	{
		std::string buffer = charptr;
		delleadingspace(buffer);

		size_t bufferlen = buffer.size();
		if (bufferlen == 0 || buffer[0] == COMLOG_COMMENT_CHAR)
		{
			continue;
		}

		if (buffer[bufferlen - 1] == COMLOG_R_CHAR)
		{
			//Remove trailing 'Windows' \r.
			buffer.resize (bufferlen - 1);
		}

		size_t pos = buffer.find(COMLOG_SEPARATOR);
		if (-1 != pos)
		{
			std::string key = buffer.substr(0, pos);
			std::string value = buffer.substr(pos + 1);
			deltailingspace(key);
			delspace(value);

			std::pair<std::map<std::string, std::string>::iterator, bool> pr = comlog_key_value_map.insert(std::pair<std::string, std::string>(key,value));
			if (!pr.second)
			{
				return false;
			}
		}

		if (charptr)
		{
			free(charptr);
			charptr = NULL;
		}
	}

	fclose(fp);

	return true;
}

void ComlogConfig::delleadingspace(std::string &str)
{
	std::string::iterator iter = str.begin(); 
	for(; iter != str.end (); ++iter)
	{   
		if (!isspace (*iter))
			break;
	}   
	str.erase (str.begin (), iter);
}

void ComlogConfig::deltailingspace(std::string &str)
{
	std::string::reverse_iterator riter = str.rbegin();
	for(; riter != str.rend (); ++riter)
	{
		if (!isspace(*riter))
			break;
	}
	str.erase(riter.base(), str.end());
}

void ComlogConfig::delspace(std::string &str)
{
	delleadingspace(str);
	deltailingspace(str);
}

bool ComlogConfig::getvalue(const std::string &key, std::string &value)
{
	assert(!key.empty());

	std::map<std::string, std::string>::iterator iter = comlog_key_value_map.find(key);
	if (iter == comlog_key_value_map.end())
	{
		return false;
	}

	value = iter->second;
	return true;
}

bool ComlogConfig::getvalue(const std::string &key, log4cplus::DailyRollingFileSchedule &value)
{
	assert(!key.empty());

	std::map<std::string, std::string>::iterator iter = comlog_key_value_map.find(key);
	if (iter == comlog_key_value_map.end())
	{
		return false;
	}

	if (iter->second == "MONTHLY")
	{
		value = log4cplus::MONTHLY;
	}
	else if (iter->second == "WEEKLY")
	{
		value = log4cplus::WEEKLY;
	}
	else if (iter->second == "DAILY")
	{
		value = log4cplus::DAILY;
	}
	else if (iter->second == "TWICE_DAILY")
	{
		value = log4cplus::TWICE_DAILY;
	}
	else if (iter->second == "HOURLY")
	{
		value = log4cplus::HOURLY;
	}
	else if (iter->second == "MINUTELY")
	{
		value = log4cplus::MINUTELY;
	}
	else
	{
		return false;
	}

	return true;
}

bool ComlogConfig::getvalue(const std::string &key, int &value)
{
	assert(!key.empty());

	std::map<std::string, std::string>::iterator iter = comlog_key_value_map.find(key);
	if (iter == comlog_key_value_map.end())
	{
		return false;
	}

	value = atoi(iter->second.c_str());
	return true;
}

bool ComlogConfig::getvalue(const std::string &key, long &value)
{
	assert(!key.empty());

	std::map<std::string, std::string>::iterator iter = comlog_key_value_map.find(key);
	if (iter == comlog_key_value_map.end())
	{
		return false;
	}

	value = (long)atoi(iter->second.c_str());
	return true;
}

bool ComlogConfig::getvalue(const std::string &key, std::ios_base::openmode &value)
{
	assert(!key.empty());

	std::map<std::string, std::string>::iterator iter = comlog_key_value_map.find(key);
	if (iter == comlog_key_value_map.end())
	{
		return false;
	}

	if (iter->second == "trunc")
	{
		value = std::ios::trunc;
	}
	else if (iter->second == "app")
	{
		value = std::ios::app;
	}
	else if (iter->second == "ate")
	{
		value = std::ios::ate;
	}
	else
	{
		return false;
	}
	
	return true;
}

void ComlogConfig::getsetvalue(std::string &stringline, char flag, std::set<std::string> &strset)
{
	assert(!stringline.empty());
	delspace(stringline);

	do
	{
		size_t pos = stringline.find(flag);
		if (-1 == pos)
		{
			delspace(stringline);
			strset.insert(stringline);
			break;
		}
		else
		{
			std::string substr = stringline.substr(0, pos);
			delspace(substr);
			strset.insert(substr);
			stringline = stringline.substr(pos + 1);
		}
	}while(1);
}

bool ComlogConfig::transferloglevel(const std::string &level, log4cplus::LogLevel &loglevel)
{
	assert(!level.empty());

	if (level == "TRACE")
	{
		loglevel = log4cplus::TRACE_LOG_LEVEL;
	}
	else if (level == "DEBUG")
	{
		loglevel = log4cplus::DEBUG_LOG_LEVEL;
	}
	else if (level == "INFO")
	{
		loglevel = log4cplus::INFO_LOG_LEVEL;
	}
	else if (level == "WARN")
	{
		loglevel = log4cplus::WARN_LOG_LEVEL;
	}
	else if (level == "ERROR")
	{
		loglevel = log4cplus::ERROR_LOG_LEVEL;
	}
	else if (level == "FATAL")
	{
		loglevel = log4cplus::FATAL_LOG_LEVEL;
	}
	else
	{
		return false;
	}
	
	return true;
}

void ComlogConfig::getloglevelset(std::string &stringline, char flag, std::set<log4cplus::LogLevel> &loglevelset)
{
	assert(!stringline.empty());

	std::set<std::string> strset;
	getsetvalue(stringline, flag, strset);

	for(std::set<std::string>::iterator iter = strset.begin(); iter != strset.end(); ++iter)
	{
		log4cplus::LogLevel loglevel;
		if (!transferloglevel((*iter), loglevel))
		{
			return;
		}
		loglevelset.insert((log4cplus::LogLevel)loglevel);
	}
}

std::string ComlogConfig::genstr(const std::string &str, uint32_t &inter)
{
	char buffer[1024];
	sprintf(buffer, "%d", inter);
	return str+buffer;
}

std::string ComlogConfig::genstr(const std::string &str1, const std::string &str2)
{
	return str1+str2;
}

































/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
