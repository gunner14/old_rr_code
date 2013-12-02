/*
 * KeywordExtract.h
 *
 *  Created on: 2012-11-27
 *      Author: dairui.cui
 */

#ifndef KEYWORDEXTRACT_H_
#define KEYWORDEXTRACT_H_

#include "SearchKeywordExtract.h"
#include "Singleton.h"
#include <IceUtil/IceUtil.h>
#include <map>

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::search::keywordextract;

namespace com{
namespace xiaonei{
namespace search{
namespace keywordextract{

class KeywordExtracterI: public KeywordExtracter, public MyUtil::Singleton<KeywordExtracterI> {
public:
    void initialize();
    void reloadKeyword(const string& path,const Ice::Current& = Ice::Current());
    virtual KwResSeq getKeywords(const string& inputStr,const Ice::Current& = Ice::Current());

private:
    std::map<string,float>* m_curMapScore;
    std::map<string,float>* m_tempMapScore;
    bool m_bIsReady;

protected:
};

}
}
}
}
#endif /* KEYWORDEXTRACT_H_ */
