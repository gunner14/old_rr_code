#ifndef _DATAPROVIDERINFO_H_
#define _DATAPROVIDERINFO_H_

#include <string>
#include <Ice/Ice.h>

struct HotShareInfo {
  Ice::Long id;
  Ice::Long share_id;
  size_t user_id;
  size_t owner_id;
  std::string title;
  std::string url;
  std::string summary;
  std::string creation_date;
  std::string release_date;
  std::string user_name;
  std::string thumb_url;
  std::string thumb_url1;
  std::string head_url; 
  std::string catagory;
};
 
struct PhotoInfo {
  std::string photoId;
  std::string headUrl;
  std::string largeUrl;
  std::string atTitle;
  std::string linkUrl;
};

struct JSONDataNode {
  std::string value1;
  std::string value2;
  std::string value3;
  std::string value4;
  std::string value5;
};

struct OnlineVideoCatagory {
  std::string catagory_name;
  std::string push_date;
  size_t new_push;
};

struct OnlineVideoProgram {
  std::string program_name;
  std::string catagory_name;
  std::string catagory_id;
  std::string push_date;
  size_t new_push;
};

struct OnlineVideoProgramDBInfo {
  std::string program_name;
  std::string thumb_url;
  std::string page_url;
  std::string swf_url;
  std::string catagory;
  std::string release_date;
  std::string creation_date;
};
#endif
