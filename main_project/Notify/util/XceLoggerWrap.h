#ifndef _XCE_LOGGER_SENDER_H_
#define _XCE_LOGGER_SENDER_H_

#include "UtilCxx/src/Singleton.h"
#include "OceCxxAdapter/src/XceLoggerAdapter.h"

namespace xce {
namespace notify {

class XceLogSender : public Singleton<XceLogSender> {
 public:
  bool Init(const vector<string>& categories) {
    categories_ = categories;
    for (size_t i = 0; i < categories_.size(); ++i) {
      categories_str += categories_[i] + "/";
    }
    try{
      using namespace com::xiaonei::service;
      xce_logger_ = XceLoggerAdapter::instance().getLogger(1, 32768);
      xce_logger_->initialize();
      MCE_INFO("XceLogSender::Init --> ok category:" << categories_str);
      return true;
    }catch(Ice::Exception& e) {
      MCE_WARN("XceLogSender::Init --> category:" << categories_str 
          << " exception: " << e);
      return false;
    }
  }
  void Send(long nid, const string& str) {
    if(xce_logger_) {
      xce_logger_->addLog(categories_, str);
      MCE_DEBUG("XceLogSender::Send --> category:" << categories_str 
          << " str:" << str)
    } else {
      MCE_WARN("XceLogSender::Send --> not init nid:" << nid 
          << " logger:" << xce_logger_ << " category:" << categories_str);
		}
	}
	void SendLog(const string& str) {
		if(xce_logger_) {
			xce_logger_->addLog(categories_, str);
			MCE_DEBUG("XceLogSender::Send --> category:" << categories_str
					<< " str:" << str)
		} else {
			MCE_WARN("XceLogSender::Send --> not init :" 
					<< " logger:" << xce_logger_ << " category:" << categories_str);
		}
	}

  string GetCategories() {
    return categories_str;
  }
 private:
  vector<string> categories_;
  com::xiaonei::service::XceLoggerPtr xce_logger_;
  string categories_str;
};

}
}

#endif //_XCE_LOGGER_SENDER_H_

