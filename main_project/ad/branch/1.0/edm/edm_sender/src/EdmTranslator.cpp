/*
 *  EdmTranslator.cpp
 *  @Author: jieyu.zeng
 *  @Date: 2012-02-24
 *  @Description: bid unit数据化工具
 *
 */
#include "EdmTranslator.h"
#include <bitset>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "EdmStruct.h"

using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace MyUtil;
using namespace boost;
using namespace com::xiaonei::xce;

bool Translator::ProcessGender(const int gender_code, vector<int>& genders) {
  try {
    if(gender_code == 0 || gender_code == 1) {
      genders.push_back((int)gender_code);
    } else if(gender_code == 2) {
      genders.push_back(0);
      genders.push_back(1);
    } 
    return true;
  } catch (std::exception e) {
    MCE_WARN("[ProcessBid] exception when process gender: " << e.what() << ", gender_code = " << gender_code);
  } catch (...) {
    MCE_WARN("[ProcessBid] unknown exception when process gender, gender_code = " << gender_code);
  }
  return false;
}

bool Translator::ProcessStage(const int stage_code, vector<int>& stages) {
  try {
    if(stage_code & 1) {
      stages.push_back(1);
    }
    if(stage_code & 2) {
      stages.push_back(2);
    }
    if(stage_code & 4) {
      stages.push_back(4);
    }
    return true;
  } catch (std::exception e) {
    MCE_WARN("[ProcessBid] exception when process stage: " << e.what() << ", stage_code = " << stage_code);
  } catch (...) {
    MCE_WARN("[ProcessBid] unknown exception when process stage, stage_code = " << stage_code);
  }
  return false;
}


bool Translator::ProcessGrade(const string& grade_code, vector<int>& grades) {
  try {
    if(grade_code.empty()) {
      //-1表示所有
      grades.push_back(-1);
    } else {
      typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
      boost::char_separator<char> sep(";");
      tokenizer tokens(grade_code, sep);
      for(tokenizer::iterator tok_iter = tokens.begin();
          tok_iter != tokens.end(); ++tok_iter) {
        boost::char_separator<char> sep2(",");
        tokenizer tokens2(*tok_iter, sep2);
        int begin = 0;
        int end = 0;
        int num = 1;
        for (tokenizer::iterator tok_iter2 = tokens2.begin();
            tok_iter2 != tokens2.end(); ++tok_iter2, ++num) {
          if(num == 1) {
            begin = boost::lexical_cast<int>(*tok_iter2);
          } else if(num == 2) {
            end = boost::lexical_cast<int>(*tok_iter2);
          }
        }
        if(!end && begin > 0) {
          end = begin;
        }
        for(int i = begin; i <= end; ++i) {
          grades.push_back(i);
        }
      }
    }
    return true;
  } catch (std::exception e) {
    MCE_WARN("[ProcessBid] exception when process grade: " << e.what() << ", grade_code = " << grade_code);
  } catch (...) {
    MCE_WARN("[ProcessBid] unknown exception when process grade, grade_code = " << grade_code);
  }
  return false;
}
bool Translator::ProcessAge(const string& age_code, vector<int>& ages) {
  try {
    if(age_code.empty()) {
      //-1表示所有
      ages.push_back(-1);
    } else {
      typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
      boost::char_separator<char> sep(";");
      tokenizer tokens(age_code, sep);
      for(tokenizer::iterator tok_iter = tokens.begin();
          tok_iter != tokens.end(); ++tok_iter) {
        boost::char_separator<char> sep2(",");
        tokenizer tokens2(*tok_iter, sep2);
        int begin = 0;
        int end = 0;
        int num = 1;
        for (tokenizer::iterator tok_iter2 = tokens2.begin();
            tok_iter2 != tokens2.end(); ++tok_iter2, ++num) {
          if(num == 1) {
            begin = boost::lexical_cast<int>(*tok_iter2);
          } else if(num == 2) {
            end = boost::lexical_cast<int>(*tok_iter2);
          }
        }
        if(!end && begin > 0) {
          end = begin;
        }
        for(int i = begin; i <= end; ++i) {
          ages.push_back(i);
        }
      }
    }
    return true;
  } catch (std::exception e) {
    MCE_WARN("[ProcessBid] exception when process age: " << e.what() << ", age_code = " << age_code);
  } catch (...) {
    MCE_WARN("[ProcessBid] unknown exception when process age, age_code = " << age_code);
  }
  return false;
}

