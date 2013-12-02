#ifndef FRIENDINFOUTIL_H_
#define FRIENDINFOUtil_H_

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "ObjectCacheI.h"
#include "Util.h"
#include "QueryRunner.h"

namespace xce {
  namespace socialgraph {

    class FriendInfoUtil {
      public:

        static std::map<int,bool> getPhotoStateBatch(const MyUtil::IntSeq& ids);
        static std::map<int,bool> getStateStarBatch(const MyUtil::IntSeq& ids);
        static std::map<int,bool> getOnlineStateBatch(const MyUtil::IntSeq& ids);
        static std::map<int,std::string> getLoginDayState(const MyUtil::IntSeq& ids);
    };


    //***************************************************************************************************************
    
   class UserLoginDaysHandler: public com::xiaonei::xce::ResultHandler {
    public:
      UserLoginDaysHandler(std::map<int, std::string>& userLoginDaysMap):
      userLoginDaysMap_(userLoginDaysMap) {}

      virtual bool handle(mysqlpp::Row& row) const;

    private:
      std::map<int, std::string>& userLoginDaysMap_;
   };
       
  } 
}

#endif
