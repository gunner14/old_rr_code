#ifndef _MP_MESSAGEDATA_H__
#define _MP_MESSAGEDATA_H__

#include "Common.h"

namespace xce {
namespace messagepipe {

class MessageData {

private:
	std::string cluster_;
	std::string subject_;
	bool is_topic_;
	std::vector<unsigned char> data_;
public: 
	MessageData(const std::string& cluster, const std::string& subject,
		const std::vector<unsigned char>& data, bool is_topic=false) 
		: cluster_(cluster),subject_(subject),is_topic_(is_topic),data_(data) {
	}
	

	std::string getCluster() {
		return cluster_;
	}
  
  void setCluster(const std::string& cluster) {
    cluster_ = cluster;
  }

	bool isTopic(){
		return is_topic_;
	}
	std::string getSubject() {
		return subject_;
	}

	std::vector<unsigned char>& getData() {
		return data_;
	}

};

}
}

#endif
