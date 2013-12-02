#ifndef HOBBY_MEMCACHED_CLIENT_
#define HOBBY_MEMCACHED_CLIENT_

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <iostream>
#include <string>
#include <map>
#include "MemcachedClientPool.h"

namespace xce{
namespace ad{
class  HobbyMemcClient{
	public:
		HobbyMemcClient(){
			pool_.Init(50);
		}   
		
		bool  GetData(int  uid, std::set<uint64_t>& hobbies){
			pair<int, MemcachedClient*> cli = pool_.GetClient(uid); 
			if(!cli.second){ 
				MCE_WARN("Get Client Error");
				return false; 
			}  

			std::string  struid = boost::lexical_cast<std::string>(uid);

			vector<string> keys;
			keys.push_back(struid);
      map<string, string> res;
			map<string, int> flags; 
			bool  bret = cli.second->Get(keys, res, flags); 
			pool_.ReleaseClient(cli.first, cli.second); 
			if(bret == false){
        MCE_INFO("Get Data failed"); 
				return false;
			}   
			
			std::vector<std::string> para_strs;
			boost::algorithm::split_regex(para_strs, res[struid], boost::regex(","));
			for(std::vector<std::string>::iterator it = para_strs.begin(); it != para_strs.end(); ++it){
				char*  chTemp = NULL;
				uint64_t  hobby = strtoul(it->c_str(), &chTemp, 16);
				hobbies.insert(hobby);
				//本地cache最多保留10个关键字
				if(hobbies.size() >= 10){
					break;
				}
			}
			return true;
		}   
		
		bool PutData(int uid, const std::set<uint64_t>& hobbies){
			pair<int, MemcachedClient*> cli = pool_.GetClient(uid);
			if(!cli.second){
				MCE_WARN("Get Client Error");
				return false;
			}
		
			std::string  key = boost::lexical_cast<std::string>(uid);
			ostringstream  oss;
			for(std::set<uint64_t>::const_iterator it = hobbies.begin(); it != hobbies.end(); ++it){
				oss << boost::lexical_cast<std::string>(*it) << ",";
			}

			bool res = cli.second->Set(key, oss.str(), 0);  
			pool_.ReleaseClient(cli.first, cli.second);  
			return res;
		}   
	private:
		PoolManager pool_; 
};
};
};
#endif
