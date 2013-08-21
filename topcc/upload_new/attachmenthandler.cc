#include "attachmenthandler.h"
#include "base3/logging.h"
#include "util.h"
#include "runtimeconfig.h"

namespace attachment {

// 生成url的路径的文件名部分,如: a_FCtt_3d8b000032331264.txt
bool GenerateDbPath( const std::string &namepre1,// ?p
                      const std::string &ext, // 原始扩展名 
                      std::string* dbpath) {
  if (!dbpath)
    return false;
  
  if (!namepre1.empty()) {
    *dbpath += namepre1;
    *dbpath += "_";
  }
  std::string uniqueStr = GetUniqueStr();
  std::string randomStr = GetRandomStr(4);

  *dbpath += "a_" + randomStr + "_" + uniqueStr;
  if (!ext.empty())
    *dbpath += + "." + ext;

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
    sprintf(str, "附件文件尺寸过大,最大支持%.1fM", (double)sc->maxfilesize_/1024/1024);
    outfile->msg_ = str;
    LOG(ERROR) << "verityfile,size :" << outfile->filesize_ ;
    return false;
  }

  // 扩展名校验
  if (sc->exts_.empty() == false && *sc->exts_.begin() != "*") {
    size_t pos = outfile->filename_.find_last_of('.');
    if (pos != std::string::npos) {
      std::string lowext = ToLower(outfile->filename_.substr(pos+1));
      if (sc->exts_.find(lowext) == sc->exts_.end()) {
        pos = std::string::npos;
      }
    }
    if (pos == std::string::npos) {
      outfile->code_ = 531;
      outfile->msg_ = "无效的文件扩展名";
      LOG(ERROR) << "verityfile,file name is error:" << outfile->filename_;
      return false;
    }
  }
  // 如果是ie的话，给的是全路径，需要截取
  // LOG(INFO) << "<>" << req.Env();
  // 处理
  //LOG(INFO) << "file name is sucess:" << outfile->filename_;
  return true;
}

int AttachmentHandler::ProcessAttachment(const ServiceConf* sc, const cwf::Request::DispositionArrayType &files, upload::OutResponse *resp) {

  // 对每张图片进行处理
  int sucess = 0;
  size_t filecount = files.size();
  resp->files_.resize(filecount);
  for (size_t i = 0; i < filecount; i++, sucess++) {
    upload::OutFile& outfile = resp->files_[i];
    outfile.filesize_ = files[i].data.size();
    outfile.filename_ = files[i].filename;
    outfile.width_ = 0; outfile.height_ = 0;
    if (!VerityFile(sc, &outfile))
      return false;
    std::string magick;
    outfile.images_.push_back(upload::OutImageUrl());
    upload::OutImageUrl &iu = outfile.images_.back();
    iu.nameprefix_ = "a";
    iu.data_ = files[i].data;
    size_t pos = outfile.filename_.find_last_of('.');
    if (pos != std::string::npos) {
      magick = outfile.filename_.substr(pos+1);
    }
    GenerateDbPath(sc->savenameprefix_, magick, &iu.url_);
  }
  return sucess;
}


}
