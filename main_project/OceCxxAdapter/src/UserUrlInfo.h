#ifndef XCE_ADAPTER_USERURLINFO_
#define XCE_ADAPTER_USERURLINFO_

#include "UserBase.h"
#include "Markable.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userurl {

using namespace xce::userbase;
using namespace MyUtil;

class UserUrlInfo;
typedef IceUtil::Handle<UserUrlInfo> UserUrlInfoPtr;

class UserUrlInfo : virtual public Ice::Object, public MyUtil::Markable {
  public:
    UserUrlInfoPtr parse(const UserUrlDataNPtr& data) {
      id_ = data->id;
      largeUrl_ = data->largeUrl;
      mainUrl_ = data->mainUrl;
      headUrl_ = data->headUrl;
      tinyUrl_ = data->tinyUrl;
      flexUrl_ = data->flexUrl;
      return this;
    }
    //为UserUrlReaderAdapter编译通过而存在，实际没有用到
    //UserUrlReaderAdapter弃用后此方法可废弃
    UserUrlInfoPtr parse(const UserUrlDataPtr& data) {
      id_ = data->id;
      largeUrl_ = data->largeUrl;
      mainUrl_ = data->mainUrl;
      headUrl_ = data->headUrl;
      tinyUrl_ = data->tinyUrl;
      flexUrl_ = "";
      return this;
    }
    std::string to_string() {
      std::ostringstream oss;
      oss << "UserUrlInfo:[";
      oss << "id:" << id_;
      oss << ",largeurl:" << largeUrl_;
      oss << ",mainurl:" << mainUrl_;
      oss << ",headurl:" << headUrl_;
      oss << ",tinyurl:" << tinyUrl_;
      oss << ",flexurl:" << flexUrl_;
      oss << "]";
      return oss.str();
    }
    int id() const {return id_;}
    std::string largeUrl() const {return largeUrl_;}
    std::string mainUrl() const {return mainUrl_;}
    std::string headUrl() const {return headUrl_;}
    std::string tinyUrl() const {return tinyUrl_;}
    std::string flexUrl() const {return flexUrl_;}
    void set_id(int id) {id_ = id;mark("ID", boost::lexical_cast<std::string>(id));}
    void set_largeUrl(const std::string& largeUrl) {largeUrl_ = largeUrl; mark("LARGEURL", largeUrl);}
    void set_tinyUrl(const std::string& tinyUrl) {tinyUrl_ = tinyUrl; mark("TINYURL", tinyUrl);}
    void set_headUrl(const std::string& headUrl) {headUrl_ = headUrl; mark("HEADURL", headUrl);}
    void set_mainUrl(const std::string& mainUrl) {mainUrl_ = mainUrl; mark("MAINURL", mainUrl);}
    void set_flexUrl(const std::string& flexUrl) {flexUrl_ = flexUrl; mark("FLEXURL", flexUrl);}
  private:
    int id_;
    std::string largeUrl_;
    std::string mainUrl_;
    std::string headUrl_;
    std::string tinyUrl_;
    std::string flexUrl_;
};

}}} //namespace xce::adapter::userurl
#endif
