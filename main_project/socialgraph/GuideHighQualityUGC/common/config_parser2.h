#ifndef CONFIG_PARSER2_H_
#define CONFIG_PARSER2_H_

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

//domain
#define DOMAIN_SELECTOR "selector"
#define DOMAIN_CONFIG "config"

//config
#define ID_O "O"
#define ID_E1 "E1"
#define ID_E2 "E2"
#define ID_E3 "E3"

//priority
#define ID_PRIORITY "priority"

//rule
#define ID_RULE "rule"

//constraint
#define ID_CONSTRAINT "constraint"

//selector
#define ID_RANDOM "random"
#define ID_WHITELIST "whitelist"
#define ID_TAILNUM "tailnum"

#define R_ALL "r_all"
#define W_1 "w_1"
#define W_2 "w_2"
#define W_3 "w_3"
#define T_0 "t_0"
#define T_1 "t_1"
#define T_2 "t_2"
#define T_3 "t_3"
#define T_4 "t_4"
#define T_5 "t_5"
#define T_6 "t_6"
#define T_7 "t_7"
#define T_8 "t_8"
#define T_9 "t_9"

//--------------------------------------------

const static int CHECK_INTERVAL = 60;		//time interval of checking file
typedef std::vector<std::string> StrItemSeq;

//--------------------------------------------
/*
@brief configuration parser 
*/
class ConfigParser : public MyUtil::Singleton<ConfigParser> {
 public:
	/*
	@brief initialize config parser
	@param file		configuration
	*/
	void Initialize(const std::string& file = "/data/xce/XSocialGraph/sg_config/person_recommend.conf");

	/* Get Config property */
	int GetConfigAsInt(const std::string& id, 
										 const std::string& key) const;	

	std::string GetConfigAsString(const std::string& id, 
																const std::string& key) const;	

	double GetConfigAsDouble(const std::string& id, 
													 const std::string& key) const;	

	StrItemSeq GetConfigAsStrItemSeq(const std::string& id, 
																	 const std::string& key) const;	

	/* Get Priority property */
	int GetPriorityAsInt(const std::string& id,
											 const std::string& key) const;

	std::string GetPriorityAsString(const std::string& id,
																	const std::string& key) const;

	double GetPriorityAsDouble(const std::string& id,
														 const std::string& key) const;

	StrItemSeq GetPriorityAsStrItemSeq(const std::string& id,
																		 const std::string& key) const;

	/* Get Rule property */
	int GetRuleAsInt(const std::string& id, 
									 const std::string& key) const;

	std::string GetRuleAsString(const std::string& id, 
															const std::string& key) const;

	double GetRuleAsDouble(const std::string& id, 
												 const std::string& key) const;

	StrItemSeq GetRuleAsStrItemSeq(const std::string& id, 
																 const std::string& key) const;

	/* Get Constraint property */
	int GetConstraintAsInt(const std::string& id, 
												 const std::string& key) const;

	std::string GetConstraintAsString(const std::string& id, 
																		const std::string& key) const;

	double GetConstraintAsDouble(const std::string& id, 
															 const std::string& key) const;

	StrItemSeq GetConstraintAsStrItemSeq(const std::string& id, 
																			 const std::string& key) const;

	/* Get Selector property */
	Json::Value GetSelectorProperty(const std::string& type,
																	const std::string& id) const;

 private:
	/*
	@brief GetPropertyAsInt 
	@param domain
	@param id		identity of each json object
	@param key		object key in each json object if exist 
	*/
	int GetPropertyAsInt(const std::string& domain, 
											 const std::string& id, 
											 const std::string& key) const;

	int GetPropertyAsInt(const std::string& id, 
											 const std::string& key) const;
	/*
	@brief GetPropertyAsString 
	@param domain
	@param id		identity of each json object
	@param key		object key in each json object if exist 
	*/
	std::string GetPropertyAsString(const std::string& domain, 
																	const std::string& id, 
																	const std::string& key) const;

	std::string GetPropertyAsString(const std::string& id, 
																	const std::string& key) const;
	/*
	@brief GetPropertyAsDouble
	@param domain
	@param id		identity of each json object
	@param key		object key in each json object if exist 
	*/
	double GetPropertyAsDouble(const std::string& domain, 
														 const std::string& id, 
														 const std::string& key) const;

	double GetPropertyAsDouble(const std::string& id, 
														 const std::string& key) const;
	/*
	@brief GetPropertyAsArray
	@param domain	
	@param id		identity of each json object
	@param key		object key in each json object if exist 
	*/
	StrItemSeq GetPropertyAsArray(const std::string& domain, 
																const std::string& id, 
																const std::string& key) const;

	StrItemSeq GetPropertyAsArray(const std::string& id, 
																const std::string& key) const;
	/*
	@brief GetPropertyAsValue
	@param domain	
	@param id		second key of each json object
	@param key		object id in each json object if exist 
	*/
	Json::Value GetPropertyAsValue(const std::string& domain,
																 const std::string& id,
																 const std::string& key) const;

	//Convert file to string type
	std::string Convert(const std::string& file) const;

	//Print json object
	void Print() const;

	//Swap with local root_
	void Swap(Json::Value& sr);	

	//Check config file according to time
	static void Check(const std::string& file, int time);

	mutable ReadWriteLock lock_;		//use rwlock to get better performance
	Json::Value root_;
};

}
}

#endif
