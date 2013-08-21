#include "action_face_store.h"
#include "base3/logging.h"
#include "base3/ptime.h"
#include "base3/escape.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <boost/tokenizer.hpp>
//#include "tair_client_api.hpp"
#include "memcache.h"

namespace upload {

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
const std::string StoreFaceAction::kMethods_[] = { "/storeface" };

StoreFaceAction::StoreFaceAction() {
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

bool StoreFaceAction::Match(const std::string& url) const {
  for (size_t i = 0; i < sizeof(kMethods_) / sizeof(kMethods_[0]); ++i) {
    if (boost::starts_with(url, kMethods_[i])) {
      return true;
    }
  }
  return false;
}

void StoreFaceAction::Stop() {
  //delete api_;
}

bool StoreFaceAction::ReadTairConfig(const std::string& strFile) {
#if 0
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
      } else
      {
        assert(0);
      }

    } else {
      std::cerr << "wrong font file format " << std::endl;
      return false;
    }
  }
  assert(!master_addr.empty() && !group_name_.empty());
#endif
  return true;
}

bool StoreFaceAction::ParseParam(const std::string& body_str) {
//format:  
//url:imagepath_1|face_id_1,face_id_2...|hostid_1,hostid_2...|\r\n
//url:imagepath_2|face_id_1,face_id_2...|hostid_1,hostid_2...|
  if (body_str.empty())
    return false;
  boost::char_separator<char> sep("\r\n");
  tokenizer tokens(body_str, sep);
  int dis = std::distance(tokens.begin(), tokens.end());
  if (dis == 0)
    return false;
  for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end();
      ++tok_iter) {
    if (boost::starts_with(*tok_iter, "url:")) {
      std::string param = tok_iter->substr(4);
      boost::char_separator<char> param_sep("|");
      tokenizer tokens_param(param, param_sep);
      dis = std::distance(tokens_param.begin(), tokens_param.end());
      if (dis != 3)
        continue;

      tokenizer::iterator it = tokens_param.begin();
      std::string image_url_str = *it;
      ++it;
      std::string face_id_str = *it;
      ++it;
      std::string hostid_str = *it;
      boost::char_separator<char> face_id_sep(",");
      boost::char_separator<char> hostid_sep(",");
      tokenizer tokens_face_id(face_id_str, face_id_sep);
      tokenizer tokens_hostid(hostid_str, hostid_sep);
      int dis_face_id = std::distance(tokens_face_id.begin(),
                                      tokens_face_id.end());
      int dis_hostid = std::distance(tokens_hostid.begin(),
                                     tokens_hostid.end());
      if (dis_face_id != dis_hostid)
        continue;
      std::map<int, int> face_id_to_hostid;
      tokenizer::iterator iter_face_id = tokens_face_id.begin();
      tokenizer::iterator iter_hostid = tokens_hostid.begin();

      for (; iter_face_id != tokens_face_id.end();
          ++iter_face_id, ++iter_hostid) {
        int face_id_tmp = atoi(iter_face_id->c_str());
        if (face_id_tmp > 0)
          face_id_to_hostid.insert(
              std::make_pair(face_id_tmp, atoi(iter_hostid->c_str())));
      }
      url_to_face_map_.insert(std::make_pair(image_url_str, face_id_to_hostid));
    }
  }
  return url_to_face_map_.size() > 0;
}

bool StoreFaceAction::GetFaceFeatureByURL(const std::string& url,
                                          face_attribute& face_attr) {
  if (url.empty())
    return false;
  cached_value_ = g_memcache.Get(url);
  return ParseFaceAttribute(cached_value_.c_str(), cached_value_.length(),
                            face_attr, face_feature_buf_length_);
}

bool StoreFaceAction::ParseFaceAttribute(const char* value, int length,
                                         face_attribute& fa,
                                         int& length_feature_buffer) {
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
bool StoreFaceAction::DoStoreFaceFeatures() {
#if 0
  //id_count为key
  static const int area = 0;
  std::map<std::string,std::map<int,int> >::iterator it = url_to_face_map_.begin();
  std::vector<std::string> urls;
  for(;it!=url_to_face_map_.end();++it)
  {
    urls.push_back(it->first);
  }
  char key_count_str[256];
  char key_face_buffer_str[256];
  for(it = url_to_face_map_.begin();it!=url_to_face_map_.end();++it)
  {
    //从memcache里面
    face_attribute face_attr;
    bool success = GetFaceFeatureByURL(it->first,face_attr);
    if(success)
    {
      for(std::map<int,int>::iterator it2=it->second.begin();it2!=it->second.end();++it2)
      {
        //得到当前的数量
        sprintf(key_count_str,"%d_count",it2->second);
        const tair::common::data_entry key(key_count_str);
        int n_exist_count =-1;
        int ret = api_->incr(area,
            key,
            0,
            &n_exist_count,
            0,
            0);
        if(ret!=0 ||(n_exist_count>=0 && n_exist_count<10))
        {
          int face_id_temp = it2->first;  //1 based
          if(face_id_temp <=face_attr.vec_buffer.size())
          {
            const int hostid = it2->second;
            sprintf(key_face_buffer_str,"%d_%d",hostid,n_exist_count);

            const tair::common::data_entry key_face_buffer(key_face_buffer_str);
            const tair::common::data_entry value_face_buffer((const char*)face_attr.vec_buffer[it2->first-1],face_feature_buf_length_);
            int put_ret;
            {
              PTIME(pt1, "tair put", true, 5);
              put_ret = api_->put(area,key_face_buffer,value_face_buffer,0,0);
            }
            LOG(INFO) <<"tair put ret "<<put_ret;
            // write count
            int ret_count = 0;
            {
              PTIME(pt1, "tair incr", true, 5);
              put_ret = api_->incr(area,key,1,&ret_count,0,0);
            }
            LOG(INFO) <<"tair incr ret "<<put_ret;
          }
        }

      }
    }
  }
#endif
}

HttpStatusCode StoreFaceAction::Process(Request* request, Response* response) {

  const Request::DispositionArrayType & files = request->files();
  if (files.size() <= 0) {
    return HC_BAD_REQUEST;
  }

  bool b = ParseParam(files[0].data);
  if (!b)
    return HC_BAD_REQUEST;
  DoStoreFaceFeatures();
}
}

