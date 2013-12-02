/*
 * UserFeatureData.h
 *
 * Created on: 2012-03-16
 *     Author: lei.yu1
 */

#ifndef AD_USER_FEATURE_DATA_H
#define AD_USER_FEATURE_DATA_H

#include <map>
#include <string>
#include <query.h>
#include <utility>
#include <Ice/Config.h>
#include <connection.h>
#include <IceUtil/Shared.h>

namespace xce{
namespace ad{

class AdUserFeatureData : public IceUtil::Shared {

public:
    AdUserFeatureData(const std::string& fileName, const std::string& md5Value) : valid_(true), md5Value_(md5Value), localFileName_(fileName) {}

    bool LoadData(const std::string& fileName, const std::string& md5Value);

    void InvalidData();
    void Clear() { userFeatureData_.clear(); }

    std::string GetUserFeature(const Ice::Long userID);
    std::string GetFileName() { return localFileName_; }
    std::string GetMd5Value();

    void SetMd5Value(const std::string&);
    void SetFileName(const std::string&);

    bool IsValidFile(const std::string& fileName, const std::string& md5Value);
protected:
    std::string ToString(const std::pair<Ice::Long, std::string>& data);
    std::pair<Ice::Long, std::string> StringSpliter(const std::string& line, const std::string& tag);
    void ComputeMd5(const std::string&, char*);

private:
    void UpdateSQL(const std::string& sql, const std::string& reason);

private:
    bool valid_;
    std::string md5Value_;
    std::string localFileName_;
    std::map<Ice::Long, std::string> userFeatureData_;
    
};// end of class AdUserFeatureData

}//end of namespace ad
}//end of namespace xce

#endif
