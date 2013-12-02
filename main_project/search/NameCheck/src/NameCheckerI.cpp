#include "NameCheckerI.h"
#include "LogWrapper.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "QueryRunner.h"
#include <stdlib.h>
#include <iconv.h>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/TimeCost.h"

#include "StringNormalize.h"
#include "../simple_segment/segment_const.h"
#include "../simple_segment/segment_utility.h"
#include "../simple_index/utility.h"

using namespace segment;
using namespace com::xiaonei::search::namecheck;
using namespace com::renren::search::utility;
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;

static const int __MAX_WORD_LEN = 1024;

void MyUtil::initialize() {
    NameCheckerI::instance().initialize();
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&NameCheckerI::instance(), service.createIdentity("M",""));//ice, enable the servant NameCheckerI
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt( "NameCheck.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault( "NameCheck.Interval", 5);
//	xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerNameCheck", &NameCheckerI::instance(), mod, interval,new SearchChannel());//
	xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerNameCheck", &NameCheckerI::instance(), mod, interval,new GadgetsChannel());//
}

static const char SCHOOL_TOKEN_FLAG[] = "/s";
static const char NAME_TOKEN_FLAG[] = "/n";
static const char COMPANY_TOKEN_FLAG[] = "/c";
static const char PLACE_TOKEN_FLAG[] = "/p";
static const char NAMELIKE_TOKEN_FLAG[] = "/l";
static const char OTHER_TOKEN_FLAG[] = "/o";
static const char ID_TOKEN_FLAG[] = "/d";
static const char EMAIL_TOKEN_FLAG[] = "/e";
static const char PINYIN_TOKEN_FLAG[] = "/y";
static const int  HEADCOUNT = 3;

void NameCheckerI::initialize()
{
    m_isReady = false;
	MCE_INFO("NameCheckerI::start service.");
    m_spChecker.SetWorkPath("/data/xce/Gadgets/etc/classes/name_check_config/dict");
    m_ipChecker.SetWorkPath("/data/xce/Gadgets/etc/classes/name_check_config/dict");
//    m_spChecker.SetWorkPath("/data/xce/XiaoNeiSearch/etc/classes/name_check_config/dict");
//    m_ipChecker.SetWorkPath("/data/xce/XiaoNeiSearch/etc/classes/name_check_config/dict");
    
	m_spChecker.Init();
	m_ipChecker.Init();

    string conf_file = "/data/xce/Gadgets/etc/classes/name_check_config/config/segment.conf";
//    string conf_file = "/data/xce/XiaoNeiSearch/etc/classes/name_check_config/config/segment.conf";
    if( !m_neProber.init(conf_file.c_str()) )
    {
        MCE_INFO("NameCheckerI::Check loadConfig False!" << conf_file);
        return;
    }
    
    set<string> setUtf8;
    setUtf8.insert("白山");
    setUtf8.insert("沈阳");
    setUtf8.insert("马鞍山");
    setUtf8.insert("黄山");
    setUtf8.insert("金华");
    setUtf8.insert("黄石");
    setUtf8.insert("岳阳");
    setUtf8.insert("邵阳");
    setUtf8.insert("金昌");
    setUtf8.insert("许昌");
    setUtf8.insert("石家庄");
    setUtf8.insert("唐山");
    setUtf8.insert("吕梁");
    setUtf8.insert("吴忠");
    setUtf8.insert("江西");

    string strDest;
    for(std::set<string>::iterator iset = setUtf8.begin();iset != setUtf8.end();iset++)
    {
        UTF8ToGBK(*iset,strDest);
        setPlace.insert(strDest);
    }
    MCE_INFO("NameCheck::NameCheck load finished.");
	m_pPageIndexer = m_pPageIndexerTemp = NULL;
	reloadPageIndex("/data/xce/PageInfo/current");
    MCE_INFO("NameCheck::Page load finished.");
	reloadInterviewData("/data/xce/InterviewInfo/interviewInfo.txt");
	reloadInterviewQuery("/data/xce/InterviewInfo/interviewQuery.txt");
	MCE_INFO("NameCheck::Interview load finished.");
	m_isReady = true;
}

