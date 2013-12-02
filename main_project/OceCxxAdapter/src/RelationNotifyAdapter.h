/*
 * =====================================================================================
 *
 *       Filename:  RelationNotifyAdapter.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年03月16日 19时29分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jun.liu (), jun.liu@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */

#ifndef __RELATION_NOTIFY_ADAPTER_H__
#define __RELATION_NOTIFY_ADAPTER_H__

#include  <string.h>
#include  <boost/thread.hpp>
#include  <vector>
#include  <zookeeper-client/zookeeper.h>
#include  <Ice/Ice.h>
#include "Singleton.h"
#include  "FriendRelationNotifier.h"


using namespace std;
using namespace boost;
using namespace wap::notify;
using namespace Ice;

namespace wap {
namespace notify{

	class RelationNotifyAdapter : public MyUtil::Singleton<RelationNotifyAdapter>
	{
		private:
			zhandle_t *zkHandler;
			shared_mutex mutex_;
			vector<FriendRelationNotifierPrx> proxyList;
			CommunicatorPtr communicator_;
		public:

			RelationNotifyAdapter();
			~RelationNotifyAdapter();


			/**
			 * @brief 通知wap好友关系变化 
			 * @param FriendRelationType type 变化类型
			 * @param int from 事件的来源者
			 * @param int target 事件的目标人
			 * @return void
			 */
			void notify(FriendRelationType type, int from, int target);

			/**
			 * @brief watcher函数 
			 **/
			static void watcher(zhandle_t * zh, int type, int state, const char* path, void * watcherCtx); 
		
		private:
			/**
			 * @brief 初始化
			 */
			void initProxy();

			/**
			 * @brief 初始化连接器
			 */
			bool initCommunicator();

			/**
			 * @brief 连接zookeeper服务器
			 */
			bool connect();
		
			/**
			 * @brief 获取Proxy, 基本hash
			 */
			FriendRelationNotifierPrx getProxy();
	
	};

	
	void MicroSleep(long microseconds); 


	void MilliSleep(long milliseconds);
};
};
#endif
