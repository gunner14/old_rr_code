#include <iostream>
#include <sys/time.h>
#include <sstream>
//#include "LogWrapper.h"

class TimeLog {
	public:
		TimeLog(const std::string log = ""): log_(log) {
			gettimeofday(&begin_, NULL);
		}

		~TimeLog() {
//			MCE_INFO(log_ << " timeuse:" << getTime());
#ifdef STDOUT
			std::ostringstream oss;
			oss << log_ << " timeuse:" << getTime() << std::endl;
			//std::cout << log_ << " timeuse:" << getTime() << std::endl;
			std::cout << oss.str();
			//std::cout<<  getTime() << std::endl;
#endif
		}

		float getTime() {
			gettimeofday(&end_, NULL);
			float timeuse = 1000000 * (end_.tv_sec - begin_.tv_sec) + end_.tv_usec
				- begin_.tv_usec;
			timeuse /= 1000;
			return timeuse;
		}

	private:
		timeval begin_, end_;
		std::string  log_;
};

