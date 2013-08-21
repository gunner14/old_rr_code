#include "action_detect.h"
#include "base3/logging.h"
#include "base3/ptime.h"
#include "base3/escape.h"
#include <boost/algorithm/string.hpp>
#include "facedetection/facedetectorimpl.h"
#include "facefeature/facefeatureimpl.h"
#include "faceclustering/faceclusteringimpl.h"
#include <string>
#include <vector>
#include <iostream>
#include "highgui.h"
#include <cv.h>  
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boost/tokenizer.hpp>
#include "arch_diff/site_xiaonei.h"

using namespace cv;

namespace upload {

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
const std::string DetectAction::kMethods_[] = { "/detect" };

DetectAction::DetectAction() {

  pvbuffer_ = (char*) malloc(256 * 1024);
  Init("./conf/face.conf");
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

}

bool DetectAction::Match(const std::string& url) const {
  for (size_t i = 0; i < sizeof(kMethods_) / sizeof(kMethods_[0]); ++i) {
    if (boost::starts_with(url, kMethods_[i])) {
      return true;
    }
  }
  return false;
}

void DetectAction::Stop() {
  free (pvbuffer_);
}

bool DetectAction::Init(const std::string& strFile) {
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
      if (key == "cache_expiration") {
        cache_expiration_ = atoi(value.c_str());
      } else if (key == "friend_count_limit") {
        friend_count_limit_ = atoi(value.c_str());
      } else if (key == "friend_buffer_limit") {
        friend_buffer_limit_ = atoi(value.c_str());
      } else

      {
        assert(0);
      }

    } else {
      std::cerr << "wrong font file format " << std::endl;
      return false;
    }
  }
  return true;
}

