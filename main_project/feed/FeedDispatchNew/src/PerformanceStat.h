#ifndef FA_PERFORMANCE_H_
#define FA_PERFORMANCE_H_

#include "TaskManager.h"
#include "IceLogger.h"
#include "Singleton.h"
#include "FeedDispatchNewI.h"
//#include "FeedCacheI.h"

using namespace xce::feed;
class PerformanceStat : public MyUtil::Timer, public MyUtil::Singleton<PerformanceStat>{
public:
	static const int INTERVAL = 5*1000; 

	PerformanceStat() : Timer(INTERVAL, 8080){
		_rm_id_count = 0;
		_rm_time_count = 0;
		_rm_ss_count = 0;
		_rm_ssactor_count = 0;
		_rm_parent_count = 0;
		_dis_count = 0;
		_reply_count = 0;

		_rm_time_max_delay = 0;
		_rm_time_min_delay = 0;
		_rm_time_sum_delay = 0;

		_rm_id_max_delay = 0;
		_rm_id_min_delay = 0;
		_rm_id_sum_delay = 0;

		_rm_ss_max_delay = 0;
		_rm_ss_min_delay = 0;
		_rm_ss_sum_delay = 0;

		_rm_ssactor_max_delay = 0;
		_rm_ssactor_min_delay = 0;
		_rm_ssactor_sum_delay = 0;

		_rm_parent_max_delay = 0;
		_rm_parent_min_delay = 0;
		_rm_parent_sum_delay = 0;

		_dis_max_delay = 0;
		_dis_min_delay = 0;
		_dis_sum_delay = 0;

		_reply_max_delay = 0;
		_reply_min_delay = 0;
		_reply_sum_delay = 0;
	}

	void handle(){
    //MCE_DEBUG("PerformanceStat::handle");
		//IceUtil::Mutex::Lock lock(_mutex);
		MCE_INFO("### PerformanceStat ### _rm_id_count:" << _rm_id_count 
			<< " max:" << _rm_id_max_delay << " min:" << _rm_id_min_delay << " avg:" << _rm_id_sum_delay/(_rm_id_count+1)
      << " _rm_ss_count:" << _rm_ss_count 
      << " max:" << _rm_ss_max_delay << " min:" << _rm_ss_min_delay << " avg:" << _rm_ss_sum_delay/(_rm_ss_count+1) 
      << " _rm_ssactor_count:" << _rm_ssactor_count 
      << " max:" << _rm_ssactor_max_delay << " min:" << _rm_ssactor_min_delay << " avg:" << _rm_ssactor_sum_delay/(_rm_ssactor_count + 1)
      << " _rm_parent_count:" << _rm_parent_count
      << " max:" << _rm_parent_max_delay << " min:" << _rm_parent_min_delay << " avg:" << _rm_parent_sum_delay/(_rm_parent_count + 1)

      << " _dis_count:" << _dis_count
      << " max:" << _dis_max_delay << " min:" << _dis_min_delay << " avg:" << _dis_sum_delay/(_dis_count + 1)

      << " _reply_count:" << _reply_count
      << " max:" << _reply_max_delay << " min:" << _reply_min_delay << " avg:" << _reply_sum_delay/(_reply_count + 1)

      << " _rm_time_count:" << _rm_time_count 
      << " max:" << _rm_time_max_delay << " min:" << _rm_time_min_delay << " avg:" << _rm_time_sum_delay/(_rm_time_count+1) 


	  );	

		_rm_id_count = 0;
		_rm_time_count = 0;
		_rm_ss_count = 0;
		_rm_ssactor_count = 0;
		_rm_parent_count = 0;
		_dis_count = 0;

		_rm_time_max_delay = 0;
		_rm_time_min_delay = 0;
		_rm_time_sum_delay = 0;

		_rm_id_max_delay = 0;
		_rm_id_min_delay = 0;
		_rm_id_sum_delay = 0;

		_rm_ss_max_delay = 0;
		_rm_ss_min_delay = 0;
		_rm_ss_sum_delay = 0;

		_rm_ssactor_max_delay = 0;
		_rm_ssactor_min_delay = 0;
		_rm_ssactor_sum_delay = 0;

		_rm_parent_max_delay = 0;
		_rm_parent_min_delay = 0;
		_rm_parent_sum_delay = 0;

		_dis_max_delay = 0;
		_dis_min_delay = 0;
		_dis_sum_delay = 0;

		_reply_max_delay = 0;
		_reply_min_delay = 0;
		_reply_sum_delay = 0;
    FeedDispatchNewI::instance().PrintThreadPoolInfoByLevel();

	}
  void incRmTime(float delay){
    IceUtil::Mutex::Lock lock(_mutex);
    _rm_time_count++;
		_rm_time_sum_delay += delay;

		if(delay > _rm_time_max_delay || _rm_time_max_delay == 0){
			_rm_time_max_delay = delay;
		}
		if(delay < _rm_time_min_delay || _rm_time_min_delay == 0){
			_rm_time_min_delay = delay;
		}
  }

