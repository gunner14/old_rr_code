#ifndef _UTIL_UPLOAD_H_
#define _UTIL_UPLOAD_H_
#include <string>
#include "upload_head.h"
#include "runtimeconfig.h"
#include "upload/syncdownload.h"


namespace upload {
bool DownImage(const std::string &url, std::string &data);
std::string ToAttachmentJson(const OutResponse &resp);
std::string ToMusicJson(const OutResponse &resp);
std::string ToImageJson(const OutResponse &resp);
std::string Output(OutputType ot, const std::string &callback, const std::string &domain, const std::string &json);
}
#endif
