#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <fstream>
#include <boost/filesystem.hpp>
#include "util.h"
#include "photo_head.h"
#include "imagehandler.h"

namespace fs = boost::filesystem;

bool _get_all_files(const fs::path &path, std::vector<std::string> *files) {

  //path.leaf();
  //path.file_string();
  if (fs::is_directory(path)) {
    fs::directory_iterator end_it;
    //fs::directory_iterator dir_itr(fullpath)
    for (fs::directory_iterator it(path); it != end_it; ++it) {
      try {
        if (fs::is_directory(*it)) {
          const char *subdir = it->path().c_str();
          std::cout << "subdir: " << subdir << std::endl;
          if (subdir[0] <= '9' && subdir[0] >= '0' &&
              subdir[1] <= '9' && subdir[1] >= '0')
            _get_all_files(it->path(), files);
        } else {
          std::string subfile(it->path().string());
          std::cout << "subfile: " << subfile << std::endl;
          //subfile.append("/");
          //subfile.append(it->leaf());
          files->push_back(subfile);
          //fs::path fullfile(fs::initial_path());
          //fullfile = fs::system_complete(fs::path(subfile, fs::native));
        }
      } catch (const std::exception &ex) {
         std::cout << "scan error: " << path << std::endl;
         return false;
      }
    }
  } else {
    std::cout << "need is path: " << path << std::endl;
    return false;
  }
  return true;
}

bool get_all_files(const std::string &path, std::vector<std::string> *files) {
  if (!files)
    return false;
  fs::path fullpath = fs::system_complete(path);
  if (!fs::exists(fullpath)) {
    return false;
  }
  _get_all_files(fullpath, files);
  return true;
}

bool get_dirs(const fs::path &path, std::vector<fs::path> *dirs) {
  fs::path fullpath = fs::system_complete(path);
  if (!fs::is_directory(path)) {
    return false;
  }
  fs::directory_iterator end_it;
  for (fs::directory_iterator it(path); it != end_it; ++it) {
    try {
      if (fs::is_directory(*it)) {
        std::cout << "subdir: " << it->path() << std::endl;
        if (dirs)
          dirs->push_back(it->path());
      }
    } catch (const std::exception &ex) {
       std::cout << "scan error: " << path << std::endl;
       return false;
    }
  }
  return true;
}

bool get_filenames(const std::string &path, std::vector<std::string> *filenames, std::vector<std::string> *stems) {
  fs::path fullpath = fs::system_complete(path);
  if (!fs::is_directory(path)) {
    return false;
  }
  fs::directory_iterator end_it;
  for (fs::directory_iterator it(path); it != end_it; ++it) {
    try {
      if (!fs::is_directory(*it)) {
        std::cout << "subfile: " << it->path().filename() << std::endl;
        if (filenames)
          filenames->push_back(it->path().filename().string());
        if (stems)
          stems->push_back(it->path().filename().string());
      }
    } catch (const std::exception &ex) {
       std::cout << "scan error: " << path << std::endl;
       return false;
    }
  }
  return true;
}

std::string MakeLogfile(const std::string &savepath, const fs::path &p) {
  std::string stem = p.stem().string();
  return savepath + "/" + stem;
}

bool HandleTimeDir(const fs::path &pt, int &olog, int& count, int &fail, int &noexist) {
  fs::path fullpath = fs::system_complete(pt);
  if (!fs::is_directory(pt)) {
    return false;
  }
  fs::directory_iterator end_it;
  std::string parent_dir = pt.stem().c_str();
  for (fs::directory_iterator it(pt); it != end_it; ++it) {
    try {
      if (fs::is_directory(*it))
        continue;
      if (++count % 100)
        std::cout << "." ;
      std::string data = LoadFile(it->path().string());
      if (data.empty()) {
        char tmp[1024];
        int sz = sprintf(tmp, "%s/%s -1 -1\n", parent_dir.c_str(), it->path().c_str());
        if (write(olog, tmp, sz) != sz) {
          return false;
        }
        //olog << parent_dir << "/" << it->path().filename() << " -1 -1"<< std::endl;
        noexist ++;
        continue;
      }
      photo::OutResponse resp;
      if (photo::ImageHandler::VerityImage(data, &resp)) {
        char tmp[1024];
        int sz = sprintf(tmp, "%s/%s %d %d\n", parent_dir.c_str(), it->path().c_str(), resp.width_, resp.height_);
        if (write(olog, tmp, sz) != sz) {
          return false;
        }
        //olog << parent_dir << "/" << it->path().filename() << " " << resp.width_ << " " << resp.height_ << std::endl;
      } else {
        char tmp[1024];
        int sz = sprintf(tmp, "%s/%s 0 0\n", parent_dir.c_str(), it->path().c_str());
        if (write(olog, tmp, sz) != sz) {
          return false;
        }
        fail++;
      }
    } catch (const std::exception &ex) {
       std::cout << "scan error: " << pt << std::endl;
       return false;
    }
  }
  return true;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "plz input /bigpool/fmn047 and fmn047" << std::endl;
  }
  std::string path = argv[1];
  std::string savepath = argv[2];
  
  /*
  std::vector<std::string> handledfiles;
  if (get_filenames(savepath, &handledfiles, NULL)) {
    std::cout << "no exist dir" << std::endl;
    return 1;
  }
  std::set<std::string> setfiles;
  for (std::vector<std::string>::iterator it = handledfiles.begin(); it != handledfiles.end(); ++it) {
    setfiles.insert(*it);
  }*/
  std::vector<fs::path> dirs;
  bool  b = get_dirs(path, &dirs);
  if (b) {
    for (std::vector<fs::path>::iterator it = dirs.begin(); it != dirs.end(); ++it) {
      std::string log = MakeLogfile(savepath, *it);
      //std::ofstream olog(log.c_str(), std::ios::app);
      int olog = open(log.c_str(), O_RDWR | O_CREAT | O_EXCL | O_LARGEFILE, S_IRUSR|S_IWUSR|S_IRGRP);
      if (!olog) {
        std::cout << "log file: " << log << "exist,handle next" << std::endl;
        continue;
      } else {
        std::cout << "start log file: " << log << std::endl;
      }
      std::vector<fs::path> timedirs;
      bool  b = get_dirs(it->string(), &timedirs);
      if (b) {
        for (std::vector<fs::path>::iterator it1 = timedirs.begin(); it1 != timedirs.end(); ++it1) {
          int count = 0;
          int fail = 0;
          int noexist = 0;
          std::cout << "--start time dir: " << it1->string() << std::endl;
          HandleTimeDir(*it1, olog, count, fail, noexist);
          std::cout << "--end time dir: count: " << count  << " noexist: " << noexist << " fail:" << fail << std::endl;
        }
      }
      close(olog);
    }
  }
  //imagehandler_.ProcessImage(resp.url_, &resp);
  return 0;
}
