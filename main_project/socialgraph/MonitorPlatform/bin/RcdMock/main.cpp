/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file main.cpp
 * @author lei.yang3@renren-inc.com(lei.yang3@renren-inc.com)
 * @date 2012/08/13 22:01:11
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include "RcdMock.h"

int main(int argc, char *argv[])
{

	if(g_rcdmock->initialize(argc, argv))
	{
		g_rcdmock->run();
	}
	return 0;
}




















/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
