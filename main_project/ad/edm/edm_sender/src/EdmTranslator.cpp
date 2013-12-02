/*
 *  EdmTranslator.cpp
 *  @Author: jieyu.zeng
 *  @Date: 2012-02-24
 *  @Description: bid unit数据化工具
 *
 */
#include "EdmTranslator.h"
#include "EdmInvertedIndex.h"
#include <bitset>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "EdmStruct.h"
typedef unsigned long uint64_t;

using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace MyUtil;
using namespace boost;
using namespace com::xiaonei::xce;

void Translator::ProcessTargetUnit(const TargetUnit& unit, const AdGroupPtr& group, DimIndexPtr& dim_index) {
  vector<int> int_keys;
  vector<uint64_t> uint64_keys;

  if(Translator::ProcessGender(unit.gender, int_keys)) {
    dim_index->gender_index_->Add(int_keys, group);
    int_keys.clear();
  }
  if(Translator::ProcessStage(unit.stage, int_keys)) {
    dim_index->stage_index_->Add(int_keys, group);
    int_keys.clear();
  }
  if(Translator::ProcessAge(unit.age, int_keys)) {
    dim_index->age_index_->Add(int_keys, group);
    int_keys.clear();
  }

  if ((1 == unit.stage) && Translator::ProcessArea(unit.area, uint64_keys)){
    dim_index->area_index_->Add(uint64_keys, group);
    uint64_keys.clear();
  } else if(2 == unit.stage) {//只有stage == 2时才允许指定学校投放
    vector<uint64_t> uint64_keys2;
    if(Translator::ProcessSchool(unit.school, uint64_keys, uint64_keys2)) {
      dim_index->school_area_index_->Add(uint64_keys, group);
      dim_index->school_index_->Add(uint64_keys2, group);
      uint64_keys.clear();
      uint64_keys2.clear();
    }
    if(Translator::ProcessGrade(unit.grade, int_keys)) {
      dim_index->grade_index_->Add(int_keys, group);
      int_keys.clear();
    }
  } else if((4 == unit.stage) && Translator::ProcessArea(unit.area, uint64_keys)) {
    dim_index->area_index_->Add(uint64_keys, group);
    uint64_keys.clear();
  } else if(6 == unit.stage && Translator::ProcessArea(unit.area, uint64_keys)) {
    dim_index->area_index_->Add(uint64_keys, group);
    uint64_keys.clear();
    int_keys.push_back(-1);
    dim_index->grade_index_->Add(int_keys, group);
    int_keys.clear();
  } else if (7 == unit.stage && Translator::ProcessArea(unit.area, uint64_keys)) {
    dim_index->area_index_->Add(uint64_keys, group);
    uint64_keys.clear();
    int_keys.push_back(-1);
    dim_index->grade_index_->Add(int_keys, group);
    int_keys.clear();
  }

  //TODO 增加根据公司和兴趣定向 
  if (Translator::ProcessCompany(unit.company, uint64_keys)){
    dim_index->company_index_->Add(uint64_keys, group);
    uint64_keys.clear();
  }
  if (Translator::ProcessInterest(unit.interest, uint64_keys)){
    dim_index->interest_index_->Add(uint64_keys, group);
    uint64_keys.clear();
  }
  //TODO end
}

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

bool Translator::ProcessCompany(const std::string& company_content, std::vector<uint64_t>& companys){
	try {
		const uint64_t OTHER_MD5 = 0xba7790b1708b71cb;
		bool isvalid;
		uint64_t out_range;
		if(company_content.empty()){
			//0表示所有
			companys.push_back(OTHER_MD5);
		}else	{
			vector<string>  temp;
			boost::algorithm::split_regex(temp, company_content,  boost::regex(","));
			for (vector<string>::iterator it = temp.begin(); it != temp.end(); ++it){
				char* chTemp = NULL;
				if(it->size() != 16) {
					continue;
				}
				isvalid = true;
				for (int i=0;i<16;i++) {
					if(((*it)[i]>='0' && (*it)[i]<='9')||((*it)[i]>='a' && (*it)[i]<='f')) {
					} else {
						isvalid = false;
						break;
					}
				}
				if(!isvalid) {
					continue;
				}
				out_range = strtoul(it->c_str(), &chTemp, 16);
				if(out_range) {
					companys.push_back(out_range);
				}
			}
		}
		if(companys.empty()) {
			companys.push_back(OTHER_MD5);
		}
		return true;
	}catch (const std::exception &e){
		MCE_WARN("[processBid] exception when process company: " << e.what() << ", company_content = " << company_content);
	}catch (...){
		MCE_WARN("[processBid] unknown exception when process company, company_content = " << company_content);
	}
	return false;
}

bool Translator::ProcessInterest(const std::string& interest_content, std::vector<uint64_t>& interests){
	try{
		const uint64_t OTHER_MD5 = 0xba7790b1708b71cb;
		bool isvalid;
		uint64_t out_range;
		if(interest_content.empty()){
			//0表示所有
			interests.push_back(OTHER_MD5);
		}else	{
			vector<string>  temp;
			boost::algorithm::split_regex(temp, interest_content,  boost::regex(","));
			for (vector<string>::iterator it = temp.begin(); it != temp.end(); ++it){
				char* chTemp = NULL;
				if(it->size() != 16) {
					continue;
				}
				isvalid = true;
				for (int i=0;i<16;i++) {
					if(((*it)[i]>='0' && (*it)[i]<='9')||((*it)[i]>='a' && (*it)[i]<='f')) {
					} else {
						isvalid = false;
						break;
					}
				}
				if(!isvalid) {
					continue;
				}
				out_range = strtoul(it->c_str(), &chTemp, 16);
				if(out_range) {
					interests.push_back(out_range);
				}
			}
		}
		if(interests.empty()) {
			interests.push_back(OTHER_MD5);
		}
		return true;
	}catch (const std::exception &e){
		MCE_WARN("[processBid] exception when process interest: " << e.what() << ", interest_content = " << interest_content);
	}catch (...){
		MCE_WARN("[processBid] unknown exception when process interest, interest_content = " << interest_content);
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


