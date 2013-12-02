/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file RcdMock.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/14 09:27:22
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <signal.h>
#include "RcdMock.h"
#include "crawler.h"
#include "common.h"

using namespace comlog;
using namespace std;

RcdMock* RcdMock::r_instance = NULL;

bool RcdMock::r_stop_rcdmock = false;

//版本号
#ifdef __VERSION_ID__
const char* RcdMock::r_version_id = __VERSION_ID__;
#else
const char* RcdMock::r_version_id = "unknown";
#endif
//构建时间
#if defined(__DATE__) && defined(__TIME__)
const char* RcdMock::r_build_date = __DATE__ " " __TIME__;
#else
const char* RcdMock::r_build_date = "unknown";
#endif

RcdMock::RcdMock()
{}

RcdMock::~RcdMock()
{}

bool RcdMock::parse_cmdline(int argc, char **argv)
{
    int iOptRet = -1;
   
    int right_arg = 0, need_arg = 2; 
    while( (iOptRet = getopt(argc, argv, "hvc:l:")) != -1)
    {
        switch(iOptRet)
        {
			case 'h':
			case 'v':
				print_version(cout);
				exit(0);
			case 'c':
				if (NULL == optarg) 
					return false;
				r_conf_path = optarg; 
				right_arg ++;
				break;
			case 'l':
				if (NULL == optarg) 
					return false;
				r_comlog_conf = optarg;
				right_arg ++;
				break;
			default:
				print_help(argv[0]);
				exit(0);
				break;
        }
    }
    if(right_arg != need_arg)
    {
		print_help(argv[0]);
        return false;
    }
    return true;
}

void RcdMock::print_help(const char* name)
{
	cout << name << " -c conf_file -l comlog_conf [-vh]" << endl;
}

void RcdMock::print_version(ostream &oss)
{
	oss << "Version : " << r_version_id << endl;
	oss << "Build Date : " << r_build_date << endl;
}

RcdMock* RcdMock::instance()
{
	if( !r_instance )
	{
		r_instance = new RcdMock();
	}
	return r_instance;
}

void RcdMock::on_sig_int(int signal)
{
	r_stop_rcdmock = true;
}

bool RcdMock::initialize(int argc, char **argv)
{
	if( !parse_cmdline(argc, argv) )
	{
		cerr << "parse_cmdline error." << endl;
		return false;
	}

	return true;
}

void RcdMock::run()
{

	COMLOG_INIT(r_comlog_conf);

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, &RcdMock::on_sig_int);
	signal(SIGTERM, &RcdMock::on_sig_int);

	crawler crawl_sys;
	if( !crawl_sys.loadconf(r_conf_path) )
	{
		ERROR("crawler load conf failed.");
		return;
	}
	if( !crawl_sys.init() )
	{
		ERROR("crawler init failed.");
		return;
	}
	if( !crawl_sys.start() )
	{
		ERROR("crawler start failed.");
		return;
	}

	while(!r_stop_rcdmock)
	{
		sleep(3);
		INFO("RcdMock is running.");
	}

	delete r_instance;
	INFO("RcdMock is stop");
}















/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
