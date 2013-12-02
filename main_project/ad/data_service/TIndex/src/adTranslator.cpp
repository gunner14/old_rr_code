#include "adTranslator.h"

#include <bitset>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/regex.hpp>

typedef unsigned long uint64_t;

using namespace xce::ad;

bool Translator::processGender(const int gender_code, std::vector<short> &genders) 
{
	if(gender_code == 0 || gender_code == 1) 
	{
		genders.push_back((short)gender_code);
	} 
	else if(gender_code == 2) 
	{
		genders.push_back(0);
		genders.push_back(1);
	} 
	else
	{
		return false;
	}

	return true;

}

bool Translator::processStage(const int stage_code, std::vector<short>& stages) 
{
	if(stage_code & 1) 
	{
		stages.push_back(1);
	}

	if(stage_code & 2) 
	{
		stages.push_back(2);
	}

	if(stage_code & 4) 
	{
		stages.push_back(4);
	}

	return true;
}

bool Translator::processGrade(const std::string& grade_code, std::vector<short>& grades) 
{
	try 
	{
		if(grade_code.empty()) 
		{
			grades.push_back(-1);
		} 
		else 
		{
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(";");
			tokenizer tokens(grade_code, sep);
			for(tokenizer::iterator tok_iter = tokens.begin();
					tok_iter != tokens.end(); ++tok_iter) 
			{
				boost::char_separator<char> sep2(",");
				tokenizer tokens2(*tok_iter, sep2);
				int begin = 0;
				int end = 0;
				int num = 1;
				for (tokenizer::iterator tok_iter2 = tokens2.begin();
						tok_iter2 != tokens2.end(); ++tok_iter2, ++num) 
				{
					if(num == 1) 
					{
						begin = boost::lexical_cast<short>(*tok_iter2);
					} 
					else if(num == 2) 
					{
						end = boost::lexical_cast<short>(*tok_iter2);
					}
				}
				if(!end && begin > 0) 
				{
					end = begin;
				}
				for(short i = begin; i <= end; ++i) 
				{
					grades.push_back(i);
				}
			}
		}
		return true;
	} 
	catch (std::exception e) 
	{
		MCE_WARN("[processBid] exception when process grade:" << e.what() << ",grade_code = " << grade_code);
	} catch (...) {
		MCE_WARN("[processBid] unknown exception when process grade, grade_code = " << grade_code);
	}
	return false;
}

bool Translator::processAge(const std::string& age_code, std::vector<short>& ages) 
{
	try 
	{
		if(age_code.empty()) 
		{
			//-1表示所有
			ages.push_back(-1);
		} 
		else 
		{
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(";");
			tokenizer tokens(age_code, sep);
			for(tokenizer::iterator tok_iter = tokens.begin();
					tok_iter != tokens.end(); ++tok_iter) 
			{
				boost::char_separator<char> sep2(",");
				tokenizer tokens2(*tok_iter, sep2);
				int begin = 0;
				int end = 0;
				int num = 1;
				for (tokenizer::iterator tok_iter2 = tokens2.begin();
						tok_iter2 != tokens2.end(); ++tok_iter2, ++num) 
				{
					if(num == 1) 
					{
						begin = boost::lexical_cast<short>(*tok_iter2);
					} else if(num == 2) 
					{
						end = boost::lexical_cast<short>(*tok_iter2);
					}
				}
				if(!end && begin > 0) 
				{
					end = begin;
				}
				for(short i = begin; i <= end; ++i) 
				{
					ages.push_back(i);
				}
			}
		}
		return true;
	} 
	catch (std::exception e) 
	{
		MCE_WARN("[processBid] exception when process age: " << e.what() << ",age_code = " << age_code);
	}
	catch (...) 
	{
		MCE_WARN("[processBid] unknown exception when process age, age_code = " << age_code);
	}
	return false;
}

bool Translator::processSchool(const std::string& school_code, std::vector<uint64_t>& areas, std::vector<uint64_t>& schools) 
{
	try 
	{
		if(school_code.empty()) 
		{
			//0表示所有
			schools.push_back(0);
		} 
		else 
		{
			std::vector<std::string> result;
			boost::algorithm::split(result, school_code, boost::is_any_of(";"));
			int num = 1;
			std::vector<uint64_t> temp;
			for(std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it, ++num) 
			{
				if(num == 1 && !(*it).empty()) 
				{
					if(processLeadingArea(*it, areas)) 
					{
					}
				}
				else if(num == 2 && !(*it).empty()) 
				{
					if(processRealSchool(*it, schools)) 
					{
					}
				}
			}
		}
		return true;
	} 
	catch (std::exception e) 
	{
		MCE_WARN("[processBid] exception when process school: " << e.what() << ",school_code = " <<  school_code);
	} 
	catch (...) 
	{
		MCE_WARN("[processBid] unknown exception when process school, school_code = " << school_code);
	}
	return false;
}

