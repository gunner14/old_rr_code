#ifndef _TIMEHANDLER_H_
#define _TIMEHANDLER_H_
#include <string>
#include <time.h>
namespace xce {
namespace dataprovider {

class TimeHandler {
  public:
    static std::string GetCurrentTime();
    static int CompareCurrentTime(const std::string &time_string);

  private:
    static struct tm* CurrentTime();
    static int CompareTime(struct tm &new_time, struct tm &current_time);
}; 

}
}
#endif
