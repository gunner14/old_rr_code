#include <fstream>
#include <sstream>
#include <iostream>
#include "base3/logging.h"
#include "runtimeconfig.h"
#include "infconfigparser.h"
#include "util.h"

RuntimeConfig g_runtime_config;

ServiceConf::ServiceConf() :
    maxfilesize_(8 * 1024 * 1024), watermark_(false),
        tickettype_(kTicketCookie), minwidth_(50), minheight_(50),
        handleexif_(true), auto_rotate_(true), checkcomment_(false),
        checkface_(false) {
}

void ServiceConf::MakeDefault() {
  if (exts_.empty()) {
    exts_.insert("jpg");
    exts_.insert("jpeg");
    exts_.insert("png");
    exts_.insert("gif");
    exts_.insert("bmp");
  }

  if (handleimages_.empty()) {
    HandleImageConf hic;
    hic.samepngtype_ = false;
    hic.samegiftype_ = false;
    hic.sharpen_ = false;
    hic.width_ = hic.height_ = 1024;
    hic.nameprefix_ = "xlarge";
    hic.crop_ = false;
    hic.quality_ = 90;
    hic.nginx_cache_ = 5 * 60;
    handleimages_.push_back(hic);

    hic.width_ = hic.height_ = 720;
    hic.nameprefix_ = "large";
    hic.crop_ = false;
    hic.quality_ = 90;
    hic.nginx_cache_ = 30 * 60;
    handleimages_.push_back(hic);

    hic.width_ = hic.height_ = 200;
    hic.nameprefix_ = "main";
    hic.crop_ = false;
    hic.quality_ = 85;
    hic.nginx_cache_ = 60 * 60;
    handleimages_.push_back(hic);

    hic.width_ = hic.height_ = 100;
    hic.nameprefix_ = "head";
    hic.crop_ = false;
    hic.quality_ = 80;
    hic.nginx_cache_ = 1440 * 60;
    handleimages_.push_back(hic);

    hic.width_ = hic.height_ = 50;
    hic.nameprefix_ = "tiny";
    hic.crop_ = true;
    hic.quality_ = 80;
    hic.nginx_cache_ = 720 * 60;
    handleimages_.push_back(hic);
  }

  if (savegroups_.empty()) {
    SaveGroupConf sgc;
    sgc.groupname_ = "fmn046";
    sgc.savenodepath_ = "/bigpool/fmn046";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn047";
    sgc.savenodepath_ = "/bigpool/fmn047";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn048";
    sgc.savenodepath_ = "/bigpool/fmn048";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn049";
    sgc.savenodepath_ = "/bigpool/fmn049";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn050";
    sgc.savenodepath_ = "/bigpool/fmn050";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn051";
    sgc.savenodepath_ = "/bigpool/fmn051";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn052";
    sgc.savenodepath_ = "/bigpool/fmn052";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn053";
    sgc.savenodepath_ = "/bigpool/fmn053";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn054";
    sgc.savenodepath_ = "/bigpool/fmn054";
    savegroups_.push_back(sgc);
    sgc.groupname_ = "fmn055";
    sgc.savenodepath_ = "/bigpool/fmn055";
    savegroups_.push_back(sgc);
  }
}

std::string ServiceConf::FindSavePath(const std::string &url) const {
  return std::string("/bigpool/") + url; // 目前都是bigpool下存储,所以省略从savegroups中查找
}

