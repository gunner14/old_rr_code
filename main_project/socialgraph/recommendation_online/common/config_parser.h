#ifndef CONFIG_PARSER_H_
#define CONFIG_PARSER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#include "json/json.h"

#include "UtilCxx/src/Singleton.h"
#include "base/rwlock.h"

namespace xce {
namespace socialgraph {

typedef std::map<std::string, std::string> AttributeBucket;
typedef std::map<std::string, std::map<std::string, std::string> > AttributeBuckets;

enum Domain {					//two different domain, including Online and Experiment
	Online = 0,
	Experiment
};

const static int CHECK_INTERVAL = 60;		//time interval of checking file

//--------------------------------------------
/*
@brief configuration parser 
*/
class ConfigParser : public MyUtil::Singleton<ConfigParser> {
 public:
	/*
	@brief initialize config parser
	@param file configuration
	*/
	void Initialize(const std::string& file = "/data/xce/XSocialGraph/sg_config/person_recommend.conf");

	/*
	@brief Get value from configuration
	@param	domain (Online, Experiment)
	@param	key (key in configuration file)
	@param	second_key (key of value to get)
	*/
	std::string Get(const Domain domain, 
	 							  const std::string& key, 
	 							  const std::string& second_key) const;

 protected:
	ConfigParser();
	virtual ~ConfigParser();

	friend class MyUtil::Singleton<ConfigParser>;

 private:
	std::string Convert(const std::string& file) const;

	bool Fill(const Json::Value& root);

	std::string Get(const AttributeBuckets& buckets, 
									const std::string& key,
									const std::string& second_key) const;

	void Print(const AttributeBuckets& buckets) const;

	static void Check(const std::string& file, 
										int time);

	void FillBuckets(const Json::Value& root, 
									 const std::string& key_name, 
									 AttributeBuckets& target);

	mutable ReadWriteLock lock_;		//use rwlock to get better performance
	AttributeBuckets rules_bucket_;
	AttributeBuckets experiment_bucket_;
};

}}

#endif
