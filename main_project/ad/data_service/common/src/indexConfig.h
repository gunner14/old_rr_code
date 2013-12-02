/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月04日 11时11分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __INDEX_SERVICE_CONFIG_H__
#define __INDEX_SERVICE_CONFIG_H__

#include <boost/lexical_cast.hpp>
#include <string>
#include <map>
#include <stdint.h>
#include "Singleton.h"
#include "indexTimer.h"
#include <IceUtil/Mutex.h>
#include <IceUtil/RWRecMutex.h>

namespace xce{
	namespace ad{

#define LOOKUP_NO_EXCEPTIONS(P, T, V)  												\
		try                                     									\
		{                                       									\
			V = boost::lexical_cast<T>(P);									\
			return(true);                      									    \
		}                                       									\
		catch(...)                        											\
		{                                       									\
			return(false);                      									\
		}																			


		class IndexConfig : public MyUtil::Singleton<IndexConfig> {

			public:

        IndexConfig():ad_close_expired_minute_(1440){}
				int loadConfig();
				int reloadConfig();

				void setServerIndex(int);
				int getServerIndex();

				void outputConfig();

				void getAdNumByAdzone(long adzone, unsigned &total_num, unsigned &new_num);
			        int getGameTotalAdNum(long zone);
			        int getGameMemberAdNum(long zone);
			        int getAdCloseExpired(){
			            return ad_close_expired_minute_;
        			}	
				std::string GetLowerDB(){ return lower_db_source_; }

				std::map<std::string, int> getTriggerInfo();	

				bool lookupValue(const std::string &key, int &val);
				bool lookupValue(const std::string &key, uint64_t &val);
				bool lookupValue(const std::string &key, bool &val);
				bool lookupValue(const std::string &key, double &val);
				bool lookupValue(const std::string &key, std::string &val);

			private:
				int load();
			        std::pair<std::string, std::string> split(const std::string &s, const std::string &spliter);
				int server_index_;
			        int ad_close_expired_minute_;
				std::map<long, std::pair<unsigned, unsigned> > adzone_ad_num_;
				std::map<std::string, int> trigger_info_;
        			std::map<long, int> game_ad_total_num_;
			        std::map<long, int> game_member_ad_num_;
				std::string lower_db_source_;
				IceUtil::RWRecMutex lock_;
		};

		class ConfigTimer: public IndexTimer
		{
			public:
				ConfigTimer(void *instance): IndexTimer(instance){}
				virtual void process();
				virtual int interval() const;
		};
	}
}

#endif 
