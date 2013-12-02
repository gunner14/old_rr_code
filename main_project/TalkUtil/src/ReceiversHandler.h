#ifndef RECEIVERS_HANDLER_
#define RECEIVERS_HANDLER_

#include <vector>
#include <string>
#include <sstream>
namespace com{
namespace xiaonei {
namespace talk {
namespace util {
class ReceiversHandler {
public:
  static std::string ReceiversAssembly(const std::vector<int> &receivers) {
    std::ostringstream os;
    for (std::vector<int>::const_iterator iter=receivers.begin(); iter!=receivers.end(); iter++) {
      os << *iter <<" ";
    }
    return os.str();
  }
  static std::string ReceiversAssembly(const std::vector<std::string> &receivers) {
    std::ostringstream os;
    for (std::vector<std::string>::const_iterator iter=receivers.begin(); iter!=receivers.end(); iter++) {
      os << *iter <<" ";
    }
    return os.str();
  }
  static std::vector<int> ReceiversSplit(const std::string &receivers) {
    size_t head = 0;
    size_t rear = 0;
    size_t uid = 0;
    std::vector<int> temp;
    while (1) {
      if ((rear=receivers.find(' ', head)) != std::string::npos) {
        uid = atoi(receivers.substr(head, rear-head).c_str());
        if (uid > 0)
          temp.push_back(uid);
        head = rear+1;
      } else{
	if(head == 0)
		temp.push_back(atoi(receivers.c_str()));//fix only one string problem
        break;
	}
    }
    return temp;
  }
  static void ReceiversSplit(const std::string &receivers, std::vector<string>& rece_queue) {
    size_t head = 0;
    size_t rear = 0;
    while (1) {
      if ((rear=receivers.find(' ', head)) != std::string::npos) {
        rece_queue.push_back(receivers.substr(head, rear-head));
        head = rear+1;
      } else{
	      if(head == 0)
		      rece_queue.push_back(receivers);//fix only one string problem
        break;
	    }
    }
  }
};


}
}
}
} 

#endif

