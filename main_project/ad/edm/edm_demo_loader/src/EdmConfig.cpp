/*
 * AdConfig.cpp
 *
 *  Created on: 2012-09-22
 *      Author: yu.fu 
 */
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include "IceLogger.h"
#include <netdb.h>
#include <arpa/inet.h>
#include "EdmConfig.h"

namespace xce {
namespace ad {

bool EdmDemoConfig::LoadConfig() {
	string file_name = "../etc/edm_demoloader_quality.config";
	try {
		ifstream config(file_name.c_str());
		string line;

		if (config) {
			while (config >> line) {
				typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
				boost::char_separator<char> sep("=");
				tokenizer tokens(line, sep);
				string key;
				string value;
				int n = 0;
				for(tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it, ++n) {
					if (0==n) {
						key = *it;
					} else if(1==n) {
						value = *it;
					}
				}
				if (2 == n) {
					edm_pv_click_th_[key] = boost::lexical_cast<int>(value);
					MCE_INFO("[Load_Config] key->value:"<< key <<" --> "<< value);
				}
			}
		} else {
			MCE_INFO("no file " << file_name);
		}
		if (config) {
			config.close();
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("EngineConfig::LoadEdmConfig error : " << e);
	} catch (...) {
		MCE_WARN("EngineConfig::LoadEdmConfig error : unkown error \"...\"");
	}
	return true;
}

}
}
