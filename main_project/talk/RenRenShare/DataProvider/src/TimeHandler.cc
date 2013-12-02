#include "TimeHandler.h"
#include <fstream>
#include <sstream>

using namespace xce::dataprovider;
using namespace std;

string TimeHandler::GetCurrentTime() {
  struct tm *tm = CurrentTime();

  ostringstream ss;
  ss << tm->tm_year+1900 << "-" << tm->tm_mon+1 << "-" <<tm->tm_mday<<" "
    << tm->tm_hour << ":"<< tm->tm_min << ":"<< tm->tm_sec;
  return ss.str();
}

int TimeHandler::CompareCurrentTime(const string &time_string) {
  struct tm *tm = CurrentTime();
  tm->tm_year += 1900;
  tm->tm_mon += 1;
  struct tm t;
  strptime(time_string.c_str(),"%Y%m%d%H%M%S",&t);
  t.tm_year += 1900;
  t.tm_mon += 1;
  return CompareTime(t, *tm);
}

struct tm* TimeHandler::CurrentTime() {
  time_t t = time(NULL);
  return localtime(&t);
}

int TimeHandler::CompareTime(struct tm &new_time, struct tm &current_time) {
  int ret = 0;
  if (new_time.tm_year > current_time.tm_year)
    ret = 1;
  else if (new_time.tm_year < current_time.tm_year)
    ret = -1;
  else if (new_time.tm_mon > current_time.tm_mon)
    ret = 1;
  else if (new_time.tm_mon < current_time.tm_mon)
    ret = -1;
  else if (new_time.tm_mday > current_time.tm_mday)
    ret = 1;
  else if (new_time.tm_mday < current_time.tm_mday)
    ret = -1;
  else if (new_time.tm_hour > current_time.tm_hour)
    ret = 1;
  else if (new_time.tm_hour < current_time.tm_hour)
    ret = -1;
  else if (new_time.tm_min > current_time.tm_min)
    ret = 1;
  else if (new_time.tm_min < current_time.tm_min)
    ret = -1;
  else if (new_time.tm_sec > current_time.tm_sec)
    ret = 1;
  else if (new_time.tm_sec < current_time.tm_sec)
    ret = -1;
  else 
    ret = 0;
  return ret;
}