  void incRmId(float delay){
    IceUtil::Mutex::Lock lock(_mutex);
    _rm_id_count++;
		_rm_id_sum_delay += delay;

		if(delay > _rm_id_max_delay || _rm_id_max_delay == 0){
			_rm_id_max_delay = delay;
		}
		if(delay < _rm_id_min_delay || _rm_id_min_delay == 0){
			_rm_id_min_delay = delay;
		}
  }

  void incRmSourceStype(float delay){
    IceUtil::Mutex::Lock lock(_mutex);
    _rm_ss_count++;
		_rm_ss_sum_delay += delay;

		if(delay > _rm_ss_max_delay || _rm_ss_max_delay == 0){
			_rm_ss_max_delay = delay;
		}
		if(delay < _rm_ss_min_delay || _rm_ss_min_delay == 0){
			_rm_ss_min_delay = delay;
		}
  }

  void incRmSourceStypeActor(float delay){
    IceUtil::Mutex::Lock lock(_mutex);
    _rm_ssactor_count++;
		_rm_ssactor_sum_delay += delay;

		if(delay > _rm_ssactor_max_delay || _rm_ssactor_max_delay == 0){
			_rm_ssactor_max_delay = delay;
		}
		if(delay < _rm_ssactor_min_delay || _rm_ssactor_min_delay == 0){
			_rm_ssactor_min_delay = delay;
		}
  }

  void incRmParent(float delay){
    IceUtil::Mutex::Lock lock(_mutex);
    _rm_parent_count++;
		_rm_parent_sum_delay += delay;

		if(delay > _rm_parent_max_delay || _rm_parent_max_delay == 0){
			_rm_parent_max_delay = delay;
		}
		if(delay < _rm_parent_min_delay || _rm_parent_min_delay == 0){
			_rm_parent_min_delay = delay;
		}
  }

  void incDispatch(float delay){
    IceUtil::Mutex::Lock lock(_mutex);
    _dis_count++;
		_dis_sum_delay += delay;

		if(delay > _dis_max_delay || _rm_parent_max_delay == 0){
			_dis_max_delay = delay;
		}
		if(delay < _dis_min_delay || _rm_parent_min_delay == 0){
			_dis_min_delay = delay;
		}
  }

  void incReply(float delay){
    IceUtil::Mutex::Lock lock(_mutex);
    _reply_count++;
		_reply_sum_delay += delay;

		if(delay > _reply_max_delay || _rm_parent_max_delay == 0){
			_reply_max_delay = delay;
		}
		if(delay < _reply_min_delay || _rm_parent_min_delay == 0){
			_reply_min_delay = delay;
		}
  }


private:
	IceUtil::Mutex _mutex;

  int _rm_id_count;
  int _rm_time_count;
	int _rm_ss_count;
	int _rm_ssactor_count;
  int _rm_parent_count;
	int _dis_count;
	int _reply_count;

  float _rm_id_max_delay;
  float _rm_id_min_delay;
  float _rm_id_sum_delay;

  float _rm_time_max_delay;
  float _rm_time_min_delay;
  float _rm_time_sum_delay;

  float _rm_ss_max_delay;
  float _rm_ss_min_delay;
  float _rm_ss_sum_delay;

  float _rm_ssactor_max_delay;
  float _rm_ssactor_min_delay;
  float _rm_ssactor_sum_delay;
	
  float _rm_parent_max_delay;
  float _rm_parent_min_delay;
  float _rm_parent_sum_delay;

  float _dis_max_delay;
  float _dis_min_delay;
  float _dis_sum_delay;

  float _reply_max_delay;
  float _reply_min_delay;
  float _reply_sum_delay;


};

#endif
