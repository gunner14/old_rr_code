#include "action_cluster.h"
#include "base3/logging.h"
#include "base3/ptime.h"
#include "base3/escape.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include "highgui.h"
#include <cv.h>  
#include <fstream>
#include "facedetection/facedetectorimpl.h"
#include "facefeature/facefeatureimpl.h"
#include "faceclustering/faceclusteringimpl.h"
#include <boost/tokenizer.hpp>
#include "arch_diff/site_xiaonei.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace cv;

namespace upload {

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

const std::string ClusterAction::kMethods_[] = { "/cluster" };

inline char* itoa(char* strnum, int numb) {
  sprintf(strnum, "%d", numb);
  return strnum;
}

std::string Output(const std::string& json) {
  return "Status: 200\r\nContent-Type: text/html; charset=utf-8\r\n\r\n" + json;
}

bool ClusterAction::GetFaceFeaturesByFriends(
    int hostid, std::vector<user_features>& feature_buffers) {
#if 0
  std::vector<int> friend_list;
  {
    PTIME(pt1, "GetFriendList", true, 5);
    bool b = xce::GetFriendList(hostid,10,friend_list);
  }
  //friend_list.push_back(191000);

  static const int area = 0;
  char key_count_str[256];
  char key_feature[256];
  int ret;
  for(size_t i=0;i<friend_list.size();++i)
  {
    //construct count key
    sprintf(key_count_str,"%d_count",friend_list[i]);
    //get count
    const tair::common::data_entry key(key_count_str);
    int n_exist_count = -1;
    int ret = api_->incr(area,
        key,
        0,
        &n_exist_count,
        0,
        0);
    LOG(INFO) << "tair incr ret "<<ret;
    vector<tair::common::data_entry *> keys;
    for(int j=0;j<n_exist_count;j++)
    {
      sprintf(key_feature,"%d_%d",friend_list[i],j);
      tair::common::data_entry* data_ptr = new tair::common::data_entry(key_feature);
      if(data_ptr)
      keys.push_back(data_ptr);
    }
    if(n_exist_count > 0)
    {
      tair::common::defree(feature_data_);

      //multi-get features
      ret = api_->mget(area, keys, feature_data_);

      LOG(INFO) << "tair mget ret "<<ret;
      //parse data 
      if(ret == 0 || TAIR_RETURN_PARTIAL_SUCCESS == ret)
      {
        std::vector<float*> buffers;
        tair::common::tair_keyvalue_map::iterator itr = feature_data_.begin();
        for(;itr!=feature_data_.end();++itr)
        {
          buffers.push_back((float*)itr->second->get_data());
        }
        user_features uf;
        uf.hostid = friend_list[i];
        uf.buffer_vec = buffers;
        feature_buffers.push_back(uf);
      }

      for(size_t k=0;k<keys.size();k++)
      delete keys[i];
    }
  }

  return feature_buffers.size()>0;
#endif
}
bool ClusterAction::ParseFaceAttribute(const char* value, int length,
                                       face_attribute& fa) {
  const unsigned int guard_start = 0xFFFFEEEE;
  const unsigned int guard_end = 0xEEEEFFFF;

  int offset = 0;
  unsigned int start = 0;
  unsigned int end = 0;

  memcpy(&start, value, 4);
  offset += 4;
  if (start != guard_start)
    return false;

  memcpy(&end, value + length - 4, 4);
  if (end != guard_end)
    return false;

  int path_type = 0;
  int path_length = 0;
  memcpy(&path_type, value + offset, 4);
  offset += 4;
  if (path_type != 1)
    return false;

  memcpy(&path_length, value + offset, 4);
  offset += 4;
  fa.file_path = std::string(value + offset, path_length);
  offset += path_length;

  bool is_end = false;
  while (!is_end) {
//id
    int type = 0;
    memcpy(&type, value + offset, 4);
    if (type == 0) {
      break;
    }
    offset += 4;
    int length_cjl = 0;
    memcpy(&length_cjl, value + offset, 4);
    offset += 4;

    int id_value = 0;
    memcpy(&id_value, value + offset, 4);
    offset += 4;
//rect
    rect rect_value = { 0 };
    int type_rect = 0;
    int length_rect = 0;
    memcpy(&type_rect, value + offset, 4);
    offset += 4;

    memcpy(&length_rect, value + offset, 4);
    offset += 4;

    memcpy(&rect_value, value + offset, sizeof(rect));
    fa.vec_rect.push_back(rect_value);
    offset += sizeof(rect);

//feature buffer
    int type_feature_buffer = 0;
    int length_feature_buffer = 0;
    memcpy(&type_feature_buffer, value + offset, 4);
    offset += 4;
    memcpy(&length_feature_buffer, value + offset, 4);
    offset += 4;
    fa.vec_buffer.push_back((float*) (value + offset));
    offset += length_feature_buffer;

    fa.predict_id.push_back(0);
    fa.cluster_result.push_back(0);
  }
  return true;
}

bool ClusterAction::GetURLs(const std::string& body_str,
                            std::vector<std::string>& urls) {
  if (body_str.empty())
    return false;
  boost::char_separator<char> sep("\r\n");
  tokenizer tokens(body_str, sep);
  int dis = std::distance(tokens.begin(), tokens.end());
  if (dis <= 0)
    return false;
  urls.clear();
  for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end();
      ++tok_iter) {
    if (boost::starts_with(*tok_iter, "url:")) {
      urls.push_back(tok_iter->substr(4));
    }
  }
  return true;
}

