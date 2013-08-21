#ifndef _CWF_ACTION_FACE_STORE_H_
#define _CWF_ACTION_FACE_STORE_H_

#include <vector>
#include <string>
#include <map>
#include "cwf/action.h"
//#include "tair_client_api.hpp"

namespace upload {

using namespace cwf;
//一张照片的多个人脸
#ifndef _rect_
#define _rect_
struct rect {
  int x0, y0, x1, y1;
};
#endif

typedef struct tag_face_attribute {
  std::string file_path;
  std::vector<rect> vec_rect;
  std::vector<float*> vec_buffer;
  std::vector<int> predict_id;  //默认为0 
  std::vector<int> cluster_result;  //默认为0，代表聚类失败 
} face_attribute;

class StoreFaceAction : public BaseAction {
 public:
  StoreFaceAction();
  void Stop();
 private:
  bool Match(const std::string& url) const;
  HttpStatusCode Process(Request*, Response*);
  bool ReadTairConfig(const std::string& strFile);
  bool ParseParam(const std::string& body_str);
  bool GetFaceFeatureByURL(const std::string& url, face_attribute& face_attr);
  bool ParseFaceAttribute(const char* value, int length, face_attribute& fa,
                          int& length_feature_buffer);
  bool DoStoreFaceFeatures();
  const static std::string kMethods_[];
//tair::tair_client_api *api_;
  std::map<std::string, std::map<int, int> > url_to_face_map_;
  std::string master_addr_;
  std::string slave_addr_;
  std::string group_name_;
  std::string cached_value_;
  int face_feature_buf_length_;
};

}  // namespace 
#endif 