bool NameCheckerI::Check(const string &name,const Ice::Current&)
{
    string strGBK;
    UTF8ToGBK(name,strGBK);
    //MCE_INFO("the input string is:" << strGBK);
    bool bIsName = IsName(strGBK);
    //MCE_INFO("the NameCheck result is " << bIsName);

    return bIsName;
}

vector<bool> NameCheckerI::CheckBatch(const vector<string> & lines_vec, const Ice::Current&)
{ 
	vector<bool> res;
    bool bCurFlag = false;
    string strGBK;
    int nSize = lines_vec.size();
    for(int j = 0;j < nSize ;j++)
    {
        UTF8ToGBK(lines_vec[j],strGBK);
        bCurFlag = IsName(strGBK);
        res.push_back(bCurFlag);
    }
    return res;
}

//代码转换:从一种编码转为另一种编码 
int NameCheckerI::code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen) 
{ 
    iconv_t cd; 
    char **pin = &inbuf; 
    char **pout = &outbuf; 

    cd = iconv_open(to_charset,from_charset); 
    if (cd==0){ 
        iconv_close(cd);
	    return -1;
	}
    memset(outbuf,0,outlen); 
    if (iconv(cd,(char**)pin,&inlen,pout,&outlen)==-1) {
	    iconv_close(cd);
        return -1; 
	}
    iconv_close(cd); 
    return 0; 
}

//GB2312码转为UTF-8码
int NameCheckerI::gbktoutf8(char *inbuf,int inlen,char *outbuf,int outlen) 
{ 
    return code_convert("gbk","utf-8",inbuf,inlen,outbuf,outlen); 
}

//UTF-8码转为GB2312码 
int NameCheckerI::utf8togbk(char *inbuf,int inlen,char *outbuf,int outlen) 
{ 
    return code_convert("utf-8","gbk",inbuf,inlen,outbuf,outlen); 
}

void NameCheckerI::UTF8ToGBK(const string& strUTF8,string& strGBK)
{
    char szDestUTF8[__MAX_WORD_LEN];
    char szDestGBK[__MAX_WORD_LEN];
    memset(szDestGBK,0,__MAX_WORD_LEN);
    memset(szDestUTF8,0,__MAX_WORD_LEN);
    memcpy(szDestUTF8,strUTF8.c_str(),strUTF8.length());
    utf8togbk(szDestUTF8,strlen(szDestUTF8),szDestGBK,__MAX_WORD_LEN);
    strGBK = szDestGBK;
}

void NameCheckerI::GBKToUTF8(const string& strGBK,string& strUTF8)
{
    char szDestUTF8[__MAX_WORD_LEN];
    char szDestGBK[__MAX_WORD_LEN];
    memset(szDestGBK,0,__MAX_WORD_LEN);
    memset(szDestUTF8,0,__MAX_WORD_LEN);
    memcpy(szDestGBK,strGBK.c_str(),strGBK.length());
    gbktoutf8(szDestGBK,strlen(szDestGBK),szDestUTF8,__MAX_WORD_LEN);
    strUTF8 = szDestUTF8;
}

