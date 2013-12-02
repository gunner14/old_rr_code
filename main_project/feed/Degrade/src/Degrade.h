/*
 * =====================================================================================
 *
 *       Filename:  degrade.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月24日 09时10分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __FEED_DEGRADE_H__
#define __FEED_DEGRADE_H__

#include <string>
#include <vector>
#include <map>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Thread.h>
#include "Singleton.h"
#include <connection.h>
#include <DbDescriptor.h>
#include <QueryRunner.h>
#include "FeedDegradeManager.h"

namespace xce {
namespace feed {
class FeedDegradeBackdoorI : virtual public FeedDegradeBackdoor {
  public:
    FeedDegradeBackdoorI(const Ice::ObjectAdapterPtr& adapter) {
      if(adapter) {
          adapter->add(this, adapter->getCommunicator()->stringToIdentity("FDBD"));
      }
    }
    virtual MyUtil::Str2StrMap control(const MyUtil::Str2StrMap& data, const Ice::Current& current = Ice::Current());

};
typedef IceUtil::Handle<FeedDegradeBackdoorI> FeedDegradeBackdoorIPtr;

class DegradeManager: public IceUtil::Thread, public MyUtil::Singleton<DegradeManager> {

  public:

    DegradeManager():interval_(5), db_source_("feed_degrade"){}

    void setInterval(unsigned interval);
    void SetDBSource(const std::string &source);

    int Init();
    void Register(const std::string &module_name);
    bool IsEnable(const std::string &module_name);
    void Enable(const std::string& module_name);
    void Disable(const std::string& module_name);

    std::map<std::string, std::string> getStatus();
    virtual void run();

  private:

    void Load();
    void LoadModuleStatus();
    void SetModuleStatus(const std::string &module_name, int status);
    int excuteQuery(const com::xiaonei::xce::Statement &sql, 
        const std::string &table,
        mysqlpp::StoreQueryResult & res);

    struct DegradeModule{
      std::string module_name_; 
      bool is_enable_;
    };

    unsigned interval_;
    std::string db_source_;

    std::map<std::string, DegradeModule> modules_;

    FeedDegradeBackdoorIPtr door_;

};

} // end namespace feed
} // end namespace xce

#endif
