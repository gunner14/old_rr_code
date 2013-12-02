#ifndef RELATIONLOADERADAPTER_H
#define RELATIONLOADERADAPTER_H
#include <map>
#include "LoaderPool.h"
namespace search{
namespace relation{

using namespace std;
class RelationLoaderAdapter{
public:
	void Init(const string& zk,const string & loaderpath);
	map<int,map<int,short> > GetRelationV2(int uid,int degrees,const string & callBiz);
};
}
}
#endif