bool NameCheckerI::IsName(const string& name)
{
	/*if (setPlace.count(name) == 1 )
    {
        MCE_INFO("NameCheckerI::Check the string is a place::"<<name);
        return false;
	}*/

	wordAry ary ;
    wordAry result;
    size_t start_idx = 0;
    if( name.empty() )
    {
        MCE_INFO("NameCheckerI::Check the name is NULL!");
        return false;
    }
    //////////////////////
    char szBuf[1024];
    memset(szBuf,0,1024);
    int nLen = name.length();
    memcpy(szBuf,name.c_str(),nLen);
    szBuf[nLen] = '\0';
    char* szTmp = szBuf;
    for (; *szTmp!='\0'; szTmp+=2)
    {
        wordItem tmpWordItem ;
        tmpWordItem.length = 2;
        tmpWordItem.ptr_wd = szTmp;
        ary.push_back(tmpWordItem);
    }
    //////////////////////

    int _nsize = ary.size();
    
    if(_nsize > 3)
    {
        return false;
    }

    if( !m_neProber.probeNameEntity( ary , start_idx , result ))
    {
        ;
    }
    //for( int i=0;i<_nsize;i++)
    //{
    //    if( !m_neProber.probeNameEntity( ary , start_idx , result ))
     //   {
     //       if(start_idx<ary.size()-1)
     //            start_idx = start_idx+1;
     //       else 
     //           break;
     //   }
    //}
    if ( result.size() == 0)
    {
        //MCE_INFO("NameCheckerI::Check result is not a name"<< name);
        return false;
    }
    //tc.step("FinishTime: ");
    return true;
}

void NameCheckerI::split(const string& s, char c, vector<string>& v)
{
    string::size_type i = 0;
    string::size_type j = s.find(c); 
    while(j != string::npos)
    { 
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);
    }//find start from position j
	v.push_back(s.substr(i));
}

