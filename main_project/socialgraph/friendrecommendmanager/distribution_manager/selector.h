#ifndef SELECTOR_H_
#define SELECTOR_H_

#include <iostream>
#include <string>
#include <map>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/mutex.hpp>
#include "json/json.h"

#include "socialgraph/friendrecommendmanager/common/config_parser2.h"
#include "socialgraph/friendrecommendmanager/common/id_filter.h"

namespace xce {
namespace socialgraph {

class Selector {
 public:
	virtual bool Access(int userid, const std::string& exp) = 0;
	
};
typedef boost::shared_ptr<Selector> SelectorPtr;

//--------------------------------------

class TrueSelector : public Selector {
 public:
	TrueSelector() {};
	
	bool Access(int userid, const std::string& exp) {return true;};
};

//--------------------------------------

class FalseSelector : public Selector {
 public:
	FalseSelector() {};
	
	bool Access(int userid, const std::string& exp) {return false;};
};

//--------------------------------------

class RandomSelector : public Selector {
 public:
	RandomSelector(const std::string& id);

	bool Access(int userid, const std::string& exp);

 private:
	void CreateCurrent();

	double current_ratio_;
	std::string current_exp_;

	std::vector<std::pair<std::string, double> > ratio_list_;
};

//--------------------------------------

class NewRandomSelector : public Selector {
 public:
	NewRandomSelector(const std::string& id);

	bool Access(int userid, const std::string& exp);

 private:
	void CreateCurrent();

	double current_ratio_;
	std::string current_exp_;

	std::vector<std::pair<std::string, double> > ratio_list_;
};

//--------------------------------------

class WhiteListSelector : public Selector {
 public:
	WhiteListSelector(const std::string& id);

	bool Access(int userid, const std::string& exp);

 private:
	std::string file_;
};

//--------------------------------------

class TailnumSelector : public Selector {
 public:
	TailnumSelector(const std::string& id);

	bool Access(int userid, const std::string& exp);

 private:
	int index_;
};

//--------------------------------------

class SelectorFactory {
 public:
	static SelectorPtr Create(const std::string& type, 
														const std::string& id);
};

}
}

#endif