bool ParseHandleImageConf(const std::string& strLine,
                          vector<HandleImageConf>& vecSaveImageConf) {
  vecSaveImageConf.clear();
  vector < std::string > vecConf;
  Split(strLine, "|", vecConf);
  for (size_t i = 0; i < vecConf.size(); i++) {
    const std::string& strConf = vecConf[i];
    vector < std::string > vecSeg;
    Split(strConf, ",", vecSeg);
    if (vecSeg.size() < 4)
      continue;
    for (size_t k = 0; k < vecSeg.size(); k++)
      vecSeg[k] = Trim(vecSeg[k]);
    HandleImageConf handleimageconf;
    handleimageconf.width_ = atoi(vecSeg[0].c_str());
    handleimageconf.height_ = atoi(vecSeg[1].c_str());
    handleimageconf.nameprefix_ = vecSeg[2];
    handleimageconf.crop_ = bool(atoi(vecSeg[3].c_str()));
    handleimageconf.quality_ = atoi(vecSeg[4].c_str());
    handleimageconf.samepngtype_ = bool(atoi(vecSeg[5].c_str()));
    //LOG(INFO) << "samepngtype:" << vecSeg[5] << "samegiftype:" << vecSeg[6];
    handleimageconf.samegiftype_ = bool(atoi(vecSeg[6].c_str()));
    handleimageconf.nginx_cache_ = atoi(vecSeg[7].c_str()) * 60;
    handleimageconf.sharpen_ = bool(atoi(vecSeg[8].c_str()));
    vecSaveImageConf.push_back(handleimageconf);
  }
  return !vecSaveImageConf.empty();
}

bool ParseSaveGroupConf(const std::string& strLine,
                        vector<SaveGroupConf>& vecSaveGroupConf,
                        vector<SaveGroupConf>& vecGifSaveGroupConf) {
  vecSaveGroupConf.clear();
  vecGifSaveGroupConf.clear();

  vector < std::string > vecConf;
  Split(strLine, "|", vecConf);
  for (size_t i = 0; i < vecConf.size(); i++) {
    const std::string& strConf = vecConf[i];
    vector < std::string > vecSeg;
    Split(strConf, ",", vecSeg);
    if (vecSeg.size() < 2)
      continue;
    for (size_t k = 0; k < vecSeg.size(); k++)
      vecSeg[k] = Trim(vecSeg[k]);
    SaveGroupConf saveGroupConf;
    saveGroupConf.groupname_ = vecSeg[0];
    saveGroupConf.savenodepath_ = vecSeg[1];

    if (vecSeg.size() == 2) {
      vecSaveGroupConf.push_back(saveGroupConf);
    }
    // > 2 and last item is "gif"
    else if (strcasecmp(vecSeg.back().c_str(), "gif") == 0) {
      vecGifSaveGroupConf.push_back(saveGroupConf);
    }
  }
  return vecSaveGroupConf.size() + vecGifSaveGroupConf.size() > 0;
}

TicketType TicketTypeFromString(const std::string &t) {
  if (t == "tmorph")
    return kTicketTMorph;
  else if (t == "urlmorph")
    return kTicketUrlMorph;
  else if (t == "fix")
    return kTicketFix;
  else if (t == "memcache")
    return kTickMemcache;
  else if (t == "none")
    return kTicketNone;
  else if (t == "trans")
    return kTickTrans;
  else
    return kTicketCookie;
}

bool ParserChannel2Service(const CInfConfigParser &channelparser,
                           std::map<std::string, ServiceConf> *ct) {
  if (!ct)
    return false;
  std::map < std::string, std::map<std::string, std::string> > channels;
  bool bOk = channelparser.ParseInfConf("conf/template.conf", channels);
  if (!bOk || channels.empty())
    return false;
  for (std::map<std::string, std::map<std::string, std::string> >::iterator it =
      channels.begin(); it != channels.end(); ++it) {
    //LOG(INFO) << std::string("titel:") << it->first;
    ServiceConf sc;
    sc.channel_ = it->first;
    for (std::map<std::string, std::string>::iterator it1 = it->second.begin();
        it1 != it->second.end(); ++it1) {
      //LOG(INFO) << std::string("key: ") << it1->first << " value: " << it1->second;
      const std::string &k = it1->first;
      const std::string &val = it1->second;
      if (k == "maxfilesize")
        sc.maxfilesize_ = atoi(val.c_str()) * 1024 * 1024; // M -> B
      else if (k == "savegroups") {
        sc.savegroups_.clear();
        sc.gif_savegroups_.clear();
        ParseSaveGroupConf(val, sc.savegroups_, sc.gif_savegroups_);
      } else if (k == "exts") {
        std::set < std::string > exts;
        Split(val, ",", exts);
        sc.exts_.swap(exts);
      } else if (k == "handleimages") {
        sc.handleimages_.clear();
        ParseHandleImageConf(val, sc.handleimages_);
      } else if (k == "minwidth") ///<最小的宽高(width,height)
        sc.minwidth_ = atoi(val.c_str());
      else if (k == "minheight")
        sc.minheight_ = atoi(val.c_str());
    }
    ct->insert(std::make_pair(it->first, sc));
  }
  return true;
}