struNCResult NameCheckerI::NECheck(const string &strInput,const Ice::Current&)
{
    struNCResult sr;
    int nHeadCount = 8;
    string strGBK;
    UTF8ToGBK(strInput,strGBK);
    string szResult = "";
    vector<string> vecSrc;
    string strQuery = strGBK; 
    split(strQuery, ' ',vecSrc);
    vector<string> vecToken;
    m_spChecker.splitSentence(vecToken,vecSrc);
    int nSize = vecToken.size();
    bool bOther = false;
    bool bSchool = false;
    bool bPlace = false;
    bool bName = false;
	bool bNameLike = false;
	bool bID = isIDNumberStr(strInput);
	bool bEmail = isEmailStr(strInput);
	bool bPinyin = isPinyinStr(strInput);

	if (bID)
	{
		sr.nHeadCount = HEADCOUNT;
		sr.strResult = strInput + ID_TOKEN_FLAG + " ";
		return sr;
	}
	
	if (bEmail)
	{
		sr.nHeadCount = HEADCOUNT;
		sr.strResult = strInput + EMAIL_TOKEN_FLAG + " ";
		return sr;
	}

	if (bPinyin)
	{
		sr.nHeadCount = HEADCOUNT;
		sr.strResult = strInput + PINYIN_TOKEN_FLAG + " ";
		return sr;
	}

    for(int i = 0;i < nSize;i++)
    {
       string szToken = vecToken[i];
	   string GBKTemp, UTFTemp;
       if (szToken == " ")
       {
           continue;
       }
       string szSchool;
       string szAppendix;
       bool bIsSchool = m_spChecker.IsSchool(szToken,szSchool,szAppendix);
       bool bIsPlaceName = false;
       bool bIsName = false;
       bool bIsNameLike = false;
	   if(true == bIsSchool)
       {
           bSchool = true;
           if(szAppendix.length() > 0)
           {
               szResult += szSchool;
               szResult += SCHOOL_TOKEN_FLAG;
               szResult += " ";

			   bIsName = IsName(szAppendix);
			   GBKTemp = szAppendix;
               GBKToUTF8(GBKTemp, UTFTemp);
			   MCE_INFO("Appendix: " << UTFTemp);
               bIsNameLike = !m_ipChecker.isImpersonal(UTFTemp);
			   
			   if (true == bIsName)
			   {
			       szResult += szAppendix;
				   szResult += NAME_TOKEN_FLAG;
				   szResult += " ";
				   bName = true;
			   }

			   else if (true == bIsNameLike)
			   {
			       szResult += szAppendix;
				   szResult += NAMELIKE_TOKEN_FLAG;
				   szResult += " ";
				   bNameLike = true;
			   }

			   else 
			   {
			       szResult += szAppendix;
                   szResult += OTHER_TOKEN_FLAG;
                   szResult += " ";
                   bOther = true;
			   }
           }
           else
           {
               szResult += szToken;
               szResult += SCHOOL_TOKEN_FLAG;
               szResult += " ";
           }
       }
       else
       {
//		   MCE_INFO("szToken: " << szTokenUTF << "| strInput:" << strInput);
           bIsPlaceName = m_spChecker.IsPlaceName(szToken);
           bIsName = IsName(szToken);
		   GBKTemp = szToken;
		   GBKToUTF8(GBKTemp, UTFTemp);
		   MCE_INFO("szToken: " << UTFTemp);
           bIsNameLike = !m_ipChecker.isImpersonal(UTFTemp);
           if(true == bIsPlaceName)
           {
               bPlace = true;
               szResult += szToken;
               szResult += PLACE_TOKEN_FLAG;
               if(true == bIsName)
               {
                   szResult += NAME_TOKEN_FLAG;
                   bName = true;
               }
               szResult += " ";
           }
           else
           {
               if(true == bIsName)
               {
                   bName = true;
                   szResult += szToken;
                   szResult += NAME_TOKEN_FLAG;
                   szResult += " ";
               }
			   else if (true == bIsNameLike)
			   {
			       bNameLike = true;
				   szResult += szToken;
				   szResult += NAMELIKE_TOKEN_FLAG;
				   szResult += " ";
			   }
           }
       }
       if(!(bIsSchool || bIsPlaceName || bIsName || bIsNameLike))
       {
           szResult += szToken;
           szResult += OTHER_TOKEN_FLAG;
           szResult += " ";
           bOther = true;
       }

    }
    szResult = szResult.substr(0,szResult.length() - 1);
    string strUtfR;
    GBKToUTF8(szResult,strUtfR);
	
	MCE_INFO("Query:'" << strInput << "' bName:" << bName << " bNameLike:" << bNameLike << " bSchool:" << bSchool << " bPlace:" << bPlace << " bOther:" << bOther);

    if (bOther && !bName) nHeadCount = 0;
	else if (!bOther && bNameLike && !bName && !bSchool && !bPlace) nHeadCount = HEADCOUNT;
	else if ((!bOther && bNameLike && (bSchool || bPlace))|| 
	         (!bOther && bName)) nHeadCount = HEADCOUNT * 3;
	else nHeadCount = HEADCOUNT * 2;
	
	sr.nHeadCount = nHeadCount;
    sr.strResult = strUtfR;
    return sr;
}

bool NameCheckerI::isIDNumberStr(const string &str){
	if (str.length() > 11 || str.length() < 3) return false;
	for (int i=0; i<str.length(); ++i) if (str[i] < '0' || str[i] > '9') return false;
	return true;
}

bool NameCheckerI::isEmailStr(const string &str){
	int at_cnt = 0;
	bool last_is_punct = false;
	int last_part_len = 0;
	int dot_cnt_after_at = 0;
	for (int i=0; i<str.length(); ++i){
		if (str[i] == '@') {
			at_cnt++;
			if (at_cnt > 1) return false;
			last_is_punct = true;
			dot_cnt_after_at = 0;
		} else 
		if (str[i] == '.' || str[i] == '+') {
			if (last_is_punct) return false;
			if (str[i] == '.') dot_cnt_after_at ++;
			last_is_punct = true;
			last_part_len = 0;
		} else 
		if (str[i] >= '0' && str[i] <= '9' || str[i] >= 'A' && str[i] <= 'Z' || str[i] >= 'a' && str[i] <= 'z' || str[i] == '_'){
			last_is_punct = false;
			last_part_len ++;
		} else
		if (str[i] == '-') {
			if (i == 0) return false;
			last_is_punct = false;
			last_part_len++;
		} else
			return false;
	}
	if (at_cnt == 0) return false;
	if (last_part_len > 6 || last_part_len < 2) return false;
	if (dot_cnt_after_at == 0 || dot_cnt_after_at > 4) return false;
	return true;
}