bool Translator::processLeadingArea(const std::string& area_code, std::vector<uint64_t>& areas) 
{
	//area0086110000000000,area0086210000000000,area0086310000000000,area0086440000000000,area0086510000000000
	try 
	{
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(",");
		tokenizer tokens(area_code, sep);
		for(tokenizer::iterator tok_iter = tokens.begin();
				tok_iter != tokens.end(); ++tok_iter) 
		{
			const std::string& str = *tok_iter;
			size_t pos = str.find("0086");
			if(pos != std::string::npos) 
			{
				uint64_t area = boost::lexical_cast<uint64_t>(str.substr(pos));
				areas.push_back(area);
			}
		}
		return true;
	} 
	catch (std::exception e) 
	{
		MCE_WARN("[processBid] exception when process LeadingArea: " << e.what() << ",LeadingArea_code= " << area_code);
	}
	catch (...) 
	{
		MCE_WARN("[processBid] unknown exception when process LeadingArea, LeadingArea_code="  << area_code);
	}
	return false;
}

bool Translator::processRealSchool(const std::string& school_code, std::vector<uint64_t>& schools) 
{
	//400000001011,400000001003,400000001056,400000001020,400000001032,400000001036
	try 
	{
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(",");
		tokenizer tokens(school_code, sep);
		for(tokenizer::iterator tok_iter = tokens.begin();
				tok_iter != tokens.end(); ++tok_iter) 
		{
			uint64_t school = boost::lexical_cast<uint64_t>(*tok_iter);
			schools.push_back(school);
		}
		return true;
	} 
	catch (std::exception e) 
	{
		MCE_WARN("[processBid] exception when process RealSchool: " << e.what() << ", RealSchool_code = " << school_code);
	} 
	catch (...) {
		MCE_WARN("[processBid] unknown exception when process RealSchool, RealSchool_code = " << school_code);
	}
	return false;
}

bool Translator::processArea(const std::string& area_code, std::vector<uint64_t>& areas) 
{
	//0086110000000000,0086310000000000,0086440100000000,0086440300000000
	try 
	{
		if(area_code.empty()) 
		{
			//0表示所有
			areas.push_back(0);
		}
		else 
		{
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(",");
			tokenizer tokens(area_code, sep);
			for(tokenizer::iterator tok_iter = tokens.begin();
					tok_iter != tokens.end(); ++tok_iter) 
			{
				uint64_t area = boost::lexical_cast<uint64_t>(*tok_iter);
				areas.push_back(area);
			}
		}
		return true;
	} 
	catch (const std::exception &e) 
	{
		MCE_WARN("[processBid] exception when process area: " << e.what() << ", area_code = " << area_code);
	} 
	catch (...) 
	{
		MCE_WARN("[processBid] unknown exception when process area, area_code = " << area_code);
	}
	return false;
}

bool Translator::processCompany(const std::string& company_content, std::vector<uint64_t>& companys){
  try 
  {

    const uint64_t OTHER_MD5 = 0xba7790b1708b71cb;
    bool isvalid;
    uint64_t out_range;
    if(company_content.empty()) 
    {
      //0表示所有
      companys.push_back(OTHER_MD5);
    }
    else 
    {
			vector<string>  temp;
      boost::algorithm::split_regex(temp, company_content,  boost::regex(","));
      for (vector<string>::iterator it = temp.begin(); it != temp.end(); ++it)
      {
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
  } 
  catch (const std::exception &e) 
  {
    MCE_WARN("[processBid] exception when process company: " << e.what() << ", company_content = " << company_content);
  } 
  catch (...) 
  {
    MCE_WARN("[processBid] unknown exception when process company, company_content = " << company_content);
  }
  return false;
}

bool Translator::processInterest(const std::string& interest_content, std::vector<uint64_t>& interests){
  try 
  {

    const uint64_t OTHER_MD5 = 0xba7790b1708b71cb;
    bool isvalid;
    uint64_t out_range;
    if(interest_content.empty()) 
    {
      //0表示所有
       interests.push_back(OTHER_MD5);
    }
    else 
    {
			vector<string>  temp;
      boost::algorithm::split_regex(temp, interest_content,  boost::regex(","));
      for (vector<string>::iterator it = temp.begin(); it != temp.end(); ++it)
      {
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
  } 
  catch (const std::exception &e) 
  {
    MCE_WARN("[processBid] exception when process interest: " << e.what() << ", interest_content = " << interest_content);
  } 
  catch (...) 
  {
    MCE_WARN("[processBid] unknown exception when process interest, interest_content = " << interest_content);
  }
  return false;
}

