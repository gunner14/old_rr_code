/*
 * =====================================================================================
 *
 *       Filename:  indexManager.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月16日 20时16分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __INDEX_SERVICE_INDEX_MANAGER_H__
#define __INDEX_SERVICE_INDEX_MANAGER_H__

#include "index.h" 
#include "ServiceI.h"
#include "Singleton.h"
#include "IndexService.h"
#include "indexLog.h"

namespace xce{
	namespace ad{

    class InitTask : virtual public MyUtil::Task{
      public:
        InitTask(){}
        virtual void handle();
    };

		class IndexManager :public IndexService, public MyUtil::Singleton<IndexManager>
		{
			public:
				IndexManager():init_ok_(false){}
				int init();

				int loadIndex();
				virtual Ice::Int trigger(const TriggerInputInfo&, GroupDict&, const Ice::Current&);

        Ice::Int trigger(const TriggerParameter& input, GroupDict& groups, const Ice::Current& cur);

        void Close(Ice::Int uid, Ice::Long creative_id, const Ice::Current&);
				void calcCtr(long zone_id, const TriggerInputInfo &input, GroupInfo &gInfo);
        void select(long zone_id, const TriggerInputInfo &input, std::vector<GroupInfo> &gVec, GroupDict& groups);
        void initOK(){init_ok_ = true;}

				void calcCtr(long zone_id, const TriggerParameter &input, GroupInfo &gInfo);
        void select(long zone_id, const TriggerParameter &input, std::vector<GroupInfo> &gVec, GroupDict& groups);

			private:
        void checkSmallWidget(const GroupInfo &info, int &small_widget_num);
      /*
        class InitLoader: public IceUtil::Thread{
          public:
            virtual void run();
        };
        friend class InitLoader;
        */

				void addIndex(IndexPtr index);
				IndexPtr getIndexByName(const std::string &name);
				std::vector<IndexPtr> indexes_;
        //InitLoader init_loader_;
        TriggerLog trigger_log_;
				int server_index_;
        bool init_ok_;
		};

	}
}

#endif

