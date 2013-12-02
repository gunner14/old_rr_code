#ifndef _AD_LOG_ICE_
#define _AD_LOG_ICE_

#include <Util.ice>

module xce{
module ad{

class EdmLogManager{
  void Expose(string log);
  void Click(string log);
  void Send(string log);
  
  int UpdateOnceCount(int count);
  
  
};
};
};
#endif
