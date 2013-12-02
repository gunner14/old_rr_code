#include "PingConfig.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <algorithm>

using namespace xce::ad;
using namespace std;
using namespace boost;

bool PingConfig::LoadConfig(){  
  string file_name = "../etc/ad_ping_receiver.config";
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
        if (!key.empty()) {
          MCE_INFO("PingConfig::LoadConfig Get config key: " << key
              << ", value: " << value);
        }
        if (2 == n) {
          if (key.compare("before_minute") == 0) {
            before_minute_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] before_minute = " << before_minute_);
          } else if (key.compare("statistic_minute") == 0) {
            statistic_minute_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] statistic_minute = " << statistic_minute_);
          } else if (key.compare("send_msg_threshold") == 0) {
            send_msg_threshold_ = boost::lexical_cast<int>(value);
            MCE_INFO("[LOAD_CONFIG] send_msg_threshold = " << send_msg_threshold_);
          } else if (key.compare("from") == 0) {
            from_ = value;
            JwsmptEmail::instance().SetFrom(from_);
            MCE_INFO("[LOAD_CONFIG] from = " << from_);
          } else if (key.compare("username") == 0) {
            username_ = value;
            JwsmptEmail::instance().SetUsername(username_);
            MCE_INFO("[LOAD_CONFIG] username = " << username_);
          } else if (key.compare("pwd") == 0) {
            pwd_ = value;
            JwsmptEmail::instance().SetPwd(pwd_);
            MCE_INFO("[LOAD_CONFIG] pwd = " << pwd_);
          } else if (key.compare("smpt_server") == 0) {
            smpt_server_ = value;
            JwsmptEmail::instance().SetSmptServer(smpt_server_);
            MCE_INFO("[LOAD_CONFIG] smpt_server = " << smpt_server_);
          } else if (key.compare("subject") == 0) {
            subject_ = value;
            JwsmptEmail::instance().SetSubject(subject_);
            MCE_INFO("[LOAD_CONFIG] subject = " << subject_);
          } else if (key.compare("email_receivers") == 0) {
            vector<string> receivers;
            split(receivers, value, is_any_of(","));
            email_receivers_ = receivers;
            JwsmptEmail::instance().SetReceivers(receivers);
            MCE_INFO("[LOAD_CONFIG] receivers = " << value);
          } else if (key.compare("phones") == 0) {
            vector<string> phones;
            split(phones, value, is_any_of(","));
            phones_ = phones;
            MCE_INFO("[LOAD_CONFIG] phones = " << value);
          }
        }
      }
    } else {
      MCE_INFO("no file " << file_name);
    }
    if (config) {
      config.close();
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("PingConfig::LoadConfig error : " << e);
  } catch (...) {
    MCE_WARN("PingConfig::LoadConfig error : unkown error \"...\"");
  }
  return true;
}

