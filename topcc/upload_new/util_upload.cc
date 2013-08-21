#include "util_upload.h"
#include "base3/escape.h"
#include "base3/stringencode.h"
namespace upload {

bool DownImage(const std::string &url, std::string &data) {
  net::SyncDownload sync;
  std::string head;
  return 200 == sync.Get(url, &data, &head);
}

inline char* itoa(char* strnum, int numb) {
  sprintf(strnum, "%d", numb);
  return strnum;
}
#define ESCAPEJSON(str) base::JsonEscape()(str)

// TODO: mv to attachmenthandler.cc
#define ENTERCHAR(c) 
std::string ToAttachmentJson(const OutResponse &resp) {
  char strnum[32];
  std::vector<OutFile> files;

  std::string json = "{";
  json += "" ENTERCHAR() "\"uploadid\":\"";
  json += base::html_encode(resp.uploadid_);
  json += "\"," ENTERCHAR() "\"code\":";
  json += itoa(strnum, resp.code_);
  json += "," ENTERCHAR() "\"msg\":\"";
  json += resp.msg_;
  json += "\"," ENTERCHAR() "\"files\":[";
  for (std::vector<OutFile>::const_iterator it = resp.files_.begin();
      it != resp.files_.end(); ++it) {
    json += "" ENTERCHAR() "{\"code\":";
    json += itoa(strnum, it->code_);
    json += "," ENTERCHAR() "\"msg\":\"";
    json += it->msg_;
    json += "\"," ENTERCHAR() "\"filename\":\"";
    json += base::html_encode(ESCAPEJSON(it->filename_));
    json += "\"," ENTERCHAR() "\"filesize\":";
    json += itoa(strnum, it->filesize_);
    json += ",";
    int i = it->exifs_.size();
    for (std::map<std::string, std::string>::const_iterator it2 =
        it->exifs_.begin(); it2 != it->exifs_.end(); ++it2, --i) {
      json += "" ENTERCHAR() "\"";
      json += ESCAPEJSON(it2->first);
      json += "\":\"";
      json += ESCAPEJSON(it2->second);
      json += "\",";
    }
    if (!it->lostblocks_.empty()) {
      json += "" ENTERCHAR() "\"lostblocks\":[";
      for (std::vector<int>::const_iterator it3 = it->lostblocks_.begin();
          it3 != it->lostblocks_.end(); ++it3) {
        json += itoa(strnum, *it3);
        if (it3 + 1 != it->lostblocks_.end())
          json += ",";
      }
      json += "],";
    }
    if (!it->images_.empty()) {
      const OutImageUrl &imul = it->images_.front();
      json += "" ENTERCHAR() "\"url\":\"";
      json += ESCAPEJSON(imul.url_);
      json += "\"";
    }
    if (it + 1 != resp.files_.end())
      json += "},";
    else
      json += "}";
  }
  json += "" ENTERCHAR() "]" ENTERCHAR() "}" ENTERCHAR() "";
  //return "{\"uploadid\":\"fu_200435454_12345671234\",\"code\":504,\"msg\":\"����һ������\"}";
  return json;
}
// TODO: mv to musichandler.cc
std::string ToMusicJson(const OutResponse &resp) {
  char strnum[32];
  std::vector<OutFile> files;

  std::string json = "{";
  json += "" ENTERCHAR() "\"uploadid\":\"";
  json += base::html_encode(resp.uploadid_);
  json += "\"," ENTERCHAR() "\"code\":";
  json += itoa(strnum, resp.code_);
  json += "," ENTERCHAR() "\"msg\":\"";
  json += resp.msg_;
  json += "\"," ENTERCHAR() "\"files\":[";
  for (std::vector<OutFile>::const_iterator it = resp.files_.begin();
      it != resp.files_.end(); ++it) {
    json += "" ENTERCHAR() "{\"code\":";
    json += itoa(strnum, it->code_);
    json += "," ENTERCHAR() "\"msg\":\"";
    json += it->msg_;
    json += "\"," ENTERCHAR() "\"filename\":\"";
    json += base::html_encode(ESCAPEJSON(it->filename_));
    json += "\"," ENTERCHAR() "\"filesize\":";
    json += itoa(strnum, it->filesize_);
    json += ",";
    int i = it->exifs_.size();
    for (std::map<std::string, std::string>::const_iterator it2 =
        it->exifs_.begin(); it2 != it->exifs_.end(); ++it2, --i) {
      json += "" ENTERCHAR() "\"";
      json += ESCAPEJSON(it2->first);
      json += "\":\"";
      json += ESCAPEJSON(it2->second);
      json += "\",";
    }
    if (!it->lostblocks_.empty()) {
      json += "" ENTERCHAR() "\"lostblocks\":[";
      for (std::vector<int>::const_iterator it3 = it->lostblocks_.begin();
          it3 != it->lostblocks_.end(); ++it3) {
        json += itoa(strnum, *it3);
        if (it3 + 1 != it->lostblocks_.end())
          json += ",";
      }
      json += "],";
    }
    if (!it->images_.empty()) {
      const OutImageUrl &imul = it->images_.front();
      json += "" ENTERCHAR() "\"url\":\"";
      json += ESCAPEJSON(imul.url_);
      json += "\"";
    }
    if (it + 1 != resp.files_.end())
      json += "},";
    else
      json += "}";
  }
  json += "" ENTERCHAR() "]" ENTERCHAR() "}" ENTERCHAR() "";
  //return "{\"uploadid\":\"fu_200435454_12345671234\",\"code\":504,\"msg\":\"����һ������\"}";
  return json;
}

// TODO: mv to imagehandler.cc
std::string ToImageJson(const OutResponse &resp) {
  char strnum[32];
  std::vector<OutFile> files;

  std::string json = "{";
  json += "" ENTERCHAR() "\"uploadid\":\"";
  json += base::html_encode(resp.uploadid_);
  json += "\"," ENTERCHAR() "\"code\":";
  json += itoa(strnum, resp.code_);
  json += "," ENTERCHAR() "\"msg\":\"";
  json += resp.msg_;
  json += "\"," ENTERCHAR() "\"files\":[";
  for (std::vector<OutFile>::const_iterator it = resp.files_.begin();
      it != resp.files_.end(); ++it) {
    json += "" ENTERCHAR() "{\"code\":";
    json += itoa(strnum, it->code_);
    json += "," ENTERCHAR() "\"msg\":\"";
    json += it->msg_;
    json += "\"," ENTERCHAR() "\"filename\":\"";
    json += base::html_encode(ESCAPEJSON(it->filename_));
    json += "\"," ENTERCHAR() "\"filesize\":";
    json += itoa(strnum, it->filesize_);
    json += "," ENTERCHAR() "\"width\":";
    json += itoa(strnum, it->width_);
    json += "," ENTERCHAR() "\"height\":";
    json += itoa(strnum, it->height_);
    json += ",";

    if (!it->exifs_.empty()) {
      json += "" ENTERCHAR() "\"exifs\":[";
      int i = it->exifs_.size();
      for (std::map<std::string, std::string>::const_iterator it2 =
          it->exifs_.begin(); it2 != it->exifs_.end(); ++it2, --i) {
        json += "" ENTERCHAR() "{\"";
        json += ESCAPEJSON(it2->first);
        json += "\":\"";
        json += ESCAPEJSON(it2->second);
        if (i != 1)
          json += "\"},";
        else
          json += "\"}";
      }
      json += "" ENTERCHAR() "],";
    }
    if (!it->lostblocks_.empty()) {
      json += "" ENTERCHAR() "\"lostblocks\":[";
      for (std::vector<int>::const_iterator it3 = it->lostblocks_.begin();
          it3 != it->lostblocks_.end(); ++it3) {
        json += itoa(strnum, *it3);
        if (it3 + 1 != it->lostblocks_.end())
          json += ",";
      }
      json += "],";
    }
    json += "" ENTERCHAR() "\"images\":[";
    for (std::vector<OutImageUrl>::const_iterator it1 = it->images_.begin();
        it1 != it->images_.end(); ++it1) {
      json += "" ENTERCHAR() "{\"url\":\"";
      json += ESCAPEJSON(it1->url_);
      json += "\"," ENTERCHAR() "\"type\":\"";
      json += it1->nameprefix_;
      json += "\"," ENTERCHAR() "\"width\":";
      json += itoa(strnum, it1->width_);
      json += "," ENTERCHAR() "\"height\":";
      json += itoa(strnum, it1->height_);
      if (it1 + 1 != it->images_.end())
        json += "" ENTERCHAR() "},";
      else
        json += "" ENTERCHAR() "}";
    }
    json += "" ENTERCHAR() "]";
    if (it + 1 != resp.files_.end())
      json += "},";
    else
      json += "}";
  }
  json += "" ENTERCHAR() "]" ENTERCHAR() "}" ENTERCHAR() "";
  //return "{\"uploadid\":\"fu_200435454_12345671234\",\"code\":504,\"msg\":\"����һ������\"}";
  return json;
}

std::string Output(OutputType ot, const std::string &callback,
                   const std::string &domain, const std::string &json) {
  if (ot == kOutputJson) {
    return "Status: 200\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
        + json;
  } else if (ot == kOutputJsonp) {
    return "Status: 200\r\nContent-Type: text/html; charset=utf-8\r\n\r\n" //<script type=\"text/javascript\">\n" 
    + base::html_encode(callback) + "(" + json + ");";
  } else if (ot == kOutputIframe) {
    return "Status: 200\r\nContent-Type: text/html; charset=utf-8\r\n\r\n<script type=\"text/javascript\">\ndocument.domain = \""
        + (domain.empty() ? "renren.com" : domain) + "\";\n"
        + base::html_encode(callback) + "(" + json + ");</script>";
  } else if (ot == kOutputIframe2) {
    return "Status: 200\r\nContent-Type: text/html; charset=utf-8\r\n\r\n<script type=\"text/javascript\">\nwindow.name = \'"
        + json + "\';\nlocation.href=\"" + base::html_encode(callback)
        + "\";</script>";
  } else if (ot == kOutputRedirect) {
    return "Status: 302 " + base::html_encode(callback) + "?json=" + json
        + "\r\n";
  }
}

}