std::string ClusterAction::ToJson(const std::vector<face_attribute>& fa_vec) {
  char szbuf[32] = { 0 };
  std::string json = "[";
  for (size_t i = 0; i < fa_vec.size(); i++)  //how many images
      {
    int id = 1;
    json += "{\"url\": \"" + fa_vec[i].file_path + "\",";
    json += "\"faces\": [";
    for (size_t j = 0; j < fa_vec[i].vec_rect.size(); j++)  //how many faces in an image
        {
      json += "{";
      json += "\"id\": " + std::string(itoa(szbuf, id++)) + ",";
      //json += "\"rect\": \""
      json += "\"rect\": [" + std::string(itoa(szbuf, fa_vec[i].vec_rect[j].x0))
          + "," + std::string(itoa(szbuf, fa_vec[i].vec_rect[j].y0)) + ","
          + std::string(itoa(szbuf, fa_vec[i].vec_rect[j].x1)) + ","
          //+ std::string(itoa(szbuf, fa_vec[i].vec_rect[j].y1)) + "\",";
          + std::string(itoa(szbuf, fa_vec[i].vec_rect[j].y1)) + "],";
      json += "\"predict_id\": "
          + std::string(itoa(szbuf, fa_vec[i].predict_id[j])) + ",";
      json += "\"cluster_result\": "
          + std::string(itoa(szbuf, fa_vec[i].cluster_result[j])) + "}";
      if (j != fa_vec[i].vec_rect.size() - 1) {
        json += ",";
      } else {
        json += "]";
      }
    }
    //
    if (i != fa_vec.size() - 1) {
      json += "},";
    } else {
      json += "}";
    }
  }
  json += "]";
  return json;
}

bool ClusterAction::GetFaceFeaturesByURLs(const std::vector<std::string>& urls,
                                          std::vector<face_attribute>& fa_vec) {
  //得到之前缓存的这一批的人脸特征值
  //首先得到的是TLV，然后通过解析得到feature buffer
  //
  if (urls.empty())
    return false;
  mapKey2Value_.clear();
  for (size_t i = 0; i < urls.size(); i++) {
    mapKey2Value_.insert(make_pair(urls[i], ""));
  }
  int count = g_memcache.Gets(mapKey2Value_);
  if (count) {
    map<string, string>::iterator iter;
    for (iter = mapKey2Value_.begin(); iter != mapKey2Value_.end(); ++iter) {
      face_attribute fa;
      bool b = ParseFaceAttribute(iter->second.c_str(), iter->second.length(),
                                  fa);
      if (b)
        fa_vec.push_back(fa);
    }
  }
  /*
   for (size_t i = 0; i < urls.size(); i++) 
   {
   string cached_value = g_memcache.Get(urls[i]);
   face_attribute fa;
   bool b = ParseFaceAttribute(cached_value.c_str(), cached_value.length(),
   fa);
   if (b)
   fa_vec.push_back(fa);
   }
   */
  return true;
}

bool ClusterAction::ReadTairConfig(const std::string& strFile) {
  if (strFile.empty()) {
    std::cerr << "file not found" << strFile << std::endl;
    return false;
  }
  static time_t tLastModifyTime = 0;
  struct stat st;
  if (stat(strFile.c_str(), &st) != 0) {
    std::cerr << "file not found:" << strFile << std::endl;
    return false;
  }
  if (st.st_mtime <= tLastModifyTime) {
    return true;
  }

  tLastModifyTime = st.st_mtime;
  std::ifstream fin(strFile.c_str());
  if (!fin) {
    std::cerr << "open file fail:" << strFile << std::endl;
    return false;
  }

  std::string strLine;
  while (getline(fin, strLine)) {
    size_t pos = strLine.find_first_of('=', 0);
    if (pos != std::string::npos) {

      std::string key = strLine.substr(0, pos);
      std::string value = strLine.substr(pos + 1);
      if (key == "master_addr") {
        master_addr_ = value;
      } else if (key == "slave_addr") {
        slave_addr_ = value;
      } else if (key == "group_name") {
        group_name_ = value;
      } else {
        assert(0);
      }

    } else {
      std::cerr << "wrong font file format " << std::endl;
      return false;
    }
  }
  assert(!master_addr.empty() && !group_name_.empty());
  return true;
}
ClusterAction::ClusterAction() {
  detector_ = new FaceDetector;
  if (detector_) {
    detector_->init("data/front_15_cascade.xml");
  } else {
    //log error
  }

  descriptor_ = new CFaceFeature;
  if (descriptor_) {
    descriptor_->LoadFaceAsm("data/facelandmark");
  } else {

    //log error
  }

  facecluster_ = new FaceClustering;
  if (facecluster_) {
  } else {
    //log error
  }
#if 0
  bool b = ReadTairConfig("./conf/tair.conf");
  assert(b);
  api_ = new tair::tair_client_api;
  assert(api_);
  assert(!master_addr_.empty());
  assert(!group_name_.empty());
  if(slave_addr_.empty())
  b = api_->startup(master_addr_.c_str(),NULL,group_name_.c_str());
  else
  b = api_->startup(master_addr_.c_str(),slave_addr_.c_str(),group_name_.c_str());
  assert(b);
#endif
}

