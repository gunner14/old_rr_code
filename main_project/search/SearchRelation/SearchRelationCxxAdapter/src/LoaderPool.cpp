#include "LoaderPool.h"
using namespace std;
using namespace search::relation;

boost::mutex LoaderPool::single_mutex_;

void LoaderListerner::HandleChildEvent(const char * path, const std::vector<std::string> & childrens) {
	MCE_INFO("LoaderListerner::HandleChildEvent:" << path << PrintHelper::Print(childrens));
	LoaderPool::GetInstance()->UpdateNodes();
}



