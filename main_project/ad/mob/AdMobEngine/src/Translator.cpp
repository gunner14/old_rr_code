#include "Translator.h"
#include <bitset>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "LogWrapper.h"
#include "Config.h"

using namespace std;
using namespace boost;
using namespace xce::ad;


bool Translator::ProcessGender(const int gender_code, vector<short>& genders) {
	try {
		if( gender_code == GENDER_FEMALE || gender_code == GENDER_MALE ) {
			genders.push_back((short)gender_code);
		} else if( gender_code == GENDER_ALL ) {
			genders.push_back( GENDER_FEMALE );
			genders.push_back( GENDER_MALE );
		} 
		return true;
	} catch (std::exception e) {
		MCE_WARN("[ProcessBid] exception when process gender: " << e.what() << ", gender_code = " << gender_code);
	} catch (...) {
		MCE_WARN("[ProcessBid] unknown exception when process gender, gender_code = " << gender_code);
	}
	return false;
}

bool Translator::ProcessStage(const int stage_code, vector<short>& stages) {
	try {
		if( stage_code & STAGE_SENIOR ) {
			stages.push_back( STAGE_SENIOR );
		}
		if( stage_code & STAGE_COLLEGE ) {
			stages.push_back( STAGE_COLLEGE );
		}
		if( stage_code & STAGE_STAFF ) {
			stages.push_back( STAGE_STAFF );
		}
		return true;
	} catch (std::exception e) {
		MCE_WARN("[ProcessBid] exception when process stage: " << e.what() << ", stage_code = " << stage_code);
	} catch (...) {
		MCE_WARN("[ProcessBid] unknown exception when process stage, stage_code = " << stage_code);
	}
	return false;
}


