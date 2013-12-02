#ifndef PowerTransfer_NotifyTransfer_h_
#define PowerTransfer_NotifyTransfer_h_

#include "Common.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace function{
namespace transfer{
class deliverNotifyClientTask: public MyUtil::Task{
public:
  deliverNotifyClientTask(const MyUtil::IntSeq& toids, const string& content):Task(11), 
                          _toids(toids),_content(content) {};  
  virtual void handle();
private:
  MyUtil::IntSeq _toids;
  string _content;
};

class deliverNotifyWebpagerTask: public MyUtil::Task{
public:
  deliverNotifyWebpagerTask(const MyUtil::IntSeq& toids, const string& content):Task(13), 
                              _toids(toids),_content(content){};  
  virtual void handle();
private:
  MyUtil::IntSeq _toids;
  string _content;
};

class deliverNotifyPhoneTask: public MyUtil::Task{
public:
  deliverNotifyPhoneTask(const MyUtil::IntSeq& toids, const string& content):Task(15), 
                         _toids(toids),_content(content){};  
  virtual void handle();
private:
  MyUtil::IntSeq _toids;
  string _content;
};


};
};
};
};
};

#endif
