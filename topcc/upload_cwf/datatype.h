#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;

#include <json/json.h>
#include "cwf/stream.h"
/**保存文件名命名示例
* 普通相册
* http://fmn043.xnimg.cn/fmn043/20090422/1860/p_large_AZUH_7958p204238.jpg
* http://fmn043.xnimg.cn/fmn043/20090422/1860/p_main_karO_7958p204238.jpg
* http://fmn043.xnimg.cn/fmn043/20090422/1860/p_head_zhC7_7958p204238.jpg
* http://fmn043.xnimg.cn/fmn043/20090422/1860/p_tiny_auB9_7958p204238.jpg
* 头像
* http://hdn511.xnimg.cn/photos/hdn511/20090422/1860/s_large_AZUH_7958p204238.jpg
* http://hdn511.xnimg.cn/photos/hdn511/20090422/1860/s_main_karO_7958p204238.jpg
* http://hdn511.xnimg.cn/photos/hdn511/20090422/1860/s_head_zhC7_7958p204238.jpg
* http://hdn511.xnimg.cn/photos/hdn511/20090422/1860/s_tiny_auB9_7958p204238.jpg
* 日志
* http://fmn043.xnimg.cn/fmn043/blog/20090422/1860/b_AZUH_7958p204238.jpg
* 群组
* http://fmn043.xnimg.cn/fmn043/group/20090422/1860/g_AZUH_7958p204238.jpg
* http://fmn043.xnimg.cn/fmn043/group/20090422/1860/g_AZUH_7958p204238.doc
* 班级
* http://fmn043.xnimg.cn/fmn043/class/20090422/1860/c_AZUH_7958p204238.jpg
*/

///存储节点配置
struct TSaveGroupConf
{
  string strGroupName;        ///<组名,dbpath必须依此开头
  string strSaveNodePath;        ///<保存节点路径,filepath必须依次开头,注:节点目前仅支持1个节点
};

///存储图片配置
struct TSaveImageConf {
  TSaveImageConf(){
    nWidth=0;
    nHeight=0;
    bCrop=false;
    quality=90;         ///默认保存质量为90
  }

  int nWidth;          ///<宽,如为0则参考另一边等比例缩放
  int nHeight;          ///<高,如为0则参考另一边等比例缩放
  string strNamePrefix;        ///<保存文件名的前缀
  bool bCrop;          ///<是否裁剪
  int quality;                                    ///保存图片的质量
};

///每种上传的配置
struct TUploadConf
{
  string strChannel;        ///<上传id,惟一:如album,head,class_album,blog_pic等
  string strBaseChannel;        ///<基类频道
  string strExts;          ///<支持的扩展名
  set<string> setExt;        ///<支持的扩展名集合
  size_t nFileSizeLimit;        ///<文件大小上限单位是字节
  string strIsLoginUrl;        ///<验证是否登录的url
  string strAfterUrl;
  vector<TSaveGroupConf> vecSaveGroupConf;  ///<存储组配置集合(保存时随机选取1个)
  string strSaveSubDir;        ///<保存子目录(相对于TSaveImageConf.strSaveNodePath)
  string strSaveNamePrefix;      ///<保存文件名前缀(如vecSaveImageConf非空,则后面应该是TSaveImageConf.strNamePrefix)
  
  ///图片特有
  vector<TSaveImageConf> vecSaveImageConf;  ///<存储图片配置集合
  pair<int,int> pairMinSize;      ///<最小的宽高(width,height)
  pair<float,float> pairSizeRate;      ///<宽高比范围(minrate,maxrate)

};

/*
///html表单提交的一个文件
struct TFormFile
{
  string strName;      ///<file在form中的字段名
  string strFileName;    ///<文件名
  string strContentType;    ///<文件内容类型
  string strFileData;    ///<文件数据
};
*/
typedef cwf::Request::FormDisposition TFormFile;
struct TRequest{
  TRequest(){
    Init(NULL, NULL);
  }
  void Init(cwf::Request *req, const TUploadConf* conf);
  
  string Env(const string& key)const;
  string Env()const;
  const string& Query(const string& key)const;
  string Query()const;
  string Cookie(const string& key)const;
  string Cookie()const;
  std::vector<TFormFile> &files()const;
  
  cwf::Request *req_;
  const TUploadConf* pUploadConf;    ///<适合此请求的上传配置
};
///每个文件的返回信息
struct TFileResponse
{
  TFileResponse(){
    nCode=1;      ///<1 表示成功,其他的未定义,目前均表示失败,默认成功
    nFileSize=0;
  }
  void ToJson(Json::Value & val,bool bForSaveDb=true)const;
  string ToJson(bool bForSaveDb=true)const;
  bool  FromJson(const Json::Value& val);
  bool  FromJson(const string& strJson);

  /*
   * nCode Description:
   *  1 正确
   *  2 文件后缀名错误，必须为gif,bmp,png,jpg,jpeg
   *  3 文件大小限制
   *  4 不是图片，格式问题
   *  5 图片最小尺寸问题
   *  6 图片宽高比问题
   *  7服务器读图错误
   *  7服务器缩图错误
   *  7服务器保存图片错误
   *  7服务器其他异常
   *  8 java验证登陆错误
   *  9 java保存数据库错误
   *  0 未知错误
   * */
  
  int nCode;        ///<返回状态,是否成功处理
  string strMsg;        ///<返回消息
  uint32_t nFileSize;      ///<文件大小
  string strFileName;
  map<string,string>mapExif;    ///<exif信息,一般来说jpg图片才会有有的
  map<string,string>mapOtherMsg;    ///<其他的返回消息
};
struct TResponse
{
  TResponse(){
    Init();
  }
  void Init();

  //bForSaveDb为true表示以保存db为目的,信息量会很多;否则只有少量的信息
  void ToJson(Json::Value& val, bool bForSaveDb=true) const;
  string ToJson(bool bForSaveDb=true) const;
  bool FromJson(const string& strJson);
  bool FromJson(const Json::Value& val);
  bool Output(const TRequest& request, string& strOutput);

  ///以下数据参与json编码  
  uint16_t nCode;
  string strMsg;
  uint16_t nTotalCount;      ///<仅在datatype.cpp中赋值
  uint16_t nErrorCount;      ///<仅在datatype.cpp中赋值
  vector<TFileResponse> vecFileResponse;  ///<每个文件的返回消息

  ///请求afterurl的返回结果
  bool bHaveSaveDb;
  int nAfterHttpCode;
  string strAfterResBody;
  string strAfterResHeader;

};

#endif
