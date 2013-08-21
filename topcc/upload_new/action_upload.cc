#include "action_upload.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include<Magick++.h>

#include "upload_head.h"
#include "base3/logging.h"
#include "base3/ptime.h"
#include "base3/escape.h"
#include "upload/syncdownload.h"
#include "tickmanager.h"
#include "runtimeconfig.h"
#include "blockmanage.h"
#include "util.h"
#include "downloader.h"
#include "util_upload.h"
#include "arch_diff/site_xiaonei.h"
#include "album_util.h"

namespace upload {

const std::string UploadAction::kMethods_[] = { "/upload.fcgi", "/upload1.fcgi",
    "/upload2.fcgi", "/zhbdebug.fcgi" };
UploadAction::UploadAction() {
}

bool UploadAction::Match(const std::string& url) const {
  for (size_t i = 0; i < sizeof(kMethods_) / sizeof(kMethods_[0]); ++i) {
    if (boost::starts_with(url, kMethods_[i])) {
      return true;
    }
  }
  return false;
}

void UploadAction::Stop() {
  disp_.Stop();
}

HttpStatusCode UploadAction::Process(Request* request, Response* response) {
  OutResponse resp;
  std::string service = request->get("pagetype");
  std::vector<int> cachetimes;
  const ServiceConf* sc;
  std::string rotate;
  std::string img_src;
  int block_index;
  int block_count;
  double dl;
  int pcount;
  OutputType defaulttype = kOutputJson;
  std::string defaultdomain;
  std::string channel = "album";
  // for thumbnail image
  bool is_thumbnail = (service == "addthumbnail");
  int uid = -1, crop_x = 0, crop_y = 0;
  Magick::Geometry crop_geom; // default invalid
  const std::string& crop_geom_str = request->get("crop_geom");
  std::string out_large_files;

  // 初始化resp
  int filecount = request->file_array_.size();
  resp.uploadid_ = request->get("uploadid");

  std::string t = request->cookie().Get("t");
  std::string tick = request->get("tick");
  std::string hostid = request->get("hostid");
  if (hostid.empty() && is_thumbnail) {
    hostid = request->get("user");
  }

  resp.code_ = 0;
  LOG(INFO) << "new upload: " << " pagetype: " << service << " uploadid: "
      << resp.uploadid_ << " form count: " << request->forms().size()
      << " CONTENT_LENGTH: " << request->header("CONTENT_LENGTH")
      << " file count: " << filecount << " Hostid: " << hostid << " t: " << t
      << " Addr: " << request->header("X_Forwarded_For");

  if (service.empty()) {
    resp.code_ = 503;
    resp.msg_ = "缺少参数或参数错误, 请配置pagetype参数";
    Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
        resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
        hostid.c_str());
    goto EXIT_OUT;
  }

  sc = g_runtime_config.GetServiceConf(service);
  if (sc == NULL) {
    resp.code_ = 508;
    resp.msg_ = "不支持的业务类型";
    Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
        resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
        hostid.c_str());
    goto EXIT_OUT;
  }

  if (hostid.empty()) {
    resp.code_ = 503;
    resp.msg_ = "缺少参数或参数错误, 请配置hostid参数";
    Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
        resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
        hostid.c_str());
    goto EXIT_OUT;
  }

  if (!crop_geom_str.empty()) {
    if (!ParseCropGeom(crop_geom_str, &crop_geom)) {
      resp.code_ = 503;
      resp.msg_ = "CropGeom参数错误";
      Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s,cropgeom=%s",
          resp.code_, resp.msg_.c_str(), service.c_str(),
          resp.uploadid_.c_str(), hostid.c_str(), crop_geom_str.c_str());
      goto EXIT_OUT;
    }
  }

  img_src = request->get("img_src"); // 仅仅自身不包含文件时使用
  if (is_thumbnail) {
    const std::string& xStr = request->get("x");
    const std::string& yStr = request->get("y");
    try {
      uid = boost::lexical_cast<int>(hostid);
      crop_x = boost::lexical_cast<int>(xStr);
      crop_y = boost::lexical_cast<int>(yStr);
    } catch (const std::exception& e) {
      resp.code_ = 503;
      resp.msg_ = "参数转换错误" + std::string(e.what());
      Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s,crop_x=%s,crop_y=%s",
          resp.code_, resp.msg_.c_str(), service.c_str(),
          resp.uploadid_.c_str(), hostid.c_str(), xStr.c_str(), yStr.c_str());
      goto EXIT_OUT;
    }
    img_src = xce::userUrl(uid, xce::userbase::LARGEURL);
    if (img_src.empty()) {
      resp.code_ = 503;
      resp.msg_ = "缺少参数或参数错误, 未找到用户大图url";
      Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
          resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
          hostid.c_str());
      goto EXIT_OUT;
    }
    // get full url
    img_src = album_util::AlbumUtil::getPhotoURL(img_src);
  }
  defaulttype = sc->outputtype_;
  channel = sc->channel_;
  defaultdomain = sc->domain_;

  if (resp.uploadid_.empty()) {
    resp.code_ = 503;
    resp.msg_ = "缺少参数或参数错误, 请配置uploadid参数";
    Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
        resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
        hostid.c_str());
    goto EXIT_OUT;
  }

  // 验票
