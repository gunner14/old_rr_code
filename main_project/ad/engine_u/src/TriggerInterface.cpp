/*
 * =====================================================================================
 *
 *       Filename:  TriggerInterface.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月23日 16时05分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <sys/time.h>
#include "AdMatchManagerI.h"
#include "TriggerInterface.h"
using namespace xce::ad;

struct PingStat{

  timeval start_, end_;

  PingStat(){ 
    gettimeofday(&start_, NULL);
  }

  ~PingStat() {
    gettimeofday(&end_, NULL);
    int timeuse = 1000 * (end_.tv_sec - start_.tv_sec) + (end_.tv_usec - start_.tv_usec)/1000;
    AdMatchManagerI::instance().ping_sender_.Send("trigger", timeuse);
  }
};

int TriggerInterface::trigger(const UserProfile &profile, std::vector<long> &zones, GroupDict& groups)
{
	TriggerInputInfo input;
	input.age = profile.age();
	input.gender = profile.gender();
	input.stage = profile.stage();
	input.grade = profile.grade();
	input.uid = profile.id();
	stringstream ss;
	ss << profile.school();
	input.school = ss.str();

	input.ipArea = profile.ip_area();
	input.currentArea = profile.current_area();
	input.company = profile.work_place();
	input.zones = zones;

  PingStat stat;
	try
	{
		int ret = IndexServiceReplicaAdapter::instance().trigger(input, groups);
		return ret;
	}
	catch (Ice::Exception& e) 
	{
		MCE_WARN("IndexServiceReplicaAdapter trigger caught ICEexception: " << e << " ,uid: " << input.uid);
		return -1;
	}
	catch (std::exception& e) 
	{
		MCE_WARN("IndexServiceReplicaAdapter trigger caught STDexception: " << e.what() << " ,uid: " << input.uid);
		return -1;
	}

}
