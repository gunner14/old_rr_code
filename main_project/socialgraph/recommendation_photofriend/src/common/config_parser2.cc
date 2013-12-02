#include "config_parser2.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "UtilCxx/src/LogMonitor.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xce {
namespace socialgraph {

void ConfigParser::Initialize(const std::string& file) {
	// 1. process content and create json object
	// 2. fill local json object
	// 3. launch check thread
	Json::Reader reader;
	Json::Value sr;
	std::string content = Convert(file);		//Get content of file
	bool parse_success = reader.parse(content, sr, false);
	if (!parse_success) {
		MCE_WARN("parse failed!! " << reader.getFormatedErrorMessages());
		return;
	}

	Swap(sr);

	boost::thread* launch_thread = new boost::thread(boost::bind(&ConfigParser::Check, file, CHECK_INTERVAL));
}

//--------------------------------------------------

int ConfigParser::GetConfigAsInt(const std::string& id, const std::string& key) const {
	return GetPropertyAsInt(DOMAIN_CONFIG, id, key);
}

std::string ConfigParser::GetConfigAsString(const std::string& id, const std::string& key) const {
	return GetPropertyAsString(DOMAIN_CONFIG, id, key);
}

double ConfigParser::GetConfigAsDouble(const std::string& id, const std::string& key) const {
	return GetPropertyAsDouble(DOMAIN_CONFIG, id, key);
}

StrItemSeq ConfigParser::GetConfigAsStrItemSeq(const std::string& id, const std::string& key) const {
	return GetPropertyAsArray(DOMAIN_CONFIG, id, key); 
}

//--------------------------------------------------

int ConfigParser::GetPriorityAsInt(const std::string& id, const std::string& key) const {
	return GetPropertyAsInt(id, key);
}

std::string ConfigParser::GetPriorityAsString(const std::string& id, const std::string& key) const {
	return GetPropertyAsString(id, key);
}

double ConfigParser::GetPriorityAsDouble(const std::string& id, const std::string& key) const {
	return GetPropertyAsDouble(id, key);
}

StrItemSeq ConfigParser::GetPriorityAsStrItemSeq(const std::string& id, const std::string& key) const {
	return GetPropertyAsArray(id, key);
}

//--------------------------------------------------

int ConfigParser::GetRuleAsInt(const std::string& id, const std::string& key) const {
	return GetPropertyAsInt(id, key);
}

std::string ConfigParser::GetRuleAsString(const std::string& id, const std::string& key) const {
	return GetPropertyAsString(id, key);
}

double ConfigParser::GetRuleAsDouble(const std::string& id, const std::string& key) const {
	return GetPropertyAsDouble(id, key);
}

StrItemSeq ConfigParser::GetRuleAsStrItemSeq(const std::string& id, const std::string& key) const {
	return GetPropertyAsArray(id, key);
}

//--------------------------------------------------

int ConfigParser::GetConstraintAsInt(const std::string& id, const std::string& key) const {
	return GetPropertyAsInt(id, key);
}

std::string ConfigParser::GetConstraintAsString(const std::string& id, const std::string& key) const {
	return GetPropertyAsString(id, key);
}

double ConfigParser::GetConstraintAsDouble(const std::string& id, const std::string& key) const {
	return GetPropertyAsDouble(id, key);
}

StrItemSeq ConfigParser::GetConstraintAsStrItemSeq(const std::string& id, const std::string& key) const {
	return GetPropertyAsArray(id, key);
}

//--------------------------------------------------

Json::Value ConfigParser::GetSelectorProperty(const std::string& id, const std::string& key) const {
	return GetPropertyAsValue(DOMAIN_SELECTOR, id, key);
}

//--------------------------------------------------

int ConfigParser::GetPropertyAsInt(const std::string& domain, 
										 							 const std::string& id, 
										 							 const std::string& key) const {
	int result = 0;
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[domain];
		if (!obj.isNull()) {
			const Json::Value& target_obj	= obj[id];
			if (!target_obj.isNull()) {
				if (target_obj[key].isInt()) {
					result = target_obj[key].asInt();
				}
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsInt Parse Error domain("
				<< domain << ") id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();
	return result;
}

int ConfigParser::GetPropertyAsInt(const std::string& id, 
											 						 const std::string& key) const {
	int result = 0;
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[id];
		if (!obj.isNull()) {
			if (obj[key].isInt()) {
				result = obj[key].asInt();
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsInt Parse Error id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();
	return result;
}

std::string ConfigParser::GetPropertyAsString(const std::string& domain, 
																							const std::string& id, 
																							const std::string& key) const {
	std::string result = "";
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[domain];
		if (!obj.isNull()) {
			const Json::Value& target_obj	= obj[id];
			if (!target_obj.isNull()) {
				if (target_obj[key].isString()) {
					result = target_obj[key].asString();
				}
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsString Parse Error domain("
				<< domain << ") id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();
	return result;
}

std::string ConfigParser::GetPropertyAsString(const std::string& id, 
																							const std::string& key) const {
	std::string result = "";
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[id];
		if (!obj.isNull()) {
			if (obj[key].isString()) {
				result = obj[key].asString();
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsString Parse Error id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();
	return result;
}

double ConfigParser::GetPropertyAsDouble(const std::string& domain, 
													 							 const std::string& id, 
													 							 const std::string& key) const {
	double result = 0.0;
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[domain];
		if (!obj.isNull()) {
			const Json::Value& target_obj	= obj[id];
			if (!target_obj.isNull()) {
				if (target_obj[key].isDouble()) {
					result = target_obj[key].asDouble();
				}
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsDouble Parse Error domain("
				<< domain << ") id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();
	return result;
}

double ConfigParser::GetPropertyAsDouble(const std::string& id, 
														 						 const std::string& key) const {
	double result = 0.0;
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[id];
		if (!obj.isNull()) {
			if (obj[key].isDouble()) {
				result = obj[key].asDouble();
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsDouble Parse Error id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();
	return result;
}

StrItemSeq ConfigParser::GetPropertyAsArray(const std::string& domain, 
																						const std::string& id, 
																						const std::string& key) const {
	StrItemSeq result;
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[domain];
		if (!obj.isNull()) {								//obj is available
			const Json::Value& target_obj	= obj[id];
			if (!target_obj.isNull()) {						//obj is available
				if (target_obj[key].isArray()) {				//is array
					for (Json::Value::const_iterator iter = target_obj[key].begin();
							iter != target_obj[key].end(); ++iter) {
						if ((*iter).isString()) {
							result.push_back((*iter).asString());
						}
					}
				}
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsArray Parse Error domain("
				<< domain << ") id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();

	return result;
}

StrItemSeq ConfigParser::GetPropertyAsArray(const std::string& id, 
																						const std::string& key) const {
	StrItemSeq result;
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[id];
		if (!obj.isNull()) {
			if (obj[key].isArray()) {
				for (Json::Value::const_iterator iter = obj[key].begin();
						iter != obj[key].end(); ++iter) {
					if ((*iter).isString()) {
						result.push_back((*iter).asString());
					}
				}
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsArray Parse Error id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();
	return result;
}

Json::Value ConfigParser::GetPropertyAsValue(const std::string& domain,
															 							 const std::string& id,
															 							 const std::string& key) const {
	Json::Value result;
	lock_.ReadLock();

	try {
		const Json::Value& obj = root_[domain];
		if (!obj.isNull()) {
			const Json::Value& target_obj = obj[id];
			if (!target_obj.isNull() &&
					!target_obj[key].isNull()) {
				result = target_obj[key];	
			}
		}
	} catch (...) {
		MCE_WARN("ConfigParser::GetPropertyAsValue Parse Error domain("
				<< domain << ") id(" << id << ") key(" << key << ")");
	}

	lock_.ReadUnlock();

	return result;
}

void ConfigParser::Print() const {
	lock_.ReadLock();

	MCE_INFO(root_);

	lock_.ReadUnlock();
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

void ConfigParser::Swap(Json::Value& sr) {
	lock_.WriteLock();
	try {
		root_.swap(sr);
	} catch (...) {
		MCE_WARN("ConfigParser::Swap");
	}

	lock_.WriteUnlock();
	Print();
}

void ConfigParser::Check(const std::string& file, int time) {
	while (true) {
		Json::Reader reader;
		Json::Value root;
		std::string content = ConfigParser::instance().Convert(file);		//Get content of file
		bool parse_success = reader.parse(content, root, false);
		if (!parse_success) {
			MCE_WARN("parse failed!! " << reader.getFormatedErrorMessages());
			return;
		}

		ConfigParser::instance().Swap(root);

		sleep(time);		//interval of checking file
	}
}

}
}