bool NameCheckerI::isPinyinStr(const string &str){
	if (str.length() > 20) return false;
	if (str.length() != normalize(str, OPT_ALPHA).length()) return false;
	return true;
}

NCResSeq NameCheckerI::NECheckBatch(const vector<string> & lines_vec, const Ice::Current&)
{ 
	NCResSeq vec_res;//the results vector
    int nSize = lines_vec.size();//the size of the input
    int j = 0;
    for(j = 0; j<nSize; j++)
    {
        struNCResult struResult;
        struResult = NECheck(lines_vec[j]);//call NECheck function
        vec_res.push_back(struResult);// push the current result to the vector.
    }
    return vec_res;
}

void NameCheckerI::reloadPageIndex(const string &path, const Ice::Current&)
{
	m_pPageBuzId = NULL;
	m_pPageBuzQuery = NULL;
	m_pPageIndexById = NULL;
	reloadPageBuzQuery("/data/xce/PageInfo/buzQuery.csv");
	
	string line;
	vector<string> vec;
	vector<int> score;
	
	m_pPageIndexById = new map<int, struPageResult>();

	if (m_pPageIndexerTemp != NULL) delete m_pPageIndexerTemp;
	m_pPageIndexerTemp = new PageIndexer;
	m_pPageIndexerTemp->m_indexer.clear();

    MCE_INFO("NameCheck::PageQuery::Start reload page data: " + path);
	ifstream fin(path.c_str());
	if (!fin.is_open()){
		MCE_INFO("NameCheck::PageQuery::Page data file not exist, read failed.");
		return;
	}

	MCE_INFO("NameCheck::PageQuery::Start read data file");
	while (getline(fin, line))
	{
		vec.clear();
		split(line, '', vec);
		if (vec.size() == 7){
			struPageResult pageResult;
			pageResult.id = pageResult.vertifyType = pageResult.fanNum = 0;
			sscanf(vec[0].c_str(), "%d", &pageResult.id);
			sscanf(vec[1].c_str(), "%d", &pageResult.vertifyType);
			sscanf(vec[2].c_str(), "%d", &pageResult.fanNum);
			pageResult.name = normalize(vec[3],OPT_CHINESE|OPT_ALPHA|OPT_NUMBER|OPT_UPPTOLOW);
			pageResult.summary = vec[4];
			pageResult.pageType = vec[5];
			pageResult.headurl = vec[6];
			pageResult.relevanceScore = pageResult.fanNum;
			if (pageResult.vertifyType == 2) pageResult.relevanceScore = pageResult.relevanceScore * 6 + 1000;
			pair<map<string, struPageResult>::iterator, bool> res = m_pPageIndexerTemp->m_indexer.insert(make_pair(pageResult.name, pageResult));
			if (!res.second){  // insert failed
				int oldRelevanceScore = res.first->second.relevanceScore;
				if (pageResult.relevanceScore > oldRelevanceScore)
					res.first->second = pageResult;
			}
			score.push_back(pageResult.relevanceScore);
//			MCE_INFO("NameCheck::Debug::Gua Mei Gua?");
			if (m_pPageBuzId->find(pageResult.id) != m_pPageBuzId->end())
				m_pPageIndexById->insert(make_pair(pageResult.id, pageResult));
//			MCE_INFO("NameCheck::Debug::Mei Gua!!!");
		}
	}

	for (map<string, int>::iterator it = m_pPageBuzQuery->begin(); it != m_pPageBuzQuery->end(); it++){
		map<int, struPageResult>::iterator it_page = m_pPageIndexById->find(it->second);
		if (it_page != m_pPageIndexById->end())
			m_pPageIndexerTemp->m_indexer[it->first] = it_page->second;
	}

    MCE_INFO("NameCheck::PageQuery::Data file read finished, " << score.size() << " lines read.");
	if (score.size() < 10) {
		MCE_INFO("NameCheck::PageQuery::Vaild data row are not enough, the page data index won't change.");
		return;
	}
	sort(score.begin(), score.end());
	
	int TotalDisplayPageNumber = 0;
	for (int i=score.size()-1; i>=0; --i, ++TotalDisplayPageNumber)
		if (score[i] < 1000) break;

	int GradeContainPageNumber = TotalDisplayPageNumber / GRADE_COUNT;
	for (int i=1; i<=GRADE_AVAILABLE; ++i)
		m_pPageIndexerTemp->m_gradation[i-1] = score[score.size()-i*GradeContainPageNumber];

    MCE_INFO("NameCheck::PageQuery::page relevance score calculate finished. Top3 rank need score greater than " << m_pPageIndexerTemp->m_gradation[0] << ", " << m_pPageIndexerTemp->m_gradation[1] << ", " << m_pPageIndexerTemp->m_gradation[2]);
	PageIndexer *onlineIndexerBackup = m_pPageIndexer;
	m_pPageIndexer = m_pPageIndexerTemp;
	m_pPageIndexerTemp = onlineIndexerBackup;
    
	MCE_INFO("NameCheck::PageQuery::Replace the page index memory finished.");
	fin.close();
    MCE_INFO("NameCheck::PageQuery::All the reload operator finished.");
	
	if (m_pPageBuzQuery != NULL) delete m_pPageBuzQuery;
	if (m_pPageBuzId != NULL) delete m_pPageBuzId;
	if (m_pPageIndexById != NULL) delete m_pPageIndexById;
}

