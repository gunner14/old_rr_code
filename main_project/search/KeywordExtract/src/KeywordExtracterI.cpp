/*
 * Author: dairui.cui@renren-inc.com
 * Date:2012-11-28
 * Property:support the keyword extract function.
 */
#include "KeywordExtracterI.h"
#include "LogWrapper.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "QueryRunner.h"
#include <stdlib.h>
#include <iconv.h>
#include <math.h>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/TimeCost.h"
#include <map>
#include "StringNormalize.h"

using namespace com::xiaonei::search::keywordextract;
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;

/*
 * Function:init the keyword extract obj.
 */
void MyUtil::initialize() {
    KeywordExtracterI::instance().initialize();
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&KeywordExtracterI::instance(), service.createIdentity("M",""));//ice, enable the servant KeywordExtracterI
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt( "KeywordExtract.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault( "KeywordExtract.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerKeywordExtract", &KeywordExtracterI::instance(), mod, interval,new SearchChannel());//
	//xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerKeywordExtract", &KeywordExtracterI::instance(), mod, interval,new GadgetsChannel());//
}

/*
 * Function:load the heat score file to memory.
 */
void KeywordExtracterI::initialize()
{
	MCE_INFO("KeywordExtracterI::start service.");
    m_bIsReady = false;
    m_curMapScore = NULL;
    m_tempMapScore = NULL;
    MCE_INFO("begin to reload keyword file");
    reloadKeyword("/data/xce/XiaoNeiSearch/etc/classes/keyword/current");
    m_bIsReady = true;
    MCE_INFO("keyword extract process initialize successful!");
}

/*
 *Parameters:
  const string& path : the heat score file path.the format is <keyword \t heatscore>
  when the file is updated,call this function.
 */
void KeywordExtracterI::reloadKeyword(const string& path,const Ice::Current&)
{
    MCE_INFO("reloadKeyword() start...");
    ifstream fin(path.c_str());    
    if (!fin.is_open())
    {
        MCE_ERROR("KeywordExtracter::reloadKeyword data file not exist" + path);
        return;
    }
    if(m_tempMapScore != NULL)
    {
        delete m_tempMapScore;
        m_tempMapScore = NULL;
    }
    m_tempMapScore = new std::map<string,float>();
    //load old map scores.
    if(m_curMapScore != NULL)
    {
        std::map<string,float>::iterator iterTemp = m_curMapScore->begin();
        for(iterTemp;iterTemp != m_curMapScore->end();iterTemp++)
        {
            m_tempMapScore->insert(std::make_pair(iterTemp->first,iterTemp->second));
        }
    }
    string line;
    std::map<string,float>::iterator iter;
    //load the newest map scores from the file.if exists,recover it.
    while (getline(fin,line))
    {
        vector<string> vec;
        com::renren::search::utility::splits(line,'\t',vec);
        if (vec.size() != 2)
        {
            MCE_INFO("format error");
            continue;
        }
        string keyword = vec[0];
        float  heatscore = atof(vec[1].c_str());
        iter = m_tempMapScore->find(keyword);
        if (iter != m_tempMapScore->end())
        {
            iter->second = heatscore > iter->second ? heatscore : iter->second;
        }
        else
        {
            m_tempMapScore->insert(std::make_pair(keyword,heatscore));
            MCE_INFO("insert keyword: " << keyword << " success," << " and heatScore is :" << heatscore);
        }
    }
    fin.close();

    MCE_INFO("m_tempMapScore's size is :" << m_tempMapScore->size());
    std::map<string,float>* pTemp = m_curMapScore;
    m_curMapScore = m_tempMapScore;
    m_tempMapScore = pTemp;
    MCE_INFO("reloadKeyword() end...");
}

KwResSeq KeywordExtracterI::getKeywords(const string& inputStr,const Ice::Current&)
{ 
    MCE_INFO("getKeywords() start...");
	KwResSeq vec_res;//the results vector
    vec_res.clear();
    if(!m_bIsReady || m_curMapScore == NULL)
    {
        MCE_INFO("service not ready");
        return vec_res;
    }
    std::map<std::string,int> mapResult = com::renren::search::utility::split(inputStr,2,5);
    std::map<string,int>::iterator iter = mapResult.begin();
    std::map<string,float> * pWorker = m_curMapScore;
    MCE_INFO("the split length is :" << mapResult.size());
    for(iter;iter != mapResult.end();iter++)
    {
        MCE_INFO("the input is :" << iter->first);
        std::map<string,float>::iterator iterItem = pWorker->find(iter->first);
        if(iterItem == pWorker->end())
        {
            continue;
        }
        else
        {
            struKwResult sRes;
            sRes.strKeyword = iterItem->first;
            sRes.heatScore = iterItem->second;
            vec_res.push_back(sRes);
        }
    }
    MCE_INFO("getKeywords() end...");
    return vec_res;
}

