#include "util.h"
#include "infconfigparser.h"
#include "runtimeconfig.h"
#include "requestparser.h"
#include <iostream>
using namespace std;

void ParseInfConfTest()
{
	string strConfFile="conf/channel.conf";
	map<string,map<string,string> >mapInfConf;
	CInfConfigParser parser;
	parser.ParseInfConf(strConfFile,mapInfConf);
	for(map<string,map<string,string> >::iterator itMap1=mapInfConf.begin(); itMap1!=mapInfConf.end(); itMap1++){
		printf("\n['%s']\n",itMap1->first.c_str());
		for(map<string,string>::iterator itMap2=itMap1->second.begin(); itMap2!=itMap1->second.end(); itMap2++){
			printf("'%s'='%s'\n",itMap2->first.c_str(),itMap2->second.c_str());
		}
	}
}
void SplitTest()
{
	//SplitToVector
	string strTest="lvwei,hello,ok";
	string strSep=",";
	vector<string>vecSeg;
	Split(strTest,strSep,vecSeg);
	printf("SplitToVector:str='%s'\tsep='%s'\tsplitcount=%lu\n",strTest.c_str(), strSep.c_str(), vecSeg.size());


	strTest="lvwei,hello,ok,";
	vecSeg.clear();
	Split(strTest,strSep,vecSeg);
	printf("SplitToVector:str='%s'\tsep='%s'\tsplitcount=%lu\n",strTest.c_str(), strSep.c_str(), vecSeg.size());
	
	strTest=",lvwei,hello,ok,";
	vecSeg.clear();
	Split(strTest,strSep,vecSeg);
	printf("SplitToVector:str='%s'\tsep='%s'\tsplitcount=%lu\n",strTest.c_str(), strSep.c_str(), vecSeg.size());
	

	//SplitToSet	
	strTest="lvwei,hello,lvwei,lvkai,zyx";
	strSep=",";
	set<string>setSeg;
	Split(strTest,strSep,setSeg);
	printf("SplitToSet:str='%s'\tsep='%s'\tsplitcount=%lu\n",strTest.c_str(), strSep.c_str(), setSeg.size());
	
	strTest="lvwei,hello,lvwei,lvwei,hello,";
	setSeg.clear();
	Split(strTest,strSep,setSeg);
	printf("SplitToSet:str='%s'\tsep='%s'\tsplitcount=%lu\n",strTest.c_str(), strSep.c_str(), setSeg.size());
}

void RuntimeConfigTest()
{
	g_runTimeConfig.Init();
}
void ParseMultiPartDataTest()
{
	string strContent=LoadFile("./postdata.test");
	size_t nPos=strContent.find("\r\n");
	if(nPos==string::npos)
		return;
	string strBoundary=strContent.substr(0,nPos);
	string strPostData=strContent.substr(nPos+2);
	map<string,string>mapQueryByPost;
	vector<TFormFile>vecFormFile;
	CRequestParser::ParseMultiPartData(strPostData,strBoundary,mapQueryByPost,vecFormFile);

}
int main(int argc,char** argv)
{
	//ParseInfConfTest(argc,char**argv);
	SplitTest();
	//RuntimeConfigTest();

	ParseMultiPartDataTest();
	return 0;
}
