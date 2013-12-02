#ifndef GetUbb_h_
#define GetUbb_h_

#include "Singleton.h"
#include "TaskManager.h"
#include "xoa2/client/client_pool.h"
#include "xoa2/client/xoa_client_factory.h"
#include "UbbServiceXoa2Adapter.h"
#include "util/cpp/String.h"
#include <IceUtil/IceUtil.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/RWRecMutex.h>

namespace com {
namespace xiaonei {
namespace talk {
namespace Ubb {
using namespace com::renren::xoa2::ugc::status;
using namespace xoa2::client;
using namespace MyUtil;
class GetUbb : public Singleton<GetUbb> {
  public:
    Xoa2UbbListResult result;
    bool flag ;
    IceUtil::RWRecMutex _mutex;
    void init() {
      flag=false;
    }
    bool judge() {
      return flag;
    }
    bool has_errorInfo() {
      return result.errorInfo.__isset.msg==false;
    }
    void getData(xml_node& content) {
      IceUtil::RWRecMutex::RLock lock(_mutex);
      xml_node group = content.append_child();
      group.set_name("group");
      group.append_attribute("name") = "group1";
      ostringstream os;
      os << "group1";
      string url_header = "http://a.xnimg.cn";
      for (int i = 0 ;i < result.xoa2UbbList.size();++i) {
        xml_node itemNode = group.append_child();
        itemNode.set_name("face");
        ostringstream ubb; 
        ubb << result.xoa2UbbList[i].ubb;
        os << result.xoa2UbbList[i].ubb ;
        itemNode.append_attribute("ubb") = ubb.str().c_str();
        ostringstream tip;
        tip << result.xoa2UbbList[i].alt;
        os << result.xoa2UbbList[i].alt;
        itemNode.append_attribute("tip")=tip.str().c_str();
        ostringstream url; 
        url << url_header << result.xoa2UbbList[i].src;
        os << url_header << result.xoa2UbbList[i].src;
        itemNode.append_attribute("url") = url.str().c_str();
      }    
      content.append_attribute("ver") = MyUtil::StrUtil::md5(os.str()).c_str();
    }
    void startTasks() {
      TaskManager::instance().scheduleRepeated(new ClientGetUbbTimer);
    }
    class ClientGetUbbTimer : public MyUtil::Timer {
      public:
        ClientGetUbbTimer() : Timer(60*1000) {}
        virtual void handle() {
          IceUtil::RWRecMutex::WLock lock(GetUbb::instance()._mutex);
          GetUbb::instance().flag = true;
          try {
            UbbServiceAdapter::instance().getDoingUbbsBytypeNE(GetUbb::instance().result,0);
          } catch (TTransportException& e) {
            MCE_WARN("GetUbb-->getDoingUbbsBytypeNE-->error!"<<e.what()
                <<"file:"<<__FILE__<<"line:"<<__LINE__);
            GetUbb::instance().flag = false;
          } catch (std::exception& e) {
            MCE_WARN("GetUbb-->getDoingUbbsBytypeNE-->error!"<<e.what()
                <<"file:"<<__FILE__<<"line:"<<__LINE__);
            GetUbb::instance().flag = false;
          } catch (...) {
            MCE_WARN("GetUbb-->getDoingUbbsBytypeNE-->error!"<<"file:"<<__FILE__<<"line:"<<__LINE__);
            GetUbb::instance().flag = false;
          }
        }
    };
};
};
};
};
};
#endif
