/*
 * =====================================================================================
 *
 *       Filename:  log.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月10日 10时01分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __INDEX_SERVICE_LOG_H__
#define __INDEX_SERVICE_LOG_H__

#include "ad/thriftappender/src/thriftappender.h"
#include <string>
#include <vector>
#include "IndexService.h"
#include "index.h"

namespace xce{
	namespace ad{

		class TriggerLog{

			public :
				static void init(const std::string& name,
						const std::string& path,
						const std::string& loglevel = "INFO",
						const std::string& pattern = "%D{[%Y-%m-%d %H:%M:%S]}  %m %n"){

					log4cplus::SharedAppenderPtr append_1(new log4cplus::DailyRollingFileAppender(path, log4cplus::HOURLY, 1, true));
					append_1->setName(name);
					append_1->setLayout( std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)) );
					log4cplus::Logger::getInstance(name).addAppender(append_1);
					log4cplus::Logger::getInstance(name).setLogLevel(log4cplus::LogLevelManager().fromString(loglevel));

				}

				void setLogString(const std::string &s){
					msg_ = s;
				}

				void setInput(const TriggerParameter& input){
					const std::vector<long> &zones = input.zones_;
					msg_ += "|";
					for(std::vector<long>::const_iterator it = zones.begin(); it != zones.end(); ++it){
						ostringstream s;
						s << *it << ",";
						msg_ += s.str();
					}


					ostringstream profile;
					profile << "|" << input.uid_ << "," << input.stage_ << "," << input.gender_ << "," \
						<< input.age_ << "," << input.grade_ << "," \
						<< input.ipArea_ << "," << input.currentArea_ << "," << input.school_ << "|" ;

					copy(input.companys_.begin(), input.companys_.end(), ostream_iterator<uint64_t>(profile, ","));
					profile << "|";

					msg_ += profile.str();

				}

				void setUseTime(int t){
					time_use_.push_back(t);
				}

				void setAdNum(int an){
					adv_num_.push_back(an);
				}

        void setAdNumAfterSelect(int an){
					after_select_adv_num_.push_back(an);
				}

				void output()
				{
          
					for(std::vector<int>::iterator it = adv_num_.begin(); it != adv_num_.end(); ++it)
					{
						ostringstream s;
						s << *it << ",";
						msg_ += s.str();
					}

					msg_ += "|";

					for(std::vector<int>::iterator it = after_select_adv_num_.begin(); it != after_select_adv_num_.end(); ++it)
					{
						ostringstream s;
						s << *it << ",";
						msg_ += s.str();
					}

          msg_ += "|";

					for(std::vector<int>::iterator it = time_use_.begin(); it != time_use_.end(); ++it)
					{
						ostringstream s;
						s << *it << ",";
						msg_ += s.str();
					}

          msg_ += "|";

					LOG_INFO("trigger", msg_);
				}


			private:
				std::string msg_;
				std::vector<int> time_use_;
				std::vector<int> adv_num_;
				std::vector<int> after_select_adv_num_;
		};

	}
}

#endif
