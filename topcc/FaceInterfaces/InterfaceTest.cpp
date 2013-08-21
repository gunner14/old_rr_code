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
#include <dirent.h>
#include <sys/stat.h>
#include <boost/algorithm/string/predicate.hpp>
#include "ptime.h"

using namespace cv;

bool GetFileBuffer_Ex(const char *file, char **buf, int* len) {

  if (file == NULL)
    return false;
  std::ifstream ifile(file, std::ios::in | std::ios::binary | std::ios::ate);
  if (!ifile)
    return false;
  *len = ifile.tellg();
  ifile.seekg(0, std::ios::beg);
  *buf = new char[*len];
  if (*buf) {
    ifile.read(*buf, *len);
    ifile.close();
    return true;
  }
  return false;

}

bool compare1(const std::string &a, const std::string &b) {
  return a < b;
}

bool Load(const std::string& path, std::vector<std::string>& filepath_array) {
  DIR * dir;
  std::string currfile;
  struct dirent *s_dir;
  struct stat file_stat;
  if (path.empty())
    return false;

  dir = opendir(path.c_str());
  while ((s_dir = readdir(dir)) != NULL) {
    if ((strcmp(s_dir->d_name, ".") == 0) || (strcmp(s_dir->d_name, "..") == 0))
      continue;
    currfile = path + s_dir->d_name;
    stat(currfile.c_str(), &file_stat);
    if (S_ISDIR(file_stat.st_mode))
      continue;
    else {
      if (boost::algorithm::ends_with(currfile, ".JPEG")
          || boost::algorithm::ends_with(currfile, ".JPG")
          || boost::algorithm::ends_with(currfile, ".jpeg")
          || boost::algorithm::ends_with(currfile, ".jpg")) {
        filepath_array.push_back(currfile);
      }
    }
  }
  sort(filepath_array.begin(), filepath_array.end(), compare1);

  closedir(dir);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("usages: %s dir_name\n", argv[0]);
    return 0;
  }
  std::string dir_path(argv[1]);
  std::vector < std::string > file_path_array;

  FaceDetector* detector = new FaceDetector;
  detector->init("data/front_15_cascade.xml");

  CFaceFeature* descriptor = new CFaceFeature;
  descriptor->LoadFaceAsm("data/facelandmark");
  FaceClustering* facecluster = new FaceClustering;
  Load(dir_path, file_path_array);
  std::vector<float*> vec_feature_buffer;
  for (size_t i = 0; i < file_path_array.size(); i++) {
    Mat pImage = cv::imread(file_path_array[i].c_str(), 1);
    int imWeight = pImage.cols;
    int imHeight = pImage.rows;

    Mat dImage;
    cv::cvtColor(pImage, dImage, CV_BGR2GRAY);
    vector < FRECT > faceSet;
    bool b;
    {
      PTIME(pt1, "detect face", true, 5);

      b = detector->detectFace(dImage.data, imWeight, imHeight, faceSet);
    }
    if (b) {
      int n = faceSet.size();
      std::string s = n > 1 ? "faces" : "face";
      std::cout << file_path_array[i] << " detect " << n << " " << s << endl;
    } else {

      std::cout << file_path_array[i] << " did not detect face(s)" << endl;
      continue;
    }
    vector < rect > faceRegion;

    for (int i = 0; i < faceSet.size(); i++) {
      rect faceRect;
      faceRect.x0 = faceSet[i].x;
      faceRect.y0 = faceSet[i].y;
      faceRect.x1 = faceSet[i].x + faceSet[i].w - 1;
      faceRect.y1 = faceSet[i].y + faceSet[i].h - 1;
      faceRegion.push_back(faceRect);
      std::cout << faceRect.x0 << " " << faceRect.y0 << " " << faceRect.x1
          << " " << faceRect.y1 << "\n";
    }

    if (faceSet.size() > 0) {

      std::vector<float*> featureBuffer;
      {
        PTIME(pt1, "extractFeature", true, 5);

        descriptor->extractFeature(dImage.data, imWeight, imHeight, faceRegion,
                                   featureBuffer);
      }
      cout << endl;
      vec_feature_buffer.insert(
          vec_feature_buffer.begin() + vec_feature_buffer.size(),
          featureBuffer.begin(), featureBuffer.end());
    }
  }
  vector<int> clusterNum(vec_feature_buffer.size(), -1);
  cout << "total " << clusterNum.size() << " features to cluster\n";
  {
    PTIME(pt1, "cluster", true, 5);
    facecluster->cluster(vec_feature_buffer, clusterNum);
  }
  for (int i = 0; i < clusterNum.size(); i++) {
    cout << clusterNum[i] << ",";
  }
  cout << endl;
  return 0;
}