bool ClusterAction::Match(const std::string& url) const {
  for (size_t i = 0; i < sizeof(kMethods_) / sizeof(kMethods_[0]); ++i) {
    if (boost::starts_with(url, kMethods_[i])) {
      return true;
    }
  }
  return false;
}

void ClusterAction::Stop() {
}

int ClusterAction::BelongsToWho(
    const std::vector<user_features>& user_features_vec, int index) {
  if (user_features_vec.size() == 0 || index < 0)
    return 0;
  if (index == 0)
    return user_features_vec[0].hostid;
  int sum = 0;
  for (size_t i = 0; i < user_features_vec.size(); i++) {
    sum += user_features_vec[i].buffer_vec.size();
    if (index < sum)
      return user_features_vec[i].hostid;
  }
  return 0;
}

int FindSame(const std::vector<int>& vec, int index) {
  if (index < 0 || index >= vec.size())
    return false;
  int value = vec[index];
  for (int i = 0; i < vec.size(); i++) {
    if (index == i)
      continue;
    if (value == vec[i]) {
      return i;
    }

  }
  return -1;

}

HttpStatusCode ClusterAction::Process(Request* request, Response* response) {
  std::vector < std::string > urls;  //通过请求获得
  std::vector<float*> upload_buffers;
  std::vector < user_features > user_features_vec;
  std::vector < face_attribute > fa;
  if (!request || request->file_array_.size() <= 0
      || request->file_array_[0].data.empty())
    return HC_BAD_REQUEST;

  bool b = GetURLs(request->file_array_[0].data, urls);
  if (!b) {
    LOG(INFO) << "GetURLs failed";
    return HC_BAD_REQUEST;
  }

  LOG(INFO) << "Get " << urls.size() << " URLs";
  GetFaceFeaturesByURLs(urls, fa);

  LOG(INFO) << "Get " << fa.size() << " facefeatures";
  //插入每张上传照片的N个人脸特征
  for (size_t i = 0; i < fa.size(); i++) {
    upload_buffers.insert(upload_buffers.begin() + upload_buffers.size(),
                          fa[i].vec_buffer.begin(), fa[i].vec_buffer.end());
  }

  //插入每个好友的N个人脸特征
  const int upload_buffer_count = upload_buffers.size();
  if (upload_buffer_count > 0) {

    std::string hostid = request->get("hostid");
    GetFaceFeaturesByFriends(atoi(hostid.c_str()), user_features_vec);
    for (std::vector<user_features>::iterator itVec = user_features_vec.begin();
        itVec != user_features_vec.end(); itVec++) {
      upload_buffers.insert(upload_buffers.begin() + upload_buffers.size(),
                            itVec->buffer_vec.begin(), itVec->buffer_vec.end());
    }

    bool has_friend_feature =
        ((upload_buffers.size() - upload_buffer_count) > 0);

    //cluster
    vector<int> cluster_num(upload_buffers.size(), -1);

    facecluster_->cluster(upload_buffers, cluster_num);
    //输出聚类和预测信息
    //每张照片中的N个人脸
    int cluster_index = -1;
    for (int i = 0; i < fa.size(); i++) {
      for (int j = 0; j < fa[i].vec_buffer.size(); j++) {
        cluster_index++;
        if (cluster_num[cluster_index] != 0) {
          //如果第i张照片中的第j个人脸成功聚类
          if (has_friend_feature) {
            //先在好友的聚类结果里面找
            vector<int>::iterator iter = find(
                cluster_num.begin() + upload_buffer_count, cluster_num.end(),
                cluster_num[cluster_index]);
            if (iter != cluster_num.end()) {
              int idx = std::distance(cluster_num.begin() + upload_buffer_count,
                                      iter);
              fa[i].predict_id[j] = BelongsToWho(user_features_vec, idx);
              fa[i].cluster_result[j] = cluster_num[cluster_index];

            } else {
              //好友的特征值都没有成功聚类,此时没有预测信息
              int pos = FindSame(cluster_num, cluster_index);
              if (pos != -1) {
                fa[i].cluster_result[j] = cluster_num[cluster_index];
              }
            }
          } else {

            fa[i].cluster_result[j] = cluster_num[cluster_index];
          }
        }
      }
    }
    //接下来，回写结果，
    std::string json = ToJson(fa);
    std::string strOutput = Output(json);
    response->WriteRaw(strOutput + '\n');
  } else {
    //log error
    std::vector < face_attribute > fa_empty;
    std::string json = ToJson(fa_empty);
    std::string strOutput = Output(json);
    response->WriteRaw(strOutput + '\n');
  }
  return HC_OK;
}
}

