/*
 * FeedQueueI.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#include "PeopleDistanceI.h"
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

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&PeopleDistanceI::instance(), service.createIdentity("M", ""));
	PeopleDistanceI::instance().Load();
}
double arccosh(double x) { 
	double t, r;
	t = x + sqrt(x * x - 1);
	r = log(t);
	//cout<<"cosh"<<t<<endl;
	return r;
}
//double pb_dist(double *a, double *b) {
//	int i;
//	double dist, tx = 1.0, ty = 1.0, tt = 0.0; 
//	for (i = 0; i < kDim; i++) {
//		tx = tx + a[i] * a[i];
//		ty = ty + b[i] * b[i];
//		tt = tt + a[i] * b[i];
//	}
//	double t = sqrt(tx * ty) - tt;
//	t = arccosh(t) * fabs(curve);
//	//cout<<"pb_dist"<<t<<endl;
//	return t;
//}

float CalDistance(const UserInfo & a, const UserInfo & b){
	const static float curve = -1.0;
	int i;
	double dist, tx = 1.0, ty = 1.0, tt = 0.0; 
	for (i = 0; i < kDim; i++) {
		tx = tx + a.loc_[i] * a.loc_[i];
		ty = ty + b.loc_[i] * b.loc_[i];
		tt = tt + a.loc_[i] * b.loc_[i];
	}
	double t = sqrt(tx * ty) - tt;
	t = arccosh(t) * fabs(curve);
	//cout<<"pb_dist"<<t<<endl;
	return t;

//return 1.0f;
}

Int2FloatMap PeopleDistanceI::Get(int uid,const IntSeq & targets,const Ice::Current& ){
	Int2FloatMap res;
	InfoMap::iterator uit = uinfos_.find(uid);	
	if(uit == uinfos_.end()){
		MCE_INFO("PeopleDistanceI::Get. uid:" << uid << " not in cache");	
		return res;
	}
	UserInfo main_info = uit->second;
	set<int> misses;
	BOOST_FOREACH(int t,targets){
		InfoMap::iterator it = uinfos_.find(t);	
		if(it == uinfos_.end()){
			misses.insert(t);	
			continue;
		}
		res[t] = CalDistance(main_info,it->second);
	}

	return res;
}
void PeopleDistanceI::Load(){
	int idin = 0; 
        //char * fname = "./a.txt";
        char * fname = "/data/xce/XiaoNeiSearch/data/PeopleDistance/a.txt";
        FILE * fp;
        if ((fp = fopen(fname, "r")) == NULL) {
                MCE_INFO("Cannot open file  " << fname);
                return ;
        }
	UserInfo uinfo;
        float *loc = uinfo.loc_;
        while(  fscanf(fp,"%d %f %f %f %f %f %f %f %f %f %f\n",&(uinfo.uid_),
                                &loc[0], &loc[1], &loc[2], &loc[3], &loc[4],
                                &loc[5], &loc[6], &loc[7], &loc[8], &loc[9]) != EOF){
		MCE_INFO("PeopleDistanceI::Load." << uinfo.ToString());
		uinfos_[uinfo.uid_]= uinfo;
                ++idin;
        }
        fclose(fp);
}

