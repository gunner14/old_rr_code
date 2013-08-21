#ifndef _CWF_ACTION_CLUSTER_H_
#define _CWF_ACTION_CLUSTER_H_

#include <vector>
#include "cwf/action.h"
#include "memcache.h"
#include "faceclustering/faceclusteringimpl.h"
#include <string>
#include <map>
#include <iostream>
//#include "tair_client_api.hpp"
class FaceDetector;
class CFaceFeature;
class FaceClustering;

namespace upload {

using namespace cwf;

//一张照片的多个人脸
typedef struct tag_face_attribute {
  std::string file_path;
  std::vector<rect> vec_rect;
  std::vector<float*> vec_buffer;
  std::vector<int> predict_id;  //默认为0 
  std::vector<int> cluster_result;  //默认为0，代表聚类失败 
} face_attribute;

typedef struct tag_user_features {
  int hostid;
  std::vector<float*> buffer_vec;
} user_features;

class ClusterAction : public BaseAction {
 public:
  ClusterAction();
  void Stop();
 private:
  bool Match(const std::string& url) const;
  HttpStatusCode Process(Request*, Response*);
 private:
  map<string, string> mapKey2Value_;
  const static std::string kMethods_[];
  FaceDetector* detector_;
  CFaceFeature* descriptor_;
  FaceClustering* facecluster_;
  //tair::tair_client_api *api_;
  std::string master_addr_;
  std::string slave_addr_;
  std::string group_name_;
  //tair::common::tair_keyvalue_map feature_data_;
  bool GetURLs(const std::string& body_str, std::vector<std::string>& urls);
  bool ReadTairConfig(const std::string& strFile);
  bool GetFaceFeaturesByFriends(int hostid,
                                std::vector<user_features>& feature_buffers);
  bool GetFaceFeaturesByURLs(const std::vector<std::string>& urls,
                             std::vector<face_attribute>& fa_vec);
  std::string ToJson(const std::vector<face_attribute>& fa_vec);
  bool ParseFaceAttribute(const char* value, int length, face_attribute& fa);
  int BelongsToWho(const std::vector<user_features>& user_features_vec,
                   int index);
};

}  // namespace 
#endif 
