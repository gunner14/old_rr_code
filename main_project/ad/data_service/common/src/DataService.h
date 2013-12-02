/*
 * =====================================================================================
 *
 *       Filename:  t.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年08月23日 10时32分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _DATA_SERVICE_H___
#define _DATA_SERVICE_H___

#include <vector>
#include <iostream>
#include <IceUtil/Monitor.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Thread.h>

#include "IndexLoader.h"
#include "IndexClient.h"
#include "TaskManager.h"
#include "index_load.h"

using namespace xce::ad;
class DataService: public IceUtil::Thread,public MyUtil::Singleton<DataService>{
  public:
		void Initialize(std::string ip,int port,int timeout);
		virtual void run();

	private:
		IndexLoader *index_loader_;
		IndexLoadManager* load_manager_;
};


#endif
