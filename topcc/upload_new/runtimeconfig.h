#ifndef _RUNTIME_CONFIG_H_
#define _RUNTIME_CONFIG_H_
#include <string>
#include <vector>
#include <set>
#include <map>

struct SaveGroupConf {
  std::string groupname_; ///<组名,dbpath必须依此开头
  std::string savenodepath_; ///<保存节点路径,filepath必须依次开头,注:节点目前仅支持1个节点
};

///存储图片配置
struct HandleImageConf {
  HandleImageConf() {
  }

  int width_; ///<宽,如为0则参考另一边等比例缩放
  int height_; ///<高,如为0则参考另一边等比例缩放
  std::string nameprefix_; ///<保存文件名的前缀
  bool crop_; ///<是否裁剪
  int quality_; ///保存图片的质量
  int nginx_cache_; ///保存到nginx-cache中的时间,秒为单位
  bool samepngtype_; /// 保持png不转jpg
  bool samegiftype_; /// 保持gif不转jpg
  bool sharpen_; /// 锐化否
};

enum TicketType {
  kTicketNone,
  kTicketCookie,
  kTicketTMorph,
  kTicketUrlMorph,
  kTicketFix,
  kTickMemcache,
  kTickTrans,
};

enum OutputType {
  kOutputNone,
  kOutputJson,
  kOutputJsonp,
  kOutputIframe,
  kOutputIframe2,
  kOutputRedirect,
};

///每种上传的配置
struct ServiceConf {
  ServiceConf();
  void MakeDefault();
  std::string FindSavePath(const std::string &url) const;

  std::string channel_; ///对应频道类型
  size_t maxfilesize_; ///<文件大小上限单位是字节
  std::vector<SaveGroupConf> savegroups_; ///<存储组配置集合(保存时随机选取1个)
  std::vector<SaveGroupConf> gif_savegroups_; ///<save gif in special save group
  std::string savesubdir_; ///<保存子目录(相对于TSaveImageConf.strSaveNodePath)
  std::string savenameprefix_; /// 默认是p
  bool checkcomment_; ///是否检查comment字段,查看是否盗用
  bool handleexif_;
  bool auto_rotate_; /// 是否根据exif信息自动旋转
  OutputType outputtype_;
  std::string domain_;
  //std::string jsonp_call; // 在网页上传递

  // 水印相关
  bool watermark_;
  std::string markpath_;

  ///票相关
  TicketType tickettype_;
  std::string ticketsuffix_;

  ///图片特有
  std::set<std::string> exts_; ///<支持的扩展名集合
  std::vector<HandleImageConf> handleimages_; ///<存储图片配置集合
  int minwidth_; ///<最小的宽高(width,height)
  int minheight_;
  int checkface_;
};

class RuntimeConfig {
public:
  bool Load();

  ///<获取ServiceConf
  const ServiceConf* GetServiceConf(const std::string& service) const;
  const std::map<std::string, ServiceConf>& uploadconfs() const {
    return uploadconfs_;
  }
private:
  std::map<std::string, ServiceConf> uploadconfs_; ///<栏目名到配置的映射
};

extern RuntimeConfig g_runtime_config;
#endif
