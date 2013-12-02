/*
 * =====================================================================================
 *
 *       Filename:  BayesRegression.h
 *
 *    Description:  Bayes计算
 *
 *        Version:  1.0
 *        Created:  2012年11月09日 15时02分21秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Duo Zhang (), duo.zhang@renren-inc.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _FEEDNEW_BAYESREGRESSION_H_
#define _FEEDNEW_BAYESREGRESSION_H_

#include "ExperimentManager.h"
#include "PropertiesManager.h"
#include "Singleton.h"
#include <map>
#include <string>

namespace xce {
namespace feed {

typedef std::map<std::string, long> FeatureMap;
class BayesRegression : public MyUtil::Singleton<BayesRegression>, public UpdaterBase {
  typedef std::vector<std::string> Rule;
  typedef std::vector<Rule> RulesType;
public:
  BayesRegression() {
    PropertiesManager::instance().AppendUpdater(this);
  }

  double GetBayes(const FeatureMap & feature);

  virtual void Update();

private:
  std::vector<std::string> getKeyid(const FeatureMap & feature);

  std::string getProperty() {
    ServiceI & service = ServiceI::instance();
    std::ostringstream oskey;
    oskey << "Service." << service.getBusiness() << ".BayesRegressionRules";
    return service.getCommunicator()->getProperties()->getPropertyWithDefault(oskey.str(), "");
  }

private:
  RulesType rules_;
  IceUtil::RWRecMutex mutex_;
};

inline double BayesRegression::GetBayes(const FeatureMap & feature) {
  std::vector<std::string> keys = getKeyid(feature);
  double weight = NOT_FOUND_VALUE;
  for(std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    weight = ExperimentManager::instance().GetValue(*it);
    if (weight != NOT_FOUND_VALUE) {
      return weight;
    }
  }
  return weight;
}

inline void BayesRegression::Update() {
  RulesType temp;
  const std::string & str_rules = getProperty();
  if (str_rules == "") {
    return ;
  }

  try {
    std::istringstream is(str_rules);
    std::string line;
    while(std::getline(is, line)) {
      std::string keyname;
      temp.push_back(Rule());
      for(std::istringstream isrule(line); isrule >> keyname; ) {
        temp.back().push_back(keyname);
      }
    }
  } catch (const std::exception & e) {
    MCE_WARN("BayesRegression::Update std::exception: " << e.what());
  } catch (...) {
    MCE_WARN("BayesRegression::Update Unknown Exception.");
  }

  IceUtil::RWRecMutex::WLock lock(mutex_);
  rules_.swap(temp);
}

inline std::vector<std::string> BayesRegression::getKeyid(const FeatureMap & feature) {
  std::vector<std::string> res;
  IceUtil::RWRecMutex::RLock lock(mutex_);
  for(RulesType::const_iterator it = rules_.begin(); it != rules_.end(); ++it) {
    ostringstream os;
    bool first = true;
    for(Rule::const_iterator rit = it->begin(); rit != it->end(); ++rit) {
      FeatureMap::const_iterator fmit = feature.find(*rit);
      if (fmit == feature.end()) {
        continue;
      }
      if (!first) {
        os << "_";
      } else {
        first = false;
      }
      os << fmit->second;
    }
    res.push_back(os.str());
  }
  res.push_back("ctr");
  return res;
}

}
}

#endif //_FEEDNEW_BAYESREGRESSION_H_