OutputType OutputFromString(const std::string &t) {
  if (t == "jsonp")
    return kOutputJsonp;
  else if (t == "iframe")
    return kOutputIframe;
  else if (t == "iframe2")
    return kOutputIframe2;
  else if (t == "redirect")
    return kOutputRedirect;
  else if (t == "none")
    return kOutputNone;
  else
    return kOutputJson;
}

bool RuntimeConfig::Load() {
  static CInfConfigParser channelparser, serviceparser;
  std::map<std::string, ServiceConf> channeltmps;
  std::map < std::string, std::map<std::string, std::string> > services;
  if (!ParserChannel2Service(channelparser, &channeltmps)) {
    LOG(ERROR) << "load template.conf fail";
    return false;
  }
  bool bOk = serviceparser.ParseInfConf("conf/service.conf", services);
  if (!bOk || services.empty()) {
    LOG(ERROR) << "load service.conf fail";
    return false;
  }
  std::map<std::string, ServiceConf> newconfs; ///<栏目名到配置的映射

  for (std::map<std::string, std::map<std::string, std::string> >::iterator it =
      services.begin(); it != services.end(); it++) {
    std::map<std::string, std::string>::iterator it1 = it->second.find(
        "channel");
    if (it1 == it->second.end()) {
      LOG(ERROR) << "no set channel on " << it->first;
      return false;
    }

    std::map<std::string, ServiceConf>::iterator findit = channeltmps.find(
        it1->second);
    if (findit == channeltmps.end()) {
      LOG(ERROR) << "no exist channel on template.conf: " << it1->second;
      return false;
    }
    ServiceConf sc = findit->second;

    // 以下可选参数
    it1 = it->second.find("tickettype");
    if (it1 != it->second.end())
      sc.tickettype_ = TicketTypeFromString(it1->second);

    it1 = it->second.find("ticketsuffix");
    if (it1 != it->second.end())
      sc.ticketsuffix_ = it1->second;

    it1 = it->second.find("domain");
    if (it1 != it->second.end())
      sc.domain_ = it1->second;

    it1 = it->second.find("savesubdir");
    if (it1 != it->second.end())
      sc.savesubdir_ = it1->second;

    it1 = it->second.find("watermark");
    if (it1 != it->second.end())
      sc.watermark_ = it1->second == "true";

    it1 = it->second.find("markpath");
    if (it1 != it->second.end())
      sc.markpath_ = it1->second;

    it1 = it->second.find("handleexif");
    if (it1 != it->second.end())
      sc.handleexif_ = it1->second != "false";

    it1 = it->second.find("checkcomment");
    if (it1 != it->second.end())
      sc.checkcomment_ = it1->second == "true";

    it1 = it->second.find("checkface");
    if (it1 != it->second.end())
      sc.checkface_ = it1->second != "false";

    it1 = it->second.find("outputtype");
    if (it1 != it->second.end())
      sc.outputtype_ = OutputFromString(it1->second);

    it1 = it->second.find("autorotate");
    if (it1 != it->second.end()) {
      sc.auto_rotate_ = (it1->second != "false");
    }

    sc.MakeDefault();
    newconfs.insert(std::make_pair(it->first, sc));
  }
  uploadconfs_.swap(newconfs);
  return true;
}

const ServiceConf* RuntimeConfig::GetServiceConf(
    const std::string& service) const {
  std::map<std::string, ServiceConf>::const_iterator itMap = uploadconfs_.find(
      service);
  if (itMap != uploadconfs_.end())
    return &itMap->second;
  else
    return NULL;
}

