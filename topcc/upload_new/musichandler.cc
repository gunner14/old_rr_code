#include "musichandler.h"
#include "id3/readers.h"
#include "id3/misc_support.h"
#include "id3/tag.h"
#include "base3/logging.h"
#include "runtimeconfig.h"
#include "util.h"


namespace music {

// 生成url的路径的文件名部分,如: m_FCtt_3d8b000032331264.mp3
bool GenerateDbPath( const std::string &namepre1,// ?p
                      const std::string &ext, // 原始扩展名, 大写如PNG
                      std::string* dbpath) {
  if (!dbpath)
    return false;
  
  if (!namepre1.empty()) {
    *dbpath += namepre1;
    *dbpath += "_";
  }
  std::string uniqueStr = GetUniqueStr();
  std::string randomStr = GetRandomStr(4);

  *dbpath += "m_" + randomStr + "_" + uniqueStr + ext;

  return true;
}

bool VerityFile(const ServiceConf* sc, upload::OutFile *outfile) {
  if (!outfile)
    return false;
  outfile->code_ = 0;

  // 大小校验
  if (outfile->filesize_ > sc->maxfilesize_) {
    outfile->code_ = 530;
    char str[256];
    sprintf(str, "音频文件尺寸过大,最大支持%.1fM", (double)sc->maxfilesize_/1024/1024);
    outfile->msg_ = str;
    LOG(ERROR) << "verityfile,size :" << outfile->filesize_ ;
    return false;
  }

  // 扩展名校验
  size_t pos = outfile->filename_.find_last_of('.');
  if (pos != std::string::npos) {
    std::string lowext = ToLower(outfile->filename_.substr(pos+1));
    if (sc->exts_.find(lowext) == sc->exts_.end()) {
      pos = std::string::npos;
    }
  }
  if (pos == std::string::npos) {
    outfile->code_ = 531;
    outfile->msg_ = "无效的文件扩展名,必须为mp3";
    LOG(ERROR) << "verityfile,file name is error:" << outfile->filename_;
    return false;
  }
  // 如果是ie的话，给的是全路径，需要截取
  // LOG(INFO) << "<>" << req.Env();
  // 处理
  //LOG(INFO) << "file name is sucess:" << outfile->filename_;
  return true;
}

bool VerityMusic(const std::string &data, std::string *magick) {
  if (!magick) return true;
  *magick = ".mp3";
  if (data.substr(0, 4) == std::string("OggS")) *magick = ".ogg";
  return true;
}

inline std::string tostring(const char *value) {
  std::string ss;
  if (value) {
    ss = value; 
    ss = GBKToUTF8(ss);
    delete value;
  }
  return ss;
}

void HandleID3(const std::string &data, std::map<std::string, std::string> *id3s) {
  ID3_MemoryReader reader(data.c_str(), data.size());
  ID3_Tag tag;
  tag.Link(reader);
  id3s->insert(std::make_pair("artist", tostring(ID3_GetArtist(&tag))));
  id3s->insert(std::make_pair("album", tostring(ID3_GetAlbum(&tag))));
  id3s->insert(std::make_pair("title", tostring(ID3_GetTitle(&tag))));
  id3s->insert(std::make_pair("year", tostring(ID3_GetYear(&tag))));
  id3s->insert(std::make_pair("comment", tostring(ID3_GetComment(&tag))));
  id3s->insert(std::make_pair("track", tostring(ID3_GetTrack(&tag))));
  id3s->insert(std::make_pair("genre", tostring(ID3_GetGenre(&tag))));
  // id3s.insert(std::make_pair("lyrics", ID3_GetLyrics(&tag)));
  id3s->insert(std::make_pair("lyricist", tostring(ID3_GetLyricist(&tag))));
}

int MusicHandler::ProcessMusic(const ServiceConf* sc, const cwf::Request::DispositionArrayType &files, upload::OutResponse *resp) {

  // 对每张图片进行处理
  int sucess = 0;
  size_t filecount = files.size();
  resp->files_.resize(filecount);
  for (size_t i = 0; i < filecount; i++,sucess++) {
    upload::OutFile& outfile = resp->files_[i];
    outfile.filesize_ = files[i].data.size();
    outfile.filename_ = files[i].filename;
    outfile.width_ = 0; outfile.height_ = 0;
    if (!VerityFile(sc, &outfile))
      return false;
    std::string magick;
    if (!VerityMusic(files[i].data, &magick))
      return false;
    HandleID3(files[i].data, &outfile.exifs_);
    outfile.images_.push_back(upload::OutImageUrl());
    upload::OutImageUrl &iu = outfile.images_.back();
    iu.nameprefix_ = "m";
    iu.data_ = files[i].data;
    GenerateDbPath(sc->savenameprefix_, magick, &iu.url_);
  }
  return sucess;
}


}