bool Translator::ProcessSchool(const string& school_code, vector<uint64_t>& areas, vector<uint64_t>& schools) {
  try {
    if(school_code.empty()) {
      //0表示所有
      schools.push_back(0);
    } else {
      vector<string> result;
      boost::algorithm::split(result, school_code, is_any_of(";"));
      int num = 1;
      vector<uint64_t> temp;
      for(vector<string>::iterator it = result.begin(); it != result.end(); ++it, ++num) {
        if(num == 1 && !(*it).empty()) {
          if(ProcessLeadingArea(*it, areas)) {
          }
        } else if(num == 2 && !(*it).empty()) {
          if(ProcessRealSchool(*it, schools)) {
          }
        }
      }
    }
    return true;
  } catch (std::exception e) {
    MCE_WARN("[ProcessBid] exception when process school: " << e.what() << ", school_code = " << school_code);
  } catch (...) {
    MCE_WARN("[ProcessBid] unknown exception when process school, school_code = " << school_code);
  }
  return false;
}

bool Translator::ProcessLeadingArea(const string& area_code, vector<uint64_t>& areas) {
  try {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(",");
    tokenizer tokens(area_code, sep);
    for(tokenizer::iterator tok_iter = tokens.begin();
        tok_iter != tokens.end(); ++tok_iter) {
      const string& str = *tok_iter;
      size_t pos = str.find("0086");
      if(pos != string::npos) {
        uint64_t area = boost::lexical_cast<uint64_t>(str.substr(pos));
        areas.push_back(area);
      }
    }
    return true;
  } catch (std::exception e) {
    MCE_WARN("[ProcessBid] exception when process LeadingArea: " << e.what() << ", LeadingArea_code = " << area_code);
  } catch (...) {
    MCE_WARN("[ProcessBid] unknown exception when process LeadingArea, LeadingArea_code = " << area_code);
  }
  return false;
}

bool Translator::ProcessRealSchool(const string& school_code, vector<uint64_t>& schools) {
  try {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(",");
    tokenizer tokens(school_code, sep);
    for(tokenizer::iterator tok_iter = tokens.begin();
        tok_iter != tokens.end(); ++tok_iter) {
      uint64_t school = boost::lexical_cast<uint64_t>(*tok_iter);
      schools.push_back(school);
    }
    return true;
  } catch (std::exception e) {
    MCE_WARN("[ProcessBid] exception when process RealSchool: " << e.what() << ", RealSchool_code = " << school_code);
  } catch (...) {
    MCE_WARN("[ProcessBid] unknown exception when process RealSchool, RealSchool_code = " << school_code);
  }
  return false;
}

bool Translator::ProcessArea(const string& area_code, vector<uint64_t>& areas) {
  try {
    if(area_code.empty()) {
      //0表示所有
      areas.push_back(0);
    } else {
      typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
      boost::char_separator<char> sep(",");
      tokenizer tokens(area_code, sep);
      for(tokenizer::iterator tok_iter = tokens.begin();
          tok_iter != tokens.end(); ++tok_iter) {
        uint64_t area = boost::lexical_cast<uint64_t>(*tok_iter);
        areas.push_back(area);
      }
    }
    return true;
  } catch (std::exception e) {
    MCE_WARN("[ProcessBid] exception when process area: " << e.what() << ", area_code = " << area_code);
  } catch (...) {
    MCE_WARN("[ProcessBid] unknown exception when process area, area_code = " << area_code);
  }
  return false;
}

/***********************************************************************************************************************/
vector<uint64_t> CommonUtil::Split(const string& str, boost::char_separator<char> separator) {
  vector < uint64_t > ans;
  Ice::Long integer = 0;
  boost::tokenizer < boost::char_separator<char> > tok(str, separator);
  for (boost::tokenizer<boost::char_separator<char> >::iterator iter =
      tok.begin(); iter != tok.end(); ++iter) {
    try {
      integer = boost::lexical_cast<uint64_t>(*iter);
      ans.push_back(integer);
    } catch (...) {
      MCE_WARN("Translator::Split --> boost cast " << (*iter)
          << " to Ice::Long error");
    }
  }
  return ans;
}


