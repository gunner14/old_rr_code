#ifndef _ALERTCLIENT_H_
#define _ALERTCLIENT_H_

#include <boost/asio.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <LogWrapper.h>
#include <curl/curl.h>

namespace MyUtil {

class AlertClient {

public:

  static void alertSMS(const std::string& number, const std::string& message) {
    MCE_WARN("AlertClient::alertSMS number: " << number << " message: " << message);
    try {
      CURL *handle = curl_easy_init();
      char *curl =curl_easy_escape(handle, message.c_str(), message.length());
      string newmessage(curl);
      curl_free(curl);
      curl_easy_cleanup(handle);
      boost::asio::ip::tcp::iostream stream("sms.notify.d.xiaonei.com","2000");
      if (stream) {
        stream << "GET /receiver?number=" << number << "&message=" << newmessage << " HTTP/1.1\nConnection:close\n\n" << std::flush;
      }
    } catch (const std::exception& e) {
      MCE_WARN("AlertClient::alertSMS std::exception: " << e.what());
    } catch (...) {
      MCE_WARN("AlertClient::alertSMS unknown exception");
    }
  }

  static void alertEmail(const std::string& email, const std::string& title, const std::string& content) {
    MCE_WARN("AlertClient::alertEmail email: " << email << " title: " <<  title << " content: " << content);
    try {
	const string tmp = " ";
  	const string send_cmd = (string)"cd /data/xce/Monitor/bin; sh /data/xce/Monitor/bin/sendmail.sh"
	        + " " + "\"" + title + "\""
		+ " " + "\"" + email + "\""
		+ " " + "\"" + content + "\"" 
		+ "\n";
	system(send_cmd.c_str());
    } catch (const std::exception& e) {
      MCE_WARN("AlertClient::alertEmail std::exception: " << e.what());
    } catch (...) {
      MCE_WARN("AlertClient::alertEmail unknown exception");
    }
  }

  //send SMS to jira also.(guanghe.ge 2011-3-31)
  static void alertJira(const std::string& author, const std::string& message) {
    try {
		ostringstream oss;
		oss << "wget -q -O /dev/null --load-cookies cookies.txt --post-data=\'assignee=" << author << "&id=" << message << "\' http://tracker.d.xiaonei.com/jira/secure/AssignIssue.jspa";
		system(oss.str().c_str());
    	MCE_WARN("AlertClient::alertJira author: " << author << " message: " << message);
    } catch (const std::exception& e) {
      MCE_WARN("AlertClient::alertJira std::exception: " << e.what());
    } catch (...) {
      MCE_WARN("AlertClient::alertJira unknown exception");
    }
  }

private:
  static string timestamp() {
    char result[100];
    time_t t= time(0);
    struct tm *newtime = localtime(&t);
    strftime(result, 100, "%d %b %g %T", newtime);
    return string(result);
  }

};

}

#endif
