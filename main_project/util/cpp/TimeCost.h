#ifndef __TIMECOST__H_
#define __TIMECOST__H_
#include "IceLogger.h"
namespace MyUtil {
class TimeCost {
	public:
		const static int LOG_LEVEL_DEBUG = 0;
		const static int LOG_LEVEL_INFO = 1;
		const static int LOG_LEVEL_WARN = 2;
		const static int LOG_LEVEL_ERROR = 3;

		static TimeCost create(const std::string& name, int level = LOG_LEVEL_DEBUG){
			TimeCost new_instance(name, level);
			new_instance.start();
			return new_instance;
		}

		void step(string step_name, bool isOneLine = false){
			long now = get_now();
			if( isOneLine )
			{
				ostringstream os;
				os << " step : " << step_name <<" "<< now - last_point_ <<" ms";
				name_+= os.str();
			}else
			{
				ostringstream os;
				os << "TimeCost : " << name_ << " " << step_name <<" "<< now - last_point_ <<" ms";
				log(os.str());
			}
			
			last_point_ = now;
		}
		
		~TimeCost(){
			ostringstream os;
			os << "TimeCost : " << name_ << " TOTAL "<< get_now() - start_ <<" ms";
			log(os.str());
		}
	private:
		std::string name_;
		long start_;
		long last_point_;
		int level_;

		TimeCost(const string& name, int level) : name_(name), level_(level) {
		}

		void log(const string& data) {
			switch (level_) {
				case LOG_LEVEL_DEBUG:
					MCE_DEBUG(data);
					break;
		                case LOG_LEVEL_INFO:
					MCE_INFO(data);
					break;
        	        	case LOG_LEVEL_WARN:
					MCE_WARN(data);
					break;
       		         	case LOG_LEVEL_ERROR:
					MCE_ERROR(data);
					break;
				default:
					MCE_DEBUG(data);
			}
		}

		void start(){
			start_ = get_now();
			last_point_ = start_;
		}

		long get_now(){
			struct timeval tv; 
			memset(&tv, 0, sizeof(tv));
			gettimeofday(&tv, NULL);
			return tv.tv_sec*1000+tv.tv_usec/1000;
		};

};

/**
 * @description 一个简单的计时类
 * @method step 计算每一步的时间
 * @method total 计算从start开始的时间
 */
class Clock {
public:
  Clock() {
    restart();
  }

  long step() {
    long old = last_point_;
    last_point_ = get_now();
    return last_point_ - old;
  }

  long total() {
    return get_now() - start_point_;
  }

  void restart() {
    start_point_ = get_now();
    last_point_ = start_point_;
  }

  long get_now() {
    struct timeval tv; 
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
  }

private:
  long start_point_;
  long last_point_;
};

}
#endif