void NameCheckerI::reloadPageBuzQuery(const string &path){
	MCE_INFO("NameCheck::PageQuery::Start reload buz dict.");
	m_pPageBuzQuery = new map<string, int>();
	m_pPageBuzId = new set<int>();
	ifstream fin(path.c_str());
	vector<string> vec;
	string line;
	while (getline(fin, line)){
		vec.clear();
		split(line, ',', vec);
		if (vec.size() < 2) continue;
		int id = 0;
		sscanf(vec[0].c_str(), "%d", &id);
		if (id <= 0) continue;
		for (int i=1; i<4; ++i){
			if (vec[i].length() == 0) continue;
			m_pPageBuzQuery->insert(make_pair(vec[i], id));
			m_pPageBuzId->insert(id);
		}
	}
	fin.close();
	MCE_INFO("NameCheck::PageQuery::BuzQuery load finished.");
}

void NameCheckerI::getPageRank(PageIndexer *workerIndexer, struPageResult &res){
	if (res.relevanceScore < 1000)
		res.rank = -1;
	else{
		res.rank = 9;
		for (size_t i=0; i<GRADE_AVAILABLE; ++i)
			if (res.relevanceScore >= workerIndexer->m_gradation[i]){
				res.rank = i;
				break;
			}
	}
}

void NameCheckerI::initNCResult(struNCResult &res){
	res.strResult = "";
	res.nHeadCount = 0;
}

void NameCheckerI::initPageResult(struPageResult &res){
	res.rank = -1;
	res.id = res.vertifyType = res.fanNum = 0;
	res.name = res.summary = res.pageType = res.headurl = "";
}

struPageResult NameCheckerI::pageQuery(const string &queryStr, const Ice::Current&)
{
	struPageResult res;
    initPageResult(res);
	PageIndexer *workerIndexer = m_pPageIndexer;
	if (workerIndexer == NULL) return res;

	map<string, struPageResult>::iterator it = workerIndexer->m_indexer.find(queryStr);
	if (it != workerIndexer->m_indexer.end())
		res = it->second;
	else
		return res;
	getPageRank(workerIndexer, res);
	return res;
}

