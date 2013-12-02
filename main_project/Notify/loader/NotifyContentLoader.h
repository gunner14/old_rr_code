#ifndef _NOTIFY_CONTENT_LOADER_H_
#define _NOTIFY_CONTENT_LOADER_H_

#include <vector>
#include "Notify.h"

namespace xce {
namespace notify { 

class NotifyContentLoader {
 public: 
  static NotifyContentDict GetNotifyContentDict(const std::vector<int64_t>& nids_seq);
  static bool AddNotifyContent(const NotifyContentPtr& notify_cont_ptr, 
      bool compress_flag);
  static bool RemoveNotifyContents(const std::vector<int64_t>& nids_seq);
};

class ContentPrintHelper{
 public:
  template<typename T>
  static string Print(const T & container,int limit = 100) {
    ostringstream ios;
    ios << "(";
    bool first_item = true;
    int count = 0;
    for (typename T::const_iterator it = container.begin(); 
        it != container.end(); ++it, ++count) {
      if (first_item == true) {
        first_item = false;
      } else {
        ios << ",";
      }
      ios << *it;
      if(count >= limit){
        break;
      }
    }
    ios << ")";
    return ios.str();
  }
};

}
}

#endif //_NOTIFY_CONTENT_LOADER_H_

