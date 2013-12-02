/*
 * =====================================================================================
 *
 *       Filename:  IndexLoader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/09/12 11:56:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __INDEX_LOADER_H__
#define __INDEX_LOADER_H__

#include <vector>
#include <list>
#include <set>
#include <map>
#include <iostream>
#include <IceUtil/Monitor.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Thread.h>

#include "ad/indexService/common/src/IndexClient.h"
#include "ad/indexService/common/src/IndexItem.h"

#include "Singleton.h"
#include "TaskManager.h"
#include "db.h"

namespace xce{
namespace ad{
class IndexLoader{
	public:
		//加载基准数据
		virtual int LoadBase(const std::string &table,const std::vector<IndexItem> &base) = 0;
		//加载增量数据
		virtual int LoadInc(const std::vector<IndexItem> &inc) = 0;
		virtual void UpdateData() = 0;
		virtual void CreateIndex() = 0;
		virtual ~IndexLoader(){}
};

class IndexPreprocessor : IceUtil::Monitor<IceUtil::Mutex>,public MyUtil::Singleton<IndexPreprocessor>{
	public:
		IndexPreprocessor(){
			loaded_num_ = 0;
			table_num_ = 0;
		}
		void RegisterTable(const std::vector<std::string> &tables);
		void SetBaseReady(const std::string &table);
		void SetIncReady(const std::string &table);
		bool IsBaseReady(const std::string &table);
		bool IsIncReady(const std::string &table);
		bool CurrentTableReady();

		std::vector<IndexItem>& GetBaseItems(const std::string &tn);
		std::vector<IndexItem>& GetBasePlusItems(const std::string &tn);

		void GetIndex(std::string &table, std::vector<IndexItem> &base_items,	std::vector<IndexItem> &inc_items);
		std::vector<IndexItem> GetIncItems();
		void AddIncItem(const IndexItem &item);
		void AddBaseItem(const IndexItem &item);
		bool FinishLoad(){
			return loaded_num_ == table_num_;
		}
		int PreprocessBaseIndex(const std::string &table, const std::string &file);
	private:
		struct IndexProgressor{
			std::string table_name_;
			bool is_base_ready_;
			bool is_inc_ready_;
		};

		class BasePreProcessor : public IceUtil::Thread{
			public:
				BasePreProcessor(const std::string &file, const std::string &table):
					filename_(file), table_name_(table){}
				void process();
				virtual void run();
			private:
				std::string filename_;
				std::string table_name_;
		};

		int loaded_num_;
		int table_num_;

		std::map<std::string, std::vector<IndexItem> > base_items_;
		std::map<std::string, std::vector<IndexItem> > base_items_plus_;
		std::vector<IndexItem> inc_items_;
		std::list<IndexProgressor> index_progress_list_;
		std::list<std::string> ready_table_list_;

};

class IndexLoadManager{
	public:
		IndexLoadManager(IndexLoader *loader):index_loader_(loader){}

		int LoadBaseIndex(){
			MCE_INFO("IndexLoadManager::LoadBaseIndex start");
			while(!IndexPreprocessor::instance().FinishLoad()){
				std::vector<IndexItem> base;
				std::vector<IndexItem> inc;
				std::string table;
				IndexPreprocessor::instance().GetIndex(table, base, inc);
				MCE_INFO("IndexLoadManager::LoadBaseIndex, process table:"<<  table.c_str() << " ,base size:" << base.size() << " , inc size:" << inc.size());
				index_loader_->LoadBase(table, base);
				index_loader_->LoadInc(inc);
			}
			index_loader_->UpdateData();
			MCE_INFO("IndexLoadManager::LoadBaseIndex End");
			return 0;
		}

		int LoadIncIndex(){
			MCE_INFO("IndexLoadManager::LoadIceIndex start");
			while(true){
				std::vector<IndexItem> incs = IndexPreprocessor::instance().GetIncItems();
				index_loader_->LoadInc(incs);
				MCE_INFO("IndexLoadManager Process Increment ,size:" << incs.size());
				index_loader_->UpdateData(); //每次加载增量需要及时更新adgroup,campaign,member信息
			}      
			MCE_INFO("IndexLoadManager::LoadIceIndex End");
			return 0;
		}
		void UpdateIndex(){
			MCE_INFO("IndexLoadManager::UpdateIndex start");
			index_loader_->CreateIndex();
			MCE_INFO("IndexLoadManager::UpdateIndex End");
		}
	private:
		IndexLoader *index_loader_;
};

class IndexUpdateTimer: public MyUtil::Timer{
	public:
		IndexUpdateTimer(IndexLoadManager*  loadmanager):Timer(3 * 60 * 1000){ //3分钟刷新一次倒排索引
			loader = loadmanager;
		}
		virtual void handle() {
			MCE_DEBUG("Update Index begin");
			loader->UpdateIndex(); //定期加载倒排索引信息
			MCE_DEBUG("Update Index stop");
		}
	private:
		IndexLoadManager* loader;
};
}
}


#endif