queryResult NameCheckerI::query(const string &queryStr, const Ice::Current&)
{
	queryResult res;
	if (!m_isReady){
		MCE_INFO("NameCheck::query::Services is not ready!!");
		initNCResult(res.ncRes);
		initPageResult(res.pageRes);
		return res;
	}
	MCE_INFO("NameCheck::query::input:'" << queryStr << "'");
	res.ncRes = NECheck(normalize(queryStr, OPT_CHINESE|OPT_ALPHA|OPT_NUMBER|OPT_PUNCT|OPT_SPACE));
	MCE_INFO("NameCheck::query::namecheck:'" << res.ncRes.nHeadCount << "'");
	res.pageRes = pageQuery(normalize(queryStr, OPT_CHINESE|OPT_ALPHA|OPT_NUMBER|OPT_UPPTOLOW));
	if (res.pageRes.rank != -1)
	MCE_INFO("NameCheck::query::page:'" << res.pageRes.name << "/" << res.pageRes.id << "/" << res.pageRes.fanNum << "'");
	res.interviewRes = interviewQuery(normalize(queryStr, OPT_CHINESE|OPT_ALPHA|OPT_NUMBER|OPT_UPPTOLOW));
	if (res.interviewRes.id != -1)
	MCE_INFO("NameCheck::queryinterview:'" << res.interviewRes.interviewName << "/" << res.interviewRes.title);
	return res;
}

void NameCheckerI::reloadInterviewData(const string &path){
	string line;
	vector<string> vec;
	ifstream fin(path.c_str());
	if (!fin.is_open()) {
		MCE_INFO("NameCheck::interview::can not found path " << path <<", read error.");
		return;
	}
	while (getline(fin, line))
	{
		vec.clear();
		split(line, '\t', vec);
		if (vec.size() == 5){
			int id = str2int(vec[0]);
			m_interviewIndex[id].id = id;
			m_interviewIndex[id].interviewName = vec[1];
			m_interviewIndex[id].landingPageId = str2int(vec[2]);
			m_interviewIndex[id].title = vec[3];
			m_interviewIndex[id].summary = vec[4];
		}
	}
	MCE_INFO("NameCheck::interview info load finished.");
	fin.close();
}

void NameCheckerI::reloadInterviewQuery(const string &path){
	string line;
	vector<string> vec;
	ifstream fin(path.c_str());
	if (!fin.is_open()) {
		MCE_INFO("NameCheck::interview::can not found path " << path <<", read error.");
		return;
	}
	while (getline(fin, line))
	{
		vec.clear();
		split(line, '\t', vec);
		if (vec.size() == 2)
			m_interviewQuery[normalize(vec[0], OPT_CHINESE|OPT_ALPHA|OPT_NUMBER|OPT_UPPTOLOW)] = str2int(vec[1]);
	}
	MCE_INFO("NameCheck::interview query load finished.");
	fin.close();
}

struInterviewResult NameCheckerI::interviewQuery(const string &queryStr, const Ice::Current&){
	struInterviewResult res;
	res.id = -1;
	res.interviewName = res.title = res.summary = "";
	res.landingPageId = 0;
	if (m_interviewQuery.count(queryStr) != 0) {
		int id = m_interviewQuery[queryStr];
		if (m_interviewIndex.count(id) != 0) res = m_interviewIndex[id];
	}
	if (res.id == -1 && (queryStr.find("黄渤") != string::npos || queryStr.find("徐峥") != string::npos|| queryStr.find("泰囧") != string::npos)){
		int id = 83; //人再囧途之泰囧
		if (m_interviewIndex.count(id) != 0) res = m_interviewIndex[id];
	}
	return res;
}

int NameCheckerI::str2int(const string &str){
	int res = 0;
//	MCE_INFO("step 0.25");
	sscanf(str.c_str(), "%d", &res);
//	MCE_INFO("step 0.5");
	return res;
}
