#include "config_parser.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "UtilCxx/src/LogMonitor.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xce {
namespace socialgraph {

ConfigParser::ConfigParser() {
}

ConfigParser::~ConfigParser() {
}

void ConfigParser::Initialize(const std::string& file) {
	// 1. process content and create json object
	// 2. fill local config basket
	// 3. launch check thread
	Json::Reader reader;
	Json::Value root;
	std::string content = Convert(file);		//Get content of file
	bool parse_success = reader.parse(content, root, false);
	if (!parse_success) {
		std::cout << "parse failed!! " << reader.getFormatedErrorMessages() << std::endl;	
		return;
	}

	Fill(root);

	boost::thread* launch_thread = new boost::thread(boost::bind(&ConfigParser::Check, file, CHECK_INTERVAL));
}

std::string ConfigParser::Get(const Domain domain, 
															const std::string& key, const std::string& second_key) const {
	switch (domain) {					//determine domain, using different bucket(Online, Experiment)
		case Online:
			return Get(rules_bucket_, key, second_key);
		case Experiment:
			return Get(experiment_bucket_, key, second_key);
		default:
			return NULL;
	}
	return NULL;
}

std::string ConfigParser::Get(const AttributeBuckets& buckets, 
															const std::string& key, const std::string& second_key) const {
	lock_.ReadLock();
	AttributeBuckets::const_iterator fiter = buckets.find(key);
	if (fiter == buckets.end()) {
		lock_.ReadUnlock();
		return ""; 
	}

	AttributeBucket::const_iterator ffiter = fiter->second.find(second_key);
	if (ffiter == fiter->second.end()) {
		lock_.ReadUnlock();
		return "";
	}
	std::string v = ffiter->second;
	lock_.ReadUnlock();

	return v;
}

std::string ConfigParser::Convert(const std::string& file) const {
	std::ifstream infile(file.c_str(), std::ifstream::in);
	std::string line = "";
	std::string content = "";

	while (getline(infile, line)) {
		content += line;
		content += "\n";
		line.clear();
	}
	infile.close();

	return content;
}

bool ConfigParser::Fill(const Json::Value& root) {
	std::string rules = "rules";
	std::string experiment = "experiment";

	//process rules_bucket
	FillBuckets(root[rules], "rule_name", rules_bucket_);
	Print(rules_bucket_);
	
	//process experiment_bucket
	FillBuckets(root[experiment], "experiment_name", experiment_bucket_);
	Print(experiment_bucket_);

	return true;
}

void ConfigParser::Check(const std::string& file, int time) {
	while (true) {
		Json::Reader reader;
		Json::Value root;
		std::string content = ConfigParser::instance().Convert(file);		//Get content of file
		bool parse_success = reader.parse(content, root, false);
		if (!parse_success) {
			std::cout << "parse failed!! " << reader.getFormatedErrorMessages() << std::endl;	
			return;
		}

		ConfigParser::instance().Fill(root);

		sleep(time);		//interval of checking file
	}
}

void ConfigParser::Print(const AttributeBuckets& buckets) const {
	for (AttributeBuckets::const_iterator iter = buckets.begin();
			iter != buckets.end(); ++iter) {
		//std::cout << iter->first << ":" << std::endl;
		MCE_INFO(iter->first << ":");
		for (AttributeBucket::const_iterator it = iter->second.begin();
				it != iter->second.end(); ++it) {
			std::string tmp = "";
			tmp += it->first;
			tmp += " -- ";
			tmp += it->second;
			//std::cout << tmp << std::endl;
			MCE_INFO(tmp);
		}
		//std::cout << "**********************" << std::endl;
		MCE_INFO("**********************");
	}
}

void ConfigParser::FillBuckets(const Json::Value& root, const std::string& key_name, AttributeBuckets& target) {
	AttributeBuckets tmp_target_bucket;

	for (int index = 0; index < root.size(); ++index) {
		AttributeBucket item_bucket;
		Json::Value::Value item_value = root[index];
		Json::Value::Members item_members = item_value.getMemberNames();
		for (Json::Value::Members::iterator iter = item_members.begin();
				iter != item_members.end(); ++iter) {
			item_bucket[*iter] = item_value[*iter].asString();
		}
		std::string key = item_value[key_name].asString();
		tmp_target_bucket[key] = item_bucket;
	}

	lock_.WriteLock();
	target.swap(tmp_target_bucket);
	lock_.WriteUnlock();
}

}}
