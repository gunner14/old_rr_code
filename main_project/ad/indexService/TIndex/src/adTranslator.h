#ifndef ADTRANSLATOR_H
#define ADTRANSLATOR_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
//#include "FeedMemcProxy/client/user_profile_client.h"
#include "Singleton.h"
#include "ServiceI.h"

namespace xce{
	namespace ad{

		class Translator : public MyUtil::Singleton<Translator> {
			public:

				static bool processGender(const int gender_code, std::vector<short>& genders);
				static bool processStage(const int stage_code, std::vector<short>& stages);
				static bool processGrade(const std::string& grade_code, std::vector<short>& grades);
				static bool processAge(const std::string& age_code, std::vector<short>& ages);
				static bool processSchool(const std::string& school_code, 
						std::vector<uint64_t>& areas, std::vector<uint64_t>& schools);
				static bool processArea(const std::string& area_code, std::vector<uint64_t>& areas);
        static bool processCompany(const std::string& company_content, std::vector<uint64_t>& companys);
        static bool processInterest(const std::string& interest_content, std::vector<uint64_t>& interests);

			private:
				static bool processLeadingArea(const std::string& area_code, std::vector<uint64_t>& areas);
				static bool processRealSchool(const std::string& school_code, std::vector<uint64_t>& schools);

		};
	}
}

#endif // ADTRANSLATOR_H
