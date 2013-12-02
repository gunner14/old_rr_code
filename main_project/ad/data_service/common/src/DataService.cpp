#include "DataService.h"

using namespace xce::ad;
void DataService::Initialize(std::string ip,int port,int timeout){
		IndexClient::instance().Initialize(ip,port,timeout);
		index_loader_ = new IndexServiceLoader();
		load_manager_ = new IndexLoadManager(index_loader_);
}

void DataService::run(){
	MCE_INFO("DataService::run Start");
	IndexClient::instance().start();
	load_manager_->LoadBaseIndex();
	load_manager_->UpdateIndex();
	MyUtil::TaskManager::instance().scheduleRepeated(new IndexUpdateTimer(load_manager_));
	load_manager_->LoadIncIndex();
}
