/*
 * FeedQueueI.h
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#ifndef NAMECHECKI_H_
#define NAMECHECKI_H_

#include "SearchNameCheck.h"
#include "Singleton.h"
#include <IceUtil/IceUtil.h>
#include "SPCheck.h"
#include "ImpersonalChecker.h"
#include "name_entity_prober.h"

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::search::namecheck;

namespace com{
namespace xiaonei{
namespace search{
namespace namecheck{

enum Parameters{
	GRADE_COUNT		=	10,
	GRADE_AVAILABLE	=	3
};

class PageIndexer{
    public : 
	map<string, struPageResult> m_indexer;
	int m_gradation[GRADE_COUNT];
};

class NameCheckerI: public NameChecker, public MyUtil::Singleton<NameCheckerI> {
public:
	virtual bool Check(const string & name,const Ice::Current& = Ice::Current());
	virtual vector<bool> CheckBatch(const vector<string> & name,const Ice::Current& = Ice::Current());

	virtual struNCResult NECheck(const string & name, const Ice::Current& = Ice::Current());
	virtual NCResSeq NECheckBatch(const vector<string> & name,const Ice::Current& = Ice::Current());
	virtual struPageResult pageQuery(const string &queryStr, const Ice::Current& = Ice::Current());
	virtual struInterviewResult interviewQuery(const string &queryStr, const Ice::Current& = Ice::Current());
	virtual void reloadPageIndex(const string &path, const Ice::Current& = Ice::Current());
	virtual queryResult query(const string &queryStr, const Ice::Current&);
    void initialize();

private:
    bool m_isReady;
    SPChecker m_spChecker;
	ImpersonalChecker m_ipChecker;
    NameEntityProber m_neProber;
    set<string> setPlace;
	PageIndexer *m_pPageIndexer, *m_pPageIndexerTemp;
	map<string, int> *m_pPageBuzQuery;
	map<int, struPageResult> *m_pPageIndexById;
	set<int> *m_pPageBuzId;
	map<int, struInterviewResult> m_interviewIndex;
	map<string, int> m_interviewQuery;

protected:
    int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen);
    int gbktoutf8(char *inbuf,int inlen,char *outbuf,int outlen);
    int utf8togbk(char *inbuf,int inlen,char *outbuf,int outlen);
    void UTF8ToGBK(const string& strUTF8,string& strGBK);
    void GBKToUTF8(const string& strGBK,string& strUTF8);
    bool IsName(const string& strName);
    void split(const string& s, char c, vector<string>& v);
	void getPageRank(PageIndexer *workerIndexer, struPageResult &res);
	void initNCResult(struNCResult &res);
	void initPageResult(struPageResult &res);
	bool isIDNumberStr(const string &str);
	bool isEmailStr(const string &str);
	bool isPinyinStr(const string &str);
	void reloadPageBuzQuery(const string &path);
	void reloadInterviewData(const string &path);
	void reloadInterviewQuery(const string &path);
	int str2int(const string &str);
};

}
}
}
}
#endif /* FEEDQUEUEI_H_ */
