#include <map>
#include "LoaderAdapter.h"

using namespace std;
using namespace search::relation;
using namespace com::renren::searchrelation;
void RelationLoaderAdapter::Init(const string & zk,const string & loaderpath){
	LoaderPool::GetInstance()->Init(zk,loaderpath);	
}
map<int,map<int,short> > RelationLoaderAdapter::GetRelationV2(int uid,int degrees,const string & callBiz){
	SearchRelationLoaderPrx prx = LoaderPool::GetInstance()->getProxy(uid)		;
	if(prx != 0){
		MCE_INFO("RelationLoaderAdapter::GetRelationV2.uid:" << uid << " degrees:" << degrees << " get proxy :" << prx );
		try{
			return prx->getRelationV2(uid,degrees,callBiz);	
		}catch (Ice::Exception& e) {
			MCE_WARN("RelationLoaderAdapter::GetRelationV2.uid:" << uid << " degrees:" << degrees << " get relation fail. e:" << e );
			throw e;
		} catch (std::exception& e) {
			MCE_WARN("RelationLoaderAdapter::GetRelationV2.uid:" << uid << " degrees:" << degrees << " get relation fail. e:" << e.what() );
			throw e;
		}
		return 	map<int,map<int,short> > ();
	}else {
		MCE_WARN("RelationLoaderAdapter::GetRelationV2.uid:" << uid << " degrees:" << degrees << " get proxy fail!!!" );
		return 	map<int,map<int,short> > ();
	}
}