#if CHECK_INTERNAL
  if (!g_tickmanager.Valid(sc->tickettype_,
          service,
          sc->ticketsuffix_,
          hostid,
          t,
          tick,
          request->header("X_REAL_IP"),
          request->header("X_Forwarded_For"))) {
#else 
  if (!g_tickmanager.Valid(sc->tickettype_, service, sc->ticketsuffix_, hostid,
                           t, tick, request->header("REFERER"))) {
#endif
    resp.code_ = 501;
    resp.msg_ = "用户未登录";
    Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s,t=%s,tick=%s",
        resp.code_, resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
        hostid.c_str(), t.c_str(), tick.c_str());
    goto EXIT_OUT;
  }

  rotate = request->get("rotate");

  // 处理windows
  if (request->header("USER_AGENT").find("MSIE") != string::npos) {
    for (cwf::Request::DispositionArrayType::iterator it =
        request->file_array_.begin(); it != request->file_array_.end(); ++it) {
      it->filename = GetFileName(it->filename);
    }
  }

  // 文件验证
  if (filecount == 0) {
    if (!img_src.empty()) {
      std::string data;
      std::string str = "download_image ";
      str += img_src;
      PTIME(pt1, str, true, 5);
      if (!DownImage(img_src, data)) {
        resp.code_ = 503;
        resp.msg_ = "参数错误或超时, 请重试";
        Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s, img_src=%s",
            resp.code_, resp.msg_.c_str(), service.c_str(),
            resp.uploadid_.c_str(), hostid.c_str(), img_src.c_str());
        goto EXIT_OUT;
      }
      Request::FormDisposition fd;
      //fd.name = "imgurl";
      fd.filename = "weburl.jpg";
      //fd.content_type = "application/octet-stream";
      fd.data = data;
      request->file_array_.push_back(fd);
      filecount = 1;
    } else {
      resp.code_ = 504;
      resp.msg_ = "没上传文件,或者包不完整";
      Log("OutputError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
          resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
          hostid.c_str());
      goto EXIT_OUT;
    }
  }

  for (std::vector<HandleImageConf>::const_iterator it =
      sc->handleimages_.begin(); it != sc->handleimages_.end(); ++it) {
    cachetimes.push_back(it->nginx_cache_);
  }

  // 处理旋转
  if (!rotate.empty()) {
    bool b = false;
    resp.files_.resize(1);
    OutFile &outfile = resp.files_[0];
    outfile.width_ = 0;
    outfile.height_ = 0;
    outfile.filename_ = request->file_array_[0].filename;
    outfile.filesize_ = request->file_array_[0].data.size();

    double dl = atof(rotate.c_str());
    if (dl < 1 && dl > -1) {
      resp.code_ = 503;
      resp.msg_ = "缺少参数或参数错误, rotate参数错误";
      Log("RouteError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s,rotate=%s",
          resp.code_, resp.msg_.c_str(), service.c_str(),
          resp.uploadid_.c_str(), hostid.c_str(), rotate.c_str());
      goto EXIT_OUT;
    }
    b = imagehandler_.Route(sc, dl, request->file_array_[0].data, &outfile);
    if (b) {
      if (disp_.Disp(hostid, sc, cachetimes, resp) == 0) {
        resp.code_ = 536;
        resp.msg_ = "处理文件失败";
        Log("DispError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
            resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
            hostid.c_str());
      }
    } else {
      resp.code_ = 536;
      resp.msg_ = "处理文件失败";
      Log("RouteError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
          resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
          hostid.c_str());
    }
    goto EXIT_OUT;
  }

  // 处理分块
  //char * pp = request->get("block_index");
  block_index = atoi(request->get("block_index").c_str());
  block_count = atoi(request->get("block_count").c_str());
  if (block_index >= 0 && block_count > 1) {
    filecount = 1;
    resp.files_.resize(filecount);
    OutFile &outfile = resp.files_[0];
    outfile.width_ = 0;
    outfile.height_ = 0;
    outfile.filesize_ = 0;
    if (request->file_array_[0].data.size() > 1024 * 1000) { // request->file_array_ 前面确保不为空
      outfile.code_ = 522;
      outfile.msg_ = "缓存满或写缓存失败, 每块不能大于1024,000B";
      resp.code_ = 536;
      resp.msg_ = "处理文件失败";
      Log("BlockError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s,blocksize=%d",
          resp.code_, resp.msg_.c_str(), service.c_str(),
          resp.uploadid_.c_str(), hostid.c_str(),
          (int) request->file_array_[0].data.size());
      goto EXIT_OUT;
      // 可能转到ProcessImage处理
    } else if (block_index >= block_count) {
      outfile.code_ = 503;
      outfile.msg_ = "缺少参数或参数错误, 块标记错误";
      resp.code_ = 536;
      resp.msg_ = "处理文件失败";
      Log("BlockError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s,blockindex=%d,blockcount=%d",
          resp.code_, resp.msg_.c_str(), service.c_str(),
          resp.uploadid_.c_str(), hostid.c_str(), block_index, block_count);
      goto EXIT_OUT;
      // 可能转到ProcessImage处理
    } else {
      //std::vector<int> losts;
      LOG(INFO) << "block manager: " << block_index << " count: "
          << block_count;
      int res = g_blockmanager.SetBlock(hostid, resp.uploadid_, block_index,
                                        block_count,
                                        request->file_array_[0].data,
                                        &request->file_array_[0].data,
                                        &outfile.lostblocks_);
      if (res < 1) {
        if (res < 0) {
          outfile.code_ = 522;
          outfile.msg_ = "缓存满或写缓存失败";
          resp.code_ = 536;
          resp.msg_ = "处理文件失败";
          Log("BlockError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
              resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
              hostid.c_str());
        } else if (res == 0) {
          outfile.code_ = 523;
          outfile.msg_ = "仍有块待传";
          Log("BlockOk:%d,%s,pagetype=%s,uploadid=%s,hostid=%s", resp.code_,
              resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
              hostid.c_str());
        }
        goto EXIT_OUT;
        // 可能转到ProcessImage处理
      }
    }
    LOG(INFO) << "block manager finish";
  }

  if (sc->channel_ == "music") {
    pcount = musichandler_.ProcessMusic(sc, request->files(), &resp);
  } else if (sc->channel_ == "attachment") {
    pcount = attachmenthandler_.ProcessAttachment(sc, request->files(), &resp);
  } else {
    if (!is_thumbnail) {
      pcount = imagehandler_.ProcessImage(sc, hostid, request->files(),
                                          photo::PROCESS_TYPE_COMMON, crop_geom, &resp);
    } else {
      pcount = imagehandler_.ProcessImage(sc, hostid, request->files(), crop_x,
                                          crop_y, &resp);
    }
  }

  if (pcount > 0) {
    int c = disp_.Disp(hostid, sc, cachetimes, resp);
    if (c == 0) {
      resp.code_ = 536;
      resp.msg_ = "处理文件失败";
      Log("DispError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s,fail=%d",
          resp.code_, resp.msg_.c_str(), service.c_str(),
          resp.uploadid_.c_str(), hostid.c_str(),
          (int) request->files().size() - c);
    } else {
      if (is_thumbnail) {
        if (resp.files_[0].images_.size() >= 1) {
          xce::setUserUrl(uid, xce::userbase::TINYURL, resp.files_[0].images_[0].url_);
          if (resp.files_[0].images_.size() >= 2) {
            xce::setUserUrl(uid, xce::userbase::FLEXURL, resp.files_[0].images_[1].url_);
          } 
        }
      }
      for (size_t i = 0; i < resp.files_.size(); ++i) {
        if (resp.files_[i].code_ == 0) {
          if (out_large_files.empty()) {
            out_large_files = resp.files_[i].images_[0].url_;
          } else {
            out_large_files += "|" + resp.files_[i].images_[0].url_;
          }
        }
      }
      Log("OutputOk:%d,%s,large=%s,pagetype=%s,uploadid=%s,hostid=%s,sucess=%d,fail=%d",
          resp.code_, resp.msg_.c_str(),out_large_files.c_str(),service.c_str(),
          resp.uploadid_.c_str(), hostid.c_str(), c,
          (int) request->files().size() - c);
    }
  } else {
    resp.code_ = 536;
    resp.msg_ = "处理文件失败";
    Log("ProcessError:%d,%s,pagetype=%s,uploadid=%s,hostid=%s,sucess=%d,fail=%d",
        resp.code_, resp.msg_.c_str(), service.c_str(), resp.uploadid_.c_str(),
        hostid.c_str(), 0, (int) request->files().size());
  }

  EXIT_OUT: // 输出
  std::string strOutput;
  std::string json;
  if (channel == "music") {
    json = ToMusicJson(resp);
  } else if (channel == "attachment") {
    json = ToAttachmentJson(resp);
  } else {
    json = ToImageJson(resp);
  }
  strOutput = Output(defaulttype, request->get("callback"), defaultdomain,
                     json);
  response->WriteRaw(strOutput + '\n');
  return HC_OK;
}
bool UploadAction::ParseCropGeom(const std::string& crop_geom_str,
                                 Magick::Geometry* crop_geom) {
  assert(crop_geom);
  int width, height, xoff, yoff;
  int num_elems = sscanf(crop_geom_str.c_str(), "(%d,%d,%d,%d)", &width, &height, &xoff, &yoff);
  if (num_elems != 4) {
    return false;
  }
  if (width <= 0 || height <= 0 || xoff < 0 || yoff < 0) {
    return false;
  }
  crop_geom->width(width);
  crop_geom->height(height);
  crop_geom->xOff(xoff);
  crop_geom->yOff(yoff);
  crop_geom->isValid(true);
  return true;
}
}  // namespace
