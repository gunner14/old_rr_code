/*
 * FeedQueueI.h
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#ifndef CARINDEXMANAGERI_H_
#define CARINDEXMANAGERI_H_

#include "SearchModel.h"
#include "SearchServer.h"
#include "Singleton.h"
#include <IceUtil/IceUtil.h>

#include "ObjectCacheI.h"
#include <map>
using namespace std;
using namespace MyUtil;

namespace com {
namespace xiaonei{
namespace search2 {
namespace server {

using namespace	com::xiaonei::search2::model;

class CarInfo: public Ice::Object {
public:
	CarInfo(int id):id(id){}
	string Dump(){
		ostringstream os;	
		os << "id:" << id << " name:" << content["name"];
		return os.str();
	}
	int id;
	map<string,string> content;

	//string name;
	//string url;
};
typedef IceUtil::Handle<CarInfo> CarInfoPtr;

class IndexManagerI: public IndexManager, public MyUtil::Singleton<IndexManagerI> {
public:
	virtual IndexResultPtr searchIndex(const IndexCondition & condition,int begin,int limit ,const Ice::Current& = Ice::Current());
	virtual IndexResultPtr searchIndexWithRestriction(const IndexCondition & condition,int begin,int limit ,const string & caller ,const Ice::Current& = Ice::Current());
	void Reload();
private:
	IceUtil::Mutex mutex_;
	map<string,CarInfoPtr> carmap_;
};

}
}
}
}
#endif /* FEEDQUEUEI_H_ */
