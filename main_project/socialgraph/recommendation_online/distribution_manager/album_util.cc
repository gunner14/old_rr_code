/*
 * album_util.cc
 *
 *  Created on: 2012-2-16
 *      Author: Administrator
 *      Copyright: renren-inc
 */

#include "album_util.h"
#include <stdlib.h>
#include <string.h>
#include <tr1/functional>

namespace album_util {

static const GroupName_Domain kGroupName_Domain[] = {
    // storagenode.dict
    { "aps001", "app.xnimg.cn" }, { "cl01", "cl001.file.xiaonei.com" }, {
        "fm01", "fm011.img.xiaonei.com" }, { "fm02", "fm021.img.xiaonei.com" },
    { "fm03", "fm031.img.xiaonei.com" }, { "fm04", "fm041.img.xiaonei.com" }, {
        "fm04", "fm042.img.xiaonei.com" }, { "fm05", "fm051.img.xiaonei.com" },
    { "fm06", "fm062.img.xiaonei.com" }, { "fm06", "fm061.img.xiaonei.com" }, {
        "fm07", "fm071.img.xiaonei.com" }, { "fm08", "fm081.img.xiaonei.com" },
    { "fm09", "fm091.img.xiaonei.com" }, { "fm10", "fm102.img.xiaonei.com" }, {
        "fm10", "fm101.img.xiaonei.com" }, { "fm11", "fm111.img.xiaonei.com" },
    { "fm12", "fm121.img.xiaonei.com" }, { "fm13", "fm131.img.xiaonei.com" }, {
        "fm13", "fm132.img.xiaonei.com" }, { "fm14", "fm141.img.xiaonei.com" },
    { "fm15", "fm151.img.xiaonei.com" }, { "fm16", "fm161.img.xiaonei.com" }, {
        "fm17", "fm171.img.xiaonei.com" }, { "fm18", "fm181.img.xiaonei.com" },
    { "fm19", "fm191.img.xiaonei.com" }, { "fm20", "fm201.img.xiaonei.com" }, {
        "fm21", "fm211.img.xiaonei.com" }, { "fm22", "fm221.img.xiaonei.com" },
    { "fm23", "fm231.img.xiaonei.com" }, { "fm241", "fm192.img.xiaonei.com" }, {
        "fm251", "fm202.img.xiaonei.com" }, { "fm26", "fm261.img.xiaonei.com" },
    { "fm27", "fm271.img.xiaonei.com" }, { "fm28", "fm281.img.xiaonei.com" }, {
        "fm291", "fm291.img.xiaonei.com" },
    { "fm301", "fm301.img.xiaonei.com" }, { "fm311", "fm311.img.xiaonei.com" },
    { "fm321", "fm321.img.xiaonei.com" }, { "fm331", "fm331.img.xiaonei.com" },
    { "fm341", "fm341.img.xiaonei.com" }, { "fm351", "fm351.img.xiaonei.com" },
    { "fm361", "fm361.img.xiaonei.com" }, { "fm371", "fm371.img.xiaonei.com" },
    { "fm381", "fm381.img.xiaonei.com" }, { "fm391", "fm391.img.xiaonei.com" },
    { "fm401", "fm401.img.xiaonei.com" }, { "fm411", "fm411.img.xiaonei.com" },
    { "fm421", "fm421.img.xiaonei.com" }, { "fm431", "fm431.img.xiaonei.com" },
    { "fm441", "fm441.img.xiaonei.com" }, { "fm451", "fm451.img.xiaonei.com" },
    { "fm461", "fm461.img.xiaonei.com" }, { "fm471", "fm471.img.xiaonei.com" },
    { "fm481", "fm481.img.xiaonei.com" }, { "fm491", "fm491.img.xiaonei.com" },
    { "fm501", "fm501.img.xiaonei.com" }, { "fm511", "fm511.img.xiaonei.com" },
    { "fm521", "fm521.img.xiaonei.com" }, { "fm531", "fm531.img.xiaonei.com" },
    { "fm541", "fm541.img.xiaonei.com" }, { "fm551", "fm551.img.xiaonei.com" },
    { "fm561", "fm561.img.xiaonei.com" }, { "fm571", "fm571.img.xiaonei.com" },
    { "fm581", "fm581.img.xiaonei.com" }, { "fm591", "fm591.img.xiaonei.com" },
    { "fm601", "fm601.img.xiaonei.com" }, { "fm611", "fm611.img.xiaonei.com" },
    { "fmn001", "fmn001.xnimg.cn" }, { "fmn002", "fmo.xnimg.cn" }, { "fmn003",
        "fmo.xnimg.cn" }, { "fmn004", "fmo.xnimg.cn" }, { "fmn005",
        "fmo.xnimg.cn" }, { "fmn006", "fmo.xnimg.cn" }, { "fmn007",
        "fmo.xnimg.cn" }, { "fmn008", "fmo.xnimg.cn" }, { "fmn009",
        "fmo.xnimg.cn" }, { "fmn010", "fmo.xnimg.cn" }, { "fmn011",
        "fmo.xnimg.cn" }, { "fmn012", "fmo.xnimg.cn" }, { "fmn013",
        "fmo.xnimg.cn" }, { "fmn014", "fmo.xnimg.cn" }, { "fmn015",
        "fmo.xnimg.cn" }, { "fmn016", "fmo.xnimg.cn" }, { "fmn017",
        "fmo.xnimg.cn" }, { "fmn018", "fmo.xnimg.cn" }, { "fmn019",
        "fmo.xnimg.cn" }, { "fmn020", "fmo.xnimg.cn" }, { "fmn021",
        "fmo.xnimg.cn" }, { "fmn022", "fmo.xnimg.cn" }, { "fmn023",
        "fmo.xnimg.cn" }, { "fmn024", "fmo.xnimg.cn" }, { "fmn025",
        "fmo.xnimg.cn" }, { "fmn026", "fmn.rrimg.com" }, { "fmn027",
        "fmn.rrimg.com" }, { "fmn028", "fmn.rrfmn.com" }, { "fmn029",
        "fmn.xnpic.com" }, { "fmn030", "fmn.xnpic.com" }, { "fmn031",
        "fmn.xnpic.com" }, { "fmn032", "fmn.rrimg.com" }, { "fmn033",
        "fmn.rrimg.com" }, { "fmn034", "fmn.rrimg.com" }, { "fmn035",
        "fmn.rrimg.com" }, { "fmn036", "fmn.rrimg.com" }, { "fmn037",
        "fmn.rrimg.com" }, { "fmn038", "fmn.rrfmn.com" }, { "fmn039",
        "fmn.xnpic.com" }, { "fmn040", "fmn.xnpic.com" }, { "fmn041",
        "fmn.xnpic.com" }, { "fmn042", "fmn.rrimg.com" }, { "fmn043",
        "fmn.rrimg.com" }, { "fmn044", "fmn.rrimg.com" }, { "fmn045",
        "fmn.rrimg.com" }, { "fmn046", "fmn.rrimg.com" }, { "fmn047",
        "fmn.rrimg.com" }, { "fmn048", "fmn.rrfmn.com" }, { "fmn049",
        "fmn.xnpic.com" }, { "fmn050", "fmn.xnpic.com" }, { "fmn051",
        "fmn.xnpic.com" }, { "fmn052", "fmn.xnpic.com" }, { "fmn053",
        "fmn.rrimg.com" }, { "fmn054", "fmn.rrimg.com" }, { "fmn055",
        "fmn.rrimg.com" }, { "fmn056", "fmn.rrimg.com" }, { "fmn057",
        "fmn.rrimg.com" }, { "fmn058", "fmn.rrfmn.com" }, { "fmn059",
        "fmn.rrimg.com" }, { "fmn060", "fmn.rrimg.com" }, { "fmn061",
        "fmn.rrimg.com" }, { "fmn062", "fmn.rrimg.com" }, { "fmn063",
        "fmn.rrimg.com" }, { "fmn064", "fmn.rrimg.com" }, { "fmn065",
        "fmn.rrimg.com" }, { "fmn066", "fmn.rrimg.com" }, { "hd10",
        "hd10.xiaonei.com" }, { "hd11", "hd11.xiaonei.com" }, { "hd12",
        "hd12.xiaonei.com" }, { "hd13", "hd13.xiaonei.com" }, { "hd14",
        "hd14.xiaonei.com" }, { "hd15", "hd15.xiaonei.com" }, { "hd16",
        "hd16.xiaonei.com" }, { "hd17", "hd17.xiaonei.com" }, { "hd18",
        "hd18.xiaonei.com" }, { "hd20", "hd20.xiaonei.com" }, { "hd201",
        "hd201.xiaonei.com" }, { "hd21", "hd21.xiaonei.com" }, { "hd22",
        "hd22.xiaonei.com" }, { "hd23", "hd23.xiaonei.com" }, { "hd24",
        "hd24.xiaonei.com" }, { "hd25", "hd25.xiaonei.com" }, { "hd26",
        "hd26.xiaonei.com" }, { "hd27", "hd27.xiaonei.com" }, { "hd28",
        "hd28.xiaonei.com" }, { "hd29", "hd29.xiaonei.com" }, { "hd30",
        "hd30.xiaonei.com" }, { "hd31", "hd31.xiaonei.com" }, { "hd32",
        "hd32.xiaonei.com" }, { "hd33", "hd33.xiaonei.com" }, { "hd34",
        "hd34.xiaonei.com" }, { "hd35", "hd35.xiaonei.com" }, { "hd36",
        "hd36.xiaonei.com" }, { "hd37", "hd37.xiaonei.com" }, { "hd40",
        "hd40.xiaonei.com" }, { "hd41", "hd41.xiaonei.com" }, { "hd42",
        "hd42.xiaonei.com" }, { "hd43", "hd43.xiaonei.com" }, { "hd44",
        "hd44.xiaonei.com" }, { "hd45", "hd45.xiaonei.com" }, { "hd46",
        "hd46.xiaonei.com" }, { "hd50", "hd50.xiaonei.com" }, { "hd51",
        "hd51.xiaonei.com" }, { "hd52", "hd52.xiaonei.com" }, { "hd53",
        "hd53.xiaonei.com" }, { "hd54", "hd54.xiaonei.com" }, { "hd60",
        "hd60.xiaonei.com" }, { "hd61", "hd61.xiaonei.com" }, { "hd70",
        "hd70.xiaonei.com" }, { "hdn101", "hdn101.xnimg.cn" }, { "hdn111",
        "hdn.xnimg.cn" }, { "hdn121", "hdn.xnimg.cn" }, { "hdn201",
        "hdn.xnimg.cn" }, { "hdn211", "hdn.xnimg.cn" }, { "hdn221",
        "hdn.xnimg.cn" }, { "hdn301", "hdn.xnimg.cn" }, { "hdn311",
        "hdn.xnimg.cn" }, { "hdn321", "hdn.xnimg.cn" }, { "hdn401",
        "hdn.xnimg.cn" }, { "hdn411", "hdn.xnimg.cn" }, { "hdn421",
        "hdn.xnimg.cn" }, { "hdn501", "hdn.xnimg.cn" }, { "hdn511",
        "hdn.xnimg.cn" }, { "hdn521", "hdn.xnimg.cn" }, { "hdn601",
        "hdn.xnimg.cn" }, { "hdn611", "hdn611.xnimg.cn" }, { "kxfmn001",
        "kxfmn001.rrimg.com" }, { "kxhdn101", "kxhdn101.rrimg.com" }, {
        "sales001", "sales001.xnimg.cn" }, { "vip", "joe.xiaonei.com" } };

static bool StrStartswith(const std::string& str, const std::string& prefix) {
  if (prefix.size() > str.size())
    return false;
  return strncmp(str.c_str(), prefix.c_str(), prefix.size()) == 0;
}
Domain::Domain() {
  loadGroups();
}
Domain::~Domain() {
  NameGroupMap::iterator it = name_group_map_.begin();
  for (; it != name_group_map_.end(); ++it) {
    delete it->second;
  }
}
void Domain::loadGroups() {
  size_t numNodes = sizeof(kGroupName_Domain) / sizeof(kGroupName_Domain[0]);
  for (size_t i = 0; i < numNodes; ++i) {
    const std::string group_name(kGroupName_Domain[i].group_name);
    Group* group = getGroup(group_name);
    if (!group) {
      group = new Group(kGroupName_Domain[i].group_name);
      name_group_map_[group_name] = group;
    }
    group->addNode(kGroupName_Domain[i].domain);
  }
}
Group* Domain::getGroup(const std::string& group_name) {
  NameGroupMap::iterator it = name_group_map_.find(group_name);
  return it == name_group_map_.end() ? NULL : it->second;
}
const std::string& Node::domain() const {
  return domain_;
}
Node::Node() {
}
Node::Node(const char* domain)
    : domain_(domain) {
}
Group::Group(const char* name)
    : name_(name) {
}
Group::~Group() {
  for (size_t i = 0; i < nodes_.size(); ++i) {
    delete nodes_[i];
  }
}
void Group::addNode(const char* domain) {
  nodes_.push_back(new Node(domain));
}
const Node* Group::getNode(const std::string& fileName) const {
  size_t hashCode = std::tr1::hash<std::string>()(fileName);
  return nodes_[hashCode % nodes_.size()];
}
const std::string& Group::name() const {
  return name_;
}

Allocator Allocator::self_;
Allocator::Allocator() {
  domain_ = new Domain();
}
Allocator::~Allocator() {
  delete domain_;
}
Allocator& Allocator::getInstance() {
  return self_;
}
AlbumUtil::AlbumUtil() {
}
std::string AlbumUtil::getPhotoURL(const std::string& path) {
  std::string url = Allocator::getInstance().locate(path);
  if (!url.empty())
    return url;
  //
  if (StrStartswith(path, OpiConstants::k5q)) {
    const size_t kFlagStartPos = OpiConstants::k5q.size() + 1;
    const std::string& flag = path.substr(kFlagStartPos);
    const std::string& domain = getPhotoDomain(flag);
    url = domain + "/" + flag;
  } else {
    if (StrStartswith(path, OpiConstants::kPic001)) {
      url = OpiConstants::kUrlPic001 + "/" + path;
    } else if (StrStartswith(path, OpiConstants::kPic002)) {
      url = OpiConstants::kUrlPic002 + "/" + path;
    } else {
      if (!pathStartsWithDate(path, url)) {
        url = OpiConstants::kUrlImg + "/photos/" + path;
      }
    }
  }
}
std::string AlbumUtil::getPhotoDomain(const std::string& flag) {
  if (StrStartswith(flag, "c02")) {
    return OpiConstants::kUrlPhoto2Album;
  } else {
    return OpiConstants::kUrlPhotoAlbum;
  }
}
bool AlbumUtil::pathStartsWithDate(const std::string& path, std::string& url) {
  const size_t kDateLength = 8;
  if (path.size() < kDateLength)
    return false;

  const std::string& date_prefix = path.substr(0, kDateLength);
  const char* date_cstr = date_prefix.c_str();
  char* endptr = NULL;
  int32_t date = strtol(date_cstr, &endptr, 10);
  if (endptr != date_cstr + kDateLength) {
    // convert failed, that's date_prefix is not in the form of 'yyyymmdd'
    return false;
  }
  const int32_t kStartDate = 20070101;
  if (date < kStartDate) {
    return false;
  }
  url = OpiConstants::kUrlHead + "/photos/" + path;
  return true;
}
const std::string OpiConstants::kUrlPhotoAlbum = "http://photos.album.5q.com";
const std::string OpiConstants::kUrlPhoto2Album = "http://photos2.album.5q.com";
const std::string OpiConstants::kUrlHead = "http://head.xiaonei.com";
const std::string OpiConstants::k5q = "5q";
const std::string OpiConstants::kPic001 = "pic001";
const std::string OpiConstants::kPic002 = "pic002";
const std::string OpiConstants::kUrlPic001 = "http://pic.xiaonei.com";
const std::string OpiConstants::kUrlPic002 = "http://pic2.xiaonei.com";
const std::string OpiConstants::kUrlImg = "http://img.xiaonei.com";
const std::string OpiConstants::kKaixinPrefix = "kx/";
const char OpiConstants::kFileSeparator = '/';

std::string Allocator::locate(const std::string& patternIn) {
  // we think the url of this image is like
  // "GROUP/photo/2007xxxx/xxxx.jpg,
  // so if it does not start with GROUP, just return null.

  std::string pattern(patternIn);
  bool isKaixin = false;
  if (StrStartswith(pattern, OpiConstants::kKaixinPrefix)) {
    isKaixin = true;
    pattern = pattern.substr(OpiConstants::kKaixinPrefix.size());
  }
  int pos = pattern.find(OpiConstants::kFileSeparator);
  Group* group = NULL;
  if (pos == -1) {
    group = domain_->getGroup(pattern);
  } else {
    group = domain_->getGroup(pattern.substr(0, pos));
  }
  if (!group)
    return std::string("");

  const std::string& file_name = pattern.substr(pos + 1);
  if (file_name.empty()) {
    printf("storage fileName null [%s]]\n", pattern.c_str());
    return "";
  }

  // check whether we do something wrong that upload the picture to
  // head storage, if so, correct it
  // group::getNode will success anyway
  std::string server_name = group->getNode(file_name)->domain();
  if (isKaixin) {
    size_t pos = server_name.find("xnimg.cn");
    if (pos != std::string::npos) {
      const size_t kXnimgLen = 8;
      server_name = server_name.substr(0, pos) + "rrimg.com"
          + server_name.substr(pos + kXnimgLen);
    }
  }
  const std::string& group_name = group->name();
  if (StrStartswith(group_name, "hd") || StrStartswith(group_name, "kxhd")) {
    return std::string("http://") + server_name + "/photos/" + group_name + "/"
        + file_name;
  } else if (StrStartswith(group_name, "fmn")
      || StrStartswith(group_name, "kxfmn")) {
    return "http://" + server_name + "/" + pattern;
  } else {
    return "http://" + server_name + "/" + file_name;
  }
}

}  // album_util namespace

