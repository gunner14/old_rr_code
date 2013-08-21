#include "runtimeconfig.h"
#include "infconfigparser.h"
#include "util.h"

CRunTimeConfig g_runTimeConfig;

void CRunTimeConfig::Init()
{
	ParseUploadConf("./conf/channel.conf");
	ParseQueryNormalizeConf("./conf/query_normalize.conf");
}

void CRunTimeConfig::ParseQueryNormalizeConf(const string& strConfFile)
{
	static CInfConfigParser queryNormalizeConfParser;
	map<string,map<string,string> >mapInfConf;
	bool bOk=queryNormalizeConfParser.ParseInfConf(strConfFile,mapInfConf);
	if(!bOk || mapInfConf.empty())
		return;

	m_mapPageType2QueryNormalize.clear();
	for(map<string,map<string,string> >::iterator itMap1=mapInfConf.begin(); itMap1!=mapInfConf.end(); itMap1++){
		const string& strPageType=itMap1->first;
		map<string,string>& mapKeyValue=itMap1->second;
		for(map<string,string>::iterator itMap2=mapKeyValue.begin(); itMap2!=mapKeyValue.end(); itMap2++){
			const string& strDesParam = itMap2->first;
			const string& strSrcParam = itMap2->second;
			pair<string,string> pairTemp;
			if(!Str2Pair<string>(strSrcParam, pairTemp, ":"))
				continue;
			m_mapPageType2QueryNormalize[strPageType][strDesParam]=pairTemp;
		}
	}
}

void CRunTimeConfig::ParseUploadConf(const string& strConfFile)
{
	static CInfConfigParser uploadConfParser;
	map<string,map<string,string> >mapInfConf;
	bool bOk=uploadConfParser.ParseInfConf(strConfFile,mapInfConf);
	if(!bOk || mapInfConf.empty())
		return;
	
	m_mapUploadConf.clear();
	for(map<string,map<string,string> >::iterator itMap1=mapInfConf.begin(); itMap1!=mapInfConf.end(); itMap1++){
		const string& strChannel=itMap1->first;
		map<string,string>& mapKeyValue=itMap1->second;
		if(strChannel==""){
			fprintf(stderr,"upload channel can't be empty\n");
			continue;
		}

		TUploadConf uploadConf;
		uploadConf.strChannel=strChannel;
		uploadConf.strBaseChannel=mapKeyValue["basechannel"];
		uploadConf.nFileSizeLimit=1024*1024*atoi(mapKeyValue["filesizelimit"].c_str());
		uploadConf.strIsLoginUrl=mapKeyValue["isloginurl"];
		uploadConf.strAfterUrl=mapKeyValue["afterurl"];
		uploadConf.strSaveSubDir=mapKeyValue["savesubdir"];
		uploadConf.strSaveNamePrefix=mapKeyValue["savenameprefix"];
		
		Str2Pair<float>(mapKeyValue["sizerate"],uploadConf.pairSizeRate);
		Str2Pair<int>(mapKeyValue["minsize"],uploadConf.pairMinSize);
		
		ParseSaveGroupConf(mapKeyValue["savegroups"],uploadConf.vecSaveGroupConf);
		ParseSaveImageConf(mapKeyValue["saveimages"],uploadConf.vecSaveImageConf);
		
		Split(mapKeyValue["exts"],",",uploadConf.setExt);
		uploadConf.strExts=mapKeyValue["exts"];

		m_mapUploadConf[strChannel]=uploadConf;

	}
}

bool  CRunTimeConfig::ParseSaveGroupConf(const string& strLine, vector<TSaveGroupConf>& vecSaveGroupConf)
{
	vecSaveGroupConf.clear();
	vector<string>vecConf;
	Split(strLine, "|", vecConf);
	for(size_t i=0; i<vecConf.size(); i++){
		const string& strConf= vecConf[i];
		vector<string>vecSeg;
		Split(strConf, ",", vecSeg);
		if(vecSeg.size()<2)
			continue;
		for(size_t k=0; k<vecSeg.size(); k++)
			vecSeg[k]=Trim(vecSeg[k]);
		TSaveGroupConf saveGroupConf;
		saveGroupConf.strGroupName = vecSeg[0];
		saveGroupConf.strSaveNodePath = vecSeg[1];
		vecSaveGroupConf.push_back(saveGroupConf);
	}
	return !vecSaveGroupConf.empty();
}

bool  CRunTimeConfig::ParseSaveImageConf(const string& strLine, vector<TSaveImageConf>& vecSaveImageConf)
{
	vecSaveImageConf.clear();
	vector<string>vecConf;
	Split(strLine, "|", vecConf);
	for(size_t i=0; i<vecConf.size(); i++){
		const string& strConf= vecConf[i];
		vector<string>vecSeg;
		Split(strConf, ",", vecSeg);
		if(vecSeg.size()<4)
			continue;
		for(size_t k=0; k<vecSeg.size(); k++)
			vecSeg[k]=Trim(vecSeg[k]);
		TSaveImageConf saveImageConf;
		saveImageConf.nWidth = atoi(vecSeg[0].c_str());
		saveImageConf.nHeight = atoi(vecSeg[1].c_str());
		saveImageConf.strNamePrefix = vecSeg[2];
		saveImageConf.bCrop = bool(atoi(vecSeg[3].c_str()));
		if(vecSeg.size() > 4)//有quality信息
			saveImageConf.quality = atoi(vecSeg[4].c_str());
		else
			saveImageConf.quality = 90;//默认90
		vecSaveImageConf.push_back(saveImageConf);
	}
	return !vecSaveImageConf.empty();
}

const TUploadConf* CRunTimeConfig::GetUploadConf(const string& channel) const
{
	map<string,TUploadConf>::const_iterator itMap=m_mapUploadConf.find(channel);
	if(itMap!=m_mapUploadConf.end())
		return &itMap->second;
	else
		return NULL;
}


bool CRunTimeConfig::IsAncestorChannal(const string& strChannel, const string& strAncestorChannel)
{
	if(strChannel == strAncestorChannel)
		return true;

	const TUploadConf* pUploadConf=NULL;
	string channel=strChannel; 
	do{
		pUploadConf=g_runTimeConfig.GetUploadConf(channel);
		if(pUploadConf){
			if(pUploadConf->strChannel == strAncestorChannel || pUploadConf->strBaseChannel == strAncestorChannel)
				return true;
			channel=pUploadConf->strBaseChannel;
		}
	}
	while(pUploadConf);
	return false;
	
}
