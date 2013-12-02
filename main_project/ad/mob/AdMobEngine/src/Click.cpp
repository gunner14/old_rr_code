/*
 *  Click.cpp
 *
 *  Created on: 2012-03-27
 *      Author: yongjun.yin
 */

#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "Click.h"
#include "Util.h"
#include "AdMobLogBAdapter.h"
#include "AdMobLogSAdapter.h"

using namespace xce::notify;
using namespace xce::ad;
using namespace MyUtil;
using namespace boost;

bool OtherRequest::response() {
	//MCE_INFO("FCGI:Other request");
	string output = "Location: \r\n\r\n";
	FCGX_PutS(output.c_str(), _fcgi_out);
	return true;
}

bool ClickRequest::response() {
	//timeval start;
	//gettimeofday(&start, NULL);
	MCE_INFO("ClickRequest::response --> Got a request from 3G");
	if (!_fcgi_out) {
		MCE_WARN("fcgi_out null");
		return false;
	}
	//获取点击用户的uid
	string uid = getProperty("uid");
	if (!uid.empty()) {
		try {
			_login_uid = boost::lexical_cast<int>(uid);
		} catch (...) {
			MCE_WARN("ClickRequest::response --> lexical_cast cookie id error : " << uid);
		}
	}
	string groupid = getProperty("groupid");
	if ( true == groupid.empty() ) {
		MCE_WARN("ClickRequest::response --> cannot get groupid ");
	}
	//获取redirect url并进行302跳转
	string url = getProperty("clickurl");
	if(url.empty() == true){
		url = "http://mt.renren.com";
		MCE_WARN("ClickRequest::response --> get clickurl error");
	}
	MCE_INFO("click uid:" << _login_uid <<" groupid:" << groupid << " location:" << url);
	string output = "Location: " + url + "\r\n\r\n";
	FCGX_PutS(output.c_str(), _fcgi_out);

	string type_info = getProperty("grouptype");
	string displaytype = getProperty("displaytype");
	if ( false == type_info.empty() ) {
		int type = atoi(type_info.c_str());
		if ( BRAND_AD == type ) {
			//点击Log处理
			string clicklog = getProperty("clicklog");
			if ( false == clicklog.empty() && ProcessClickLog(clicklog)) {
				AdMobLogBAdapter::instance().Click( clicklog );
			}

			string transtype_info = getProperty("transtype");
			int transtype = atoi(transtype_info.c_str());
			if (CPC == transtype && false == displaytype.empty() && ROTATE3G != atoi(displaytype.c_str())) {
				//Cpc Log处理
				string cpclog = getProperty("cpcchargelog");
				bool result = ProcessCpcLog(cpclog);
				if ( true == result ) {
					AdMobLogBAdapter::instance().CpcCharge( cpclog );
				}
			}
		} else if ( SELF_AD == type ) {
			//点击Log处理
			string clicklog = getProperty("clicklog");
			if ( false == clicklog.empty() && ProcessClickLog(clicklog)) {
				//MCE_DEBUG("clicklog------->"<<clicklog);
				AdMobLogSAdapter::instance().Click( clicklog );
			}

			string transtype_info = getProperty("transtype");
			int transtype = atoi(transtype_info.c_str());
			if (CPC == transtype) {
				//Cpc Log处理
				string cpclog = getProperty("cpcchargelog");
				bool result = ProcessCpcLog(cpclog);
				if ( true == result ) {
					AdMobLogSAdapter::instance().CpcCharge( cpclog );
				}
			}
		} else {
			MCE_WARN("ClickRequest::response --> Unknown adgroup type:" << type);
			return true;
		}
	}

	return true;
}

inline bool ClickRequest::ProcessCpcLog(string& cpclog) {
	if ( false == cpclog.empty() ) {
		MyUtil::Date date(time(NULL));
		string clicktime = date.toDateTime();
		string timeval = date.toDateTime("%Y-%m-%d %H:%M:");
		try {
			size_t time_pos = cpclog.find("CLICKTIME");
			if ( time_pos == string::npos ) {
				return false;
			}
			cpclog.replace(time_pos, 9, clicktime);
			time_pos = cpclog.find("STARTTIME");
			if ( time_pos == string::npos ) {
				return false;
			}
			cpclog.replace(time_pos, 9, timeval+"00");
			time_pos = cpclog.find("ENDTIME");
			if ( time_pos == string::npos ) {
				return false;
			}
			cpclog.replace(time_pos, 9, timeval+"59");
		} catch (std::exception& e) {
			MCE_WARN("ClickRequest::ProcessCpcLog --> std exception: " << __LINE__ <<" "<< e.what());
			return false;
		} catch (...) {
			MCE_WARN("ClickRequest::ProcessCpcLog --> unknown exception");
			return false;
		}
		return true;
	} else {
		MCE_WARN("ClickRequest::ProcessCpcLog --> cpclog is Empty");
	}
	return false;
}

inline bool ClickRequest::ProcessClickLog(string& click_log) {
	if (click_log.empty()){
		MCE_WARN("ClickRequest::ProcessClickLog --> click_log is Empty");
		return false;
	}
  size_t date_pos = click_log.find("$DATE$");
	if (date_pos != string::npos) {
    MyUtil::Date date(time(NULL));
	  click_log.replace(date_pos, 6, date.toDateTime());
	  MCE_DEBUG("ClickRequest::ProcessClickLog --> $DATE$ changed by " << date.toDateTime());
		return true;
	}
	MCE_WARN("ClickRequest::ProcessClickLog --> click_log is False");
	return false;
}
