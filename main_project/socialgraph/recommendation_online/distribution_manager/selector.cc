#include "selector.h"

#include "UtilCxx/src/LogMonitor.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xce {
namespace socialgraph {

RandomSelector::RandomSelector(const std::string& id) {
	Json::Value v = ConfigParser::instance().GetSelectorProperty(ID_RANDOM, id);
	if (v.isNull())
		return;

	for (int i = 1; i < 10; ++i) {		//10 is max
		std::string tmp = "E";
		tmp += i + '0';
		if (v.isMember(tmp)) {
			ratio_list_.push_back(std::make_pair<std::string, double>(tmp, v[tmp].asDouble()));
		}
	}

	for (std::vector<std::pair<std::string, double> >::iterator iter = ratio_list_.begin();
			iter != ratio_list_.end(); ++iter) {
		MCE_DEBUG(iter->first << " : " << iter->second);
	}
	current_exp_ = ID_O;			//"O" as default
	CreateCurrent();		//create current experiment tag
}

void RandomSelector::CreateCurrent() {
	srand(time(NULL));
	current_ratio_ = (double)rand() / RAND_MAX;					// random once
	
	MCE_DEBUG("[RandomSelector] CreateCurrent current_ratio_ : " << current_ratio_)
	double base = 0.0;
	for (std::vector<std::pair<std::string, double> >::iterator iter = ratio_list_.begin();
			iter != ratio_list_.end(); ++iter) {
		base = base + iter->second;
		if (current_ratio_ < base) {
			current_exp_ = iter->first;
			break;
		}
	}
	MCE_DEBUG("current_exp_ : " << current_exp_);
}

bool RandomSelector::Access(int userid, const std::string& exp) {
	return exp == current_exp_;
}

//--------------------------------------

NewRandomSelector::NewRandomSelector(const std::string& id) {
	Json::Value v = ConfigParser::instance().GetSelectorProperty(ID_RANDOM, id);
	if (v.isNull())
		return;

	Json::Value::Members members = v.getMemberNames();
	for (Json::Value::Members::iterator iter = members.begin();
			iter != members.end(); ++iter) {
		ratio_list_.push_back(std::make_pair<std::string, double>(*iter, v[*iter].asDouble()));
	}

	CreateCurrent();		//create current experiment tag
}

void NewRandomSelector::CreateCurrent() {
	srand(time(NULL));
	current_ratio_ = (double)rand() / RAND_MAX;					// random once
	
	MCE_INFO("[NewRandomSelector] CreateCurrent current_ratio_ : " << current_ratio_)
	double base = 0.0;
	for (std::vector<std::pair<std::string, double> >::iterator iter = ratio_list_.begin();
			iter != ratio_list_.end(); ++iter) {
		base = base + iter->second;
		if (current_ratio_ < base) {
			current_exp_ = iter->first;
			break;
		}
	}
	MCE_INFO("current_exp_ : " << current_exp_);
}

bool NewRandomSelector::Access(int userid, const std::string& exp) {
	return exp == current_exp_;
}

//--------------------------------------

WhiteListSelector::WhiteListSelector(const std::string& id) {
	Json::Value v = ConfigParser::instance().GetSelectorProperty(ID_WHITELIST, id);
	if (!v.isNull()) {
		file_ = v["file"].asString();
	}
}

bool WhiteListSelector::Access(int userid, const std::string& exp) {
	bool result = false;
	if (IdFilter::instance().IsExist(file_, userid)) {
		result = true;
	}
	return result;
}

//--------------------------------------

TailnumSelector::TailnumSelector(const std::string& id) {
	Json::Value v = ConfigParser::instance().GetSelectorProperty(ID_TAILNUM, id);
	if (!v.isNull()) {
		index_ = v["index"].asInt();
	}
}

bool TailnumSelector::Access(int userid, const std::string& exp) {
	bool result = false;
	if (userid % 10 == index_) {
		result = true;
	} 

	return result;
}

//--------------------------------------

SelectorPtr SelectorFactory::Create(const std::string& type, 
																	  const std::string& id) {
	if (ID_WHITELIST == type) {
		return SelectorPtr(new WhiteListSelector(id));	
	} else if (ID_TAILNUM == type) {
		return SelectorPtr(new TailnumSelector(id));
	} else if (ID_RANDOM == type) {
		return SelectorPtr(new NewRandomSelector(id));
	} else if ("trueselector" == type) {
		return SelectorPtr(new TrueSelector());
	} else if ("falseselector" == type) {
		return SelectorPtr(new FalseSelector());
	} 
	
	return SelectorPtr();
}

}
}
