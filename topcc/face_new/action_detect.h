#ifndef _CWF_ACTION_DETECT_H_
#define _CWF_ACTION_DETECT_H_

#include <vector>
#include <string>
#include <map>
#include "cwf/action.h"
#include "memcache.h"

#include "faceclustering/faceclusteringimpl.h"
class FaceDetector;
class CFaceFeature;
class FaceClustering;

namespace upload {

//一张照片的多个人脸
typedef struct tag_face_attribute {
  std::string file_path;
  std::vector<rect> vec_rect;
  std::vector<float*> vec_buffer;
  std::vector<int> predict_id;  //默认为0 
  std::vector<int> cluster_result;  //默认为0，代表聚类失败 
} face_attribute;

using namespace cwf;

class DetectAction : public BaseAction {
 public:
  DetectAction();
  void Stop();
 private:
  bool Match(const std::string& url) const;
  HttpStatusCode Process(Request*, Response*);
  bool Init(const std::string& strFile);
  map<string, string> mapKey2Value_;
 private:
  bool WriteFaceAttribute(const std::string& file_path,
                          const std::vector<rect>& vecRect,
                          const std::vector<float*>& vecbuffer);
  bool ParseImagePath(const std::string& image_path, std::string& image_url,
                      std::string& hostid);
  const static std::string kMethods_[];
  FaceDetector* detector_;
  CFaceFeature* descriptor_;
  FaceClustering* facecluster_;
  char* pvbuffer_;
  std::string str_buffer_;
  int cache_expiration_;
  int friend_count_limit_;
  int friend_buffer_limit_;
};

}  // namespace 
#endif 