bool DetectAction::WriteFaceAttribute(const std::string& file_path,
                                      const std::vector<rect>& vecRect,
                                      const std::vector<float*>& vecbuffer) {
  const unsigned int start = 0xFFFFEEEE;
  const unsigned int end = 0xEEEEFFFF;
  //tlv
  const int type_path = 1;
  const int type_face_id = 2;
  const int type_face_rect = 3;
  const int type_face_buffer = 4;
  const int type_end = 0;
  int offset = 0;
  //start
  memcpy(pvbuffer_, &start, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  //path start
  //path type
  memcpy(pvbuffer_ + offset, &type_path, sizeof(int));
  offset += sizeof(type_path);

  //path length 
  int path_length = file_path.length();
  memcpy(pvbuffer_ + offset, &path_length, sizeof(int));
  offset += sizeof(path_length);

  //path value

  memcpy(pvbuffer_ + offset, file_path.c_str(), file_path.length());
  offset += file_path.length();
  //path end

  //face related start 
  int id = 1;
  const int face_id_length = 4;
  const int rect_memory_size = sizeof(rect);
  const int featurebuffer_length = sizeof(float)
      * descriptor_->GetFeatureLength();

  for (int i = 0; i < vecbuffer.size(); i++) {
    //face id  
    //face id type 
    memcpy(pvbuffer_ + offset, &type_face_id, sizeof(int));
    offset += sizeof(int);
    //face id length 
    memcpy(pvbuffer_ + offset, &face_id_length, sizeof(int));
    offset += sizeof(int);

    //face id value 
    memcpy(pvbuffer_ + offset, &id, sizeof(int));
    offset += sizeof(int);
    id++;

    //face rect type 
    memcpy(pvbuffer_ + offset, &type_face_rect, sizeof(int));
    offset += sizeof(int);
    //face rect length
    memcpy(pvbuffer_ + offset, &rect_memory_size, sizeof(int));
    offset += sizeof(int);

    //face rect value 
    memcpy(pvbuffer_ + offset, &vecRect[i], sizeof(rect));
    offset += sizeof(rect);

    //face buffer type 
    memcpy(pvbuffer_ + offset, &type_face_buffer, sizeof(int));
    offset += sizeof(int);
    //face buffer length
    memcpy(pvbuffer_ + offset, &featurebuffer_length, sizeof(int));
    offset += sizeof(int);
    //face buffer value 
    memcpy(pvbuffer_ + offset, vecbuffer[i], featurebuffer_length);
    offset += featurebuffer_length;

  }
  //face related end

  //end type
  memcpy(pvbuffer_ + offset, &type_end, sizeof(int));
  offset += sizeof(int);
  //end length
  int end_length = 4;
  memcpy(pvbuffer_ + offset, &end_length, sizeof(int));
  offset += sizeof(int);
  //end value 
  int end_value = 0;
  memcpy(pvbuffer_ + offset, &end_value, sizeof(int));
  offset += sizeof(int);

  //end
  memcpy(pvbuffer_ + offset, &end, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  str_buffer_ = std::string(pvbuffer_, offset);
  return g_memcache.Set(file_path, str_buffer_, cache_expiration_, 0);

}

bool DetectAction::ParseImagePath(const std::string& image_path,
                                  std::string& image_url, std::string& hostid) {
  if (image_path.empty())
    return false;
  boost::char_separator<char> sep("|");
  tokenizer tokens(image_path, sep);
  int dis = std::distance(tokens.begin(), tokens.end());
  if (dis != 2)
    return false;
  int i = 0;
  for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end();
      i++, ++tok_iter) {
    if (!tok_iter->empty()) {
      size_t pos = tok_iter->find_first_of('=');
      if (pos != std::string::npos) {
        if (i == 0) {
          image_url = tok_iter->substr(pos + 1);
        } else {
          hostid = tok_iter->substr(pos + 1);
        }
      }
    }

  }

  return true;
}

HttpStatusCode DetectAction::Process(Request* request, Response* response) {

  const Request::DispositionArrayType & files = request->files();
  if (files.size() <= 0) {
    return HC_BAD_REQUEST;
  }
  std::string imagePath = request->header("IMAGE_PATH");
  pid_t curpid = getpid();
  static int times = 1;
  std::string image_url;
  std::string hostid;
  bool b = ParseImagePath(imagePath, image_url, hostid);
  if (!b)
    return HC_BAD_REQUEST;
  vector < FRECT > faceSet;

  std::vector<unsigned char> image_buffer;
  image_buffer.assign(
      request->file_array_[0].data.c_str(),
      request->file_array_[0].data.c_str()
          + request->file_array_[0].data.size());
  Mat pImage;
  {
    PTIME(pt1, "imdecode", true, 5);
    pImage = cv::imdecode(image_buffer, 1);
  }

  Mat dImage;
  int imWeight = pImage.cols;
  int imHeight = pImage.rows;

  {
    PTIME(pt1, "CV_BGR2GRAY", true, 5);
    cv::cvtColor(pImage, dImage, CV_BGR2GRAY);
  }
  byte* imgData = dImage.data;
  {

    PTIME(pt1, "detect face", true, 5);
    detector_->detectFace(imgData, imWeight, imHeight, faceSet);
  }
  LOG(INFO) << "image_url=" << image_url << ",hostid=" << hostid
      << ",face_count=" << faceSet.size();
  if (faceSet.size() > 0) {
    vector < rect > faceRegion;
    vector<float*> featureBuffer;
    for (int i = 0; i < faceSet.size(); i++) {
      rect faceRect;
      faceRect.x0 = faceSet[i].x;
      faceRect.y0 = faceSet[i].y;
      faceRect.x1 = faceSet[i].x + faceSet[i].w - 1;
      faceRect.y1 = faceSet[i].y + faceSet[i].h - 1;
      faceRegion.push_back(faceRect);
    }

    {
      PTIME(pt1, "extractFeature", true, 5);
      descriptor_->extractFeature(imgData, imWeight, imHeight, faceRegion,
                                  featureBuffer);
    }
    size_t pos = image_url.find("large_");
    if (pos != std::string::npos) {
      image_url = image_url.erase(pos, 6);
    } else {
      pos = image_url.find("original_");
      if (pos != std::string::npos)
        image_url = image_url.erase(pos, 9);
    }

    bool ret;
    {
      PTIME(pt1, "write cache", true, 5);
      ret = WriteFaceAttribute(image_url, faceRegion, featureBuffer);
    }
    if (ret)
      LOG(INFO) << image_url << " write cache success";
    else
      LOG(INFO) << image_url << " write cache failed";
  }
  return HC_OK;
}
}

