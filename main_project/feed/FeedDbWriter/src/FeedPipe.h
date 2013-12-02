#ifndef FEED_PIPE_H_
#define FEED_PIPE_H_

#include <Ice/Ice.h>
#include <IceUtil/AbstractMutex.h>
#include <IceUtil/Mutex.h>
//#include "ServiceI.h"
#include "TaskManager.h"
//#include "Singleton.h"
#include "Pipe.h"


namespace xce {
namespace feed{

using namespace MyUtil;

class RemovePipe : public Pipe {
public:
  RemovePipe(const string& name, int index) :
    Pipe(name, index) {
  }
  ;
  virtual void handle(const ObjectSeq& seq);

};


class PipeFactoryI : public PipeFactory {
public:
  virtual PipePtr create(const string& type, int index) {

   // if (type == "FeedPipe") {
      return new RemovePipe(type,index);
    //} else {
    //}   
  }
};

}
;
}
;

#endif /*FEED_PIPE_H*/




