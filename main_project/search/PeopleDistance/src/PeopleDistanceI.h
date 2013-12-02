/*
 * FeedQueueI.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#include "SearchPeopleDistance.h"
#include "LogWrapper.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "QueryRunner.h"
#include <boost/foreach.hpp>
#include "map"

using namespace std;
using namespace boost;
using namespace MyUtil;
using namespace com::xiaonei::search::distance;
namespace com {
namespace xiaonei{
namespace search {
namespace distance{

const static int kDim = 10;

class UserInfo{
public:
	int uid_;
	float loc_[10];
	string ToString(){
		return "";	
	}
};
class PeopleDistanceI: public PeopleDistance, public MyUtil::Singleton<PeopleDistanceI> {
public:
	virtual Int2FloatMap Get(int uid,const IntSeq & targets,const Ice::Current& = Ice::Current());
	void Load();
private:
	typedef map<int,UserInfo> InfoMap;
	InfoMap uinfos_;
};

}
}
}
}

