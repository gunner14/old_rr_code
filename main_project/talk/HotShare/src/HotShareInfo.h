#ifndef _HOTSHAREINFO_H_
#define _HOTSHAREINFO_H_

#include <string>
#include <Ice/Ice.h>

struct HotShareInfo {
  Ice::Long id;
  Ice::Long local_id;
  Ice::Long share_id;
  Ice::Long type;
  size_t user_id;
  std::string title;
  std::string url;
  std::string summary;
  std::string creation_date;
  std::string user_name;
  std::string thumb_url;
  std::string main_url;
  std::string head_url;
};
 

#endif