bool Translator::ProcessGrade(const string& grade_code, vector<short>& grades) {
	//41,41;43,45;51,51;53,53;61,68
	try {
		if(grade_code.empty()) {
			grades.push_back( ALL );
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
						begin = boost::lexical_cast<short>(*tok_iter2);
					} else if(num == 2) {
						end = boost::lexical_cast<short>(*tok_iter2);
					}
				}
				if(!end && begin > 0) {
					end = begin;
				}
				for(short i = begin; i <= end; ++i) {
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

bool Translator::ProcessAge(const string& age_code, vector<short>& ages) {
	try {
		if(age_code.empty()) {
			ages.push_back( ALL );
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
						begin = boost::lexical_cast<short>(*tok_iter2);
					} else if(num == 2) {
						end = boost::lexical_cast<short>(*tok_iter2);
					}
				}
				if(!end && begin > 0) {
					end = begin;
				}
				for(short i = begin; i <= end; ++i) {
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
			schools.push_back( ALL );
		} else {
			vector<string> result;
			boost::algorithm::split(result, school_code, is_any_of(";"));
			int num = 1;
			vector<uint64_t> temp;
			for(vector<string>::iterator it = result.begin(); it != result.end(); ++it, ++num) {
				if(num == 1 && !(*it).empty()) {
					if(ProcessLeadingArea(*it, areas)) {
						MCE_DEBUG("Translator::ProcessSchool --> LeadingArea:" << (*it));
					}
				} else if(num == 2 && !(*it).empty()) {
					if(ProcessRealSchool(*it, schools)) {
						MCE_DEBUG("Translator::ProcessSchool --> RealSchool:" << (*it));
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
	//area0086110000000000,area0086210000000000,area0086310000000000,area0086440000000000,area0086510000000000
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
	//400000001011,400000001003,400000001056,400000001020,400000001032,400000001036
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
	//0086110000000000,0086310000000000,0086440100000000,0086440300000000
	try {
		if(area_code.empty()) {
			areas.push_back( ALL );
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

bool Translator::ProcessPlatform(const string& platform_code, vector<uint64_t>& platforms) {
	try {
		if(platform_code.empty()) {
			platforms.push_back( ALL );
		} else {
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(",");
			tokenizer tokens(platform_code, sep);
			for(tokenizer::iterator tok_iter = tokens.begin();
					tok_iter != tokens.end(); ++tok_iter) {
				uint64_t platform = boost::lexical_cast<uint64_t>(*tok_iter);
				platforms.push_back(platform);
			}
		}
		return true;
	} catch (std::exception e) {
		MCE_WARN("[ProcessBid] exception when process platform: " << e.what() << ", platform_code = " << platform_code);
	} catch (...) {
		MCE_WARN("[ProcessBid] unknown exception when process platform, platform_code = " << platform_code);
	}
	return false;
}

bool Translator::ProcessNetwork(const string& network_code, vector<short>& networks) {
	try {
		if(network_code.empty()) {
			networks.push_back( ALL );
		} else {
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(",");
			tokenizer tokens(network_code, sep);
			for(tokenizer::iterator tok_iter = tokens.begin();
					tok_iter != tokens.end(); ++tok_iter) {
				short network = boost::lexical_cast<short>(*tok_iter);
				networks.push_back(network);
			}
		}
		return true;
	} catch (std::exception e) {
		MCE_WARN("[ProcessBid] exception when process network: " << e.what() << ", network_code = " << network_code);
	} catch (...) {
		MCE_WARN("[ProcessBid] unknown exception when process network, network_code = " << network_code);
	}
	return false;
}

bool Translator::ProcessBrand3G(const string& brand_code, vector<uint64_t>& brands) {
	try {
		if(brand_code.empty()) {
			brands.push_back( ALL );
		} else {
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(",");
			tokenizer tokens(brand_code, sep);
			for(tokenizer::iterator tok_iter = tokens.begin();
					tok_iter != tokens.end(); ++tok_iter) {
				uint64_t brand = boost::lexical_cast<uint64_t>(*tok_iter);
				brands.push_back(brand);
			}
		}
		return true;
	} catch (std::exception e) {
		MCE_WARN("[ProcessBid] exception when process brand: " << e.what() << ", brand_code = " << brand_code);
	} catch (...) {
		MCE_WARN("[ProcessBid] unknown exception when process brand, brand_code = " << brand_code);
	}
	return false;
}

bool Translator::ProcessResolution(const string& resolution_code, vector<short>& resolutions) {
	try {
		if(resolution_code.empty()) {
			resolutions.push_back( ALL );
		} else {
			vector<string> fields;
			boost::algorithm::split_regex( fields, resolution_code, boost::regex("[\\*|X]"));
			vector<string>::iterator it = fields.begin();
			short resolution = atoi( it->c_str() );
			set<short> width_set = ResolutionMap::instance().GetCeilWidths(resolution);
			for(set<short>::iterator it = width_set.begin();it!=width_set.end();++it) {
				MCE_DEBUG("Translator::ProcessResolution --> resolution index add key:"<<(*it));
				resolutions.push_back(*it);
			}
		}
		return true;
	} catch (std::exception e) {
		MCE_WARN("[ProcessBid] exception when process resolution: " << e.what() << ", resolution_code = " << resolution_code);
	} catch (...) {
		MCE_WARN("[ProcessBid] unknown exception when process resolution, resolution_code = " << resolution_code);
	}
	return false;
}

bool Translator::ProcessLBS(const string& lbs_code, const double radius, vector<Rectangle>& lbs_list) {
	try {
		if( lbs_code.empty() || -1 == atoi(lbs_code.c_str()) ) {
			//return false;
			lbs_list.push_back( LBS_ALL );
		} else {
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(";");
			tokenizer tokens(lbs_code, sep);
			for(tokenizer::iterator tok_iter = tokens.begin();
					tok_iter != tokens.end(); ++tok_iter) {
				boost::char_separator<char> sep_sub(",");
				string sub_str = boost::lexical_cast<string>(*tok_iter);
				tokenizer tokens_sub(sub_str, sep_sub);

				double longitude = -1;
				double latitude = -1;
				int num = 1;
				for(tokenizer::iterator tokens_sub_iter = tokens_sub.begin();
						tokens_sub_iter != tokens_sub.end(); ++tokens_sub_iter,++num) {
					if ( 1 == num ) {
						longitude = boost::lexical_cast<double>(*tokens_sub_iter);
					} else if ( 2 == num ) {
						latitude = boost::lexical_cast<double>(*tokens_sub_iter);
						num = 0;
					} else {
						break;
					}
				}
				if( longitude != -1 && latitude != -1 ) {
					Rectangle rec(longitude, latitude, radius);
					lbs_list.push_back(rec);
				}
			}
		}
		return true;
	} catch (std::exception e) {
		MCE_WARN("[ProcessBid] exception when process lbs: " << e.what() << ", lbs_code = " << lbs_code);
	} catch (...) {
		MCE_WARN("[ProcessBid] unknown exception when process lbs, lbs_code = " << lbs_code);
	}
	return false;
}
