#include "requestparser.h"
#include "util.h"
#include "runtimeconfig.h"
#include <fcgi_stdio.h>
char g_buf[8192];

void CRequestParser::ParseCGIQuery(const string &strQuery, map<string, string> &mapParam)
{	
	mapParam.clear();
	vector<string>vecSeg;
	Split(strQuery,"&",vecSeg);
	for(size_t i=0;i<vecSeg.size();i++){
		const string& strSeg=vecSeg[i];
		size_t nPos = strSeg.find('=');
		if (nPos != string::npos) {
			const string& strName=strSeg.substr(0,nPos);
			if(mapParam.find(strName) == mapParam.end())		//同resin服务器,优先前面的参数,如a=1&b=2&a=3,则取a=1
				mapParam[strName] = DecodeUrl(strSeg.substr(nPos+1));
		}
	}
}

void CRequestParser::ParseStreamData(const string& strPostData, vector<TFormFile>& vecFormFile)
{
	vecFormFile.push_back(TFormFile());
	TFormFile& formfile=vecFormFile[vecFormFile.size()-1];
	formfile.strName="tmp.jpg";
	formfile.strFileName="tmp.jpg";			//先decode再encode,来兼容client有的encode的,有的没有encode
	formfile.strContentType="application/octet-stream";
	formfile.strFileData=strPostData;
}


void CRequestParser::ParseMultiPartData(const string& strPostData,const string& strBoundary,map<string,string>& mapQueryByPost,vector<TFormFile>& vecFormFile)
{
	mapQueryByPost.clear();
	vecFormFile.clear();
	string strRealBoundary="--"+strBoundary;
	vector<string> vecPart;
	Split(strPostData,strRealBoundary,vecPart);
	for(size_t i=0; i<vecPart.size();i++){
		const string& strPart = vecPart[i];
		string strNameInfo;		//字段名称相关信息
		string strValue;		//字段的值
		string strDisposition;
		string strName;
		string strFileName;
		string strContentType;    

		size_t nNameInfoEndPos=strPart.find("\r\n\r\n");
		if(nNameInfoEndPos==string::npos)
			continue;

		strNameInfo=strPart.substr(0,nNameInfoEndPos);
		strValue=strPart.substr(nNameInfoEndPos+4,strPart.size()-nNameInfoEndPos-6);
		
		vector<string> vecLine;
		Split(strNameInfo,"\r\n",vecLine);
		string strLine,strHeaderName,strHeaderValue,strSeg,strKey,strVal;
		for(size_t j=0; j<vecLine.size(); j++){
			strLine=vecLine[j];
			size_t nPos=strLine.find(":");
			if(nPos==string::npos)
				continue;

			strHeaderName=ToLower(strLine.substr(0,nPos));
			strHeaderValue=strLine.substr(nPos+1);
			vector<string>vecSeg;
			Split(strHeaderValue,";",vecSeg);
			if(vecSeg.size()==0)
				continue;

			if(strHeaderName=="content-disposition"){
				strDisposition=Trim(vecSeg[0]);
				for(size_t k=1; k<vecSeg.size(); k++){
					strSeg=Trim(vecSeg[k]);
					nPos=strSeg.find("=");
					if(nPos!=string::npos){
						strKey=ToLower(strSeg.substr(0,nPos));
						strVal=strSeg.substr(nPos+1);
						strVal=Trim(strVal,"\" ");
						if(strKey=="name")
							strName=strVal;
						else if(strKey=="filename")
							strFileName=strVal;
					}
				}

			}
			else if(strHeaderName=="content-type"){
				strContentType=Trim(vecSeg[0]);
			}
		}//end for vecLine
		
		//
		if(strName=="")
			continue;
		if(strFileName==""){
			mapQueryByPost[strName]=strValue;
		}
		else if(strValue.size()>0){
			vecFormFile.push_back(TFormFile());
			TFormFile& formfile=vecFormFile[vecFormFile.size()-1];
			formfile.strName=strName;
			formfile.strFileName=strFileName;			//先decode再encode,来兼容client有的encode的,有的没有encode
			formfile.strContentType=strContentType;
			formfile.strFileData=strValue;
		}

	}//end for vecSEg
}
bool CRequestParser::ParseRequest(TRequest& request,TResponse& response)
{
	request.Init();
	response.Init();

	string& strPostData=request.strPostData;
	map<string,string>& mapEnv=request.mapEnv;
	map<string,string>& mapQuery=request.mapQuery;
	map<string,string>& mapCookie=request.mapCookie;
	vector<TFormFile>& vecFormFile=request.vecFormFile;
	
	//获取环境变量
	string strVar;
	size_t nPos=string::npos;
	for(char** var=environ; *var!=NULL; ++var){
		strVar=*var;
		nPos=strVar.find('=');
		if(nPos!=string::npos)
			mapEnv[strVar.substr(0,nPos)]=strVar.substr(nPos+1);
	}

	//获取post内容
	int nContentLength=atoi(mapEnv["CONTENT_LENGTH"].c_str());
	strPostData.reserve(nContentLength);
	if(nContentLength>0){
		int nReadCount=0;
		while(true){
			nReadCount=fread(g_buf,sizeof(char),sizeof(g_buf),FCGI_stdin);
			strPostData+=string(g_buf,nReadCount);
			if((size_t)nReadCount<sizeof(g_buf))
				break;
		}
		/*char* buf=new char[nContentLength];
		int nReadCount=fread(buf,sizeof(char),nContentLength,stdin);
		strPostData=string(buf,nReadCount);
		delete buf;*/
				
		if(strPostData.size()<(size_t)nContentLength){
			response.nCode=0;
			response.strMsg="ParseRequest Error: Not enough bytes received on standard input";
			return false;
		}
	}

	//解析get参数表
	string strQuery=mapEnv["QUERY_STRING"];	
	ParseCGIQuery(strQuery,mapQuery);
	
	
	//解析post数据:获取post参数表和文件列表
	map<string,string>mapQueryByPost;
	string strContentType=request.mapEnv["CONTENT_TYPE"];
	string strBoundary;
	nPos=strContentType.find(';');
	if(nPos!=string::npos){
		strBoundary=Trim(strContentType.substr(nPos+1));
		strContentType=ToLower(Trim(strContentType.substr(0,nPos)));
		nPos=strBoundary.find("boundary=");
		if(nPos!=string::npos)
			strBoundary=strBoundary.substr(nPos+9);
		else
			strBoundary="";
	}


	if(strContentType=="application/x-www-form-urlencoded"){
		ParseCGIQuery(strPostData,mapQueryByPost);
	}
	else if(strContentType=="text/plain"){		
	}
	else if(strContentType=="multipart/form-data"){
		ParseMultiPartData(strPostData,strBoundary,mapQueryByPost,vecFormFile);
	}
	else if(strContentType=="application/octet-stream")
		ParseStreamData(strPostData,vecFormFile);
	else{
	}

	//合并post参数表到get参数表中:仅当某post参数不在get参数表中时才合并
	for(map<string,string>::iterator itMap=mapQueryByPost.begin(); itMap!=mapQueryByPost.end(); itMap++){
		const string& strKey=itMap->first;
		const string& strValue=itMap->second;
		if(mapQuery.find(strKey)==mapQuery.end())	//同resin服务器,优先get参数,同一名称的参数即在get又在post中,则取get参数
			mapQuery[strKey]=strValue;
	}

	//为了兼容,对不同页面类型(由pagetype参数值而定)做参数归一化,规则由配置文件conf/query_normalize.conf指定
	//归一化目标参数规则:如目标参数存在则跳过; 如指定的源参数存在,则取其值,否则取缺省值
	string strPageType= request.Query("pagetype");
	map<string,map<string,pair<string,string> > >::const_iterator itMap =g_runTimeConfig.m_mapPageType2QueryNormalize.find(strPageType);
	if(itMap != g_runTimeConfig.m_mapPageType2QueryNormalize.end()){
		for(map<string,pair<string,string> >::const_iterator itMap2 = itMap->second.begin(); itMap2!=itMap->second.end(); itMap2++){
			const string& strDstParam = itMap2->first;
			const string& strSrcParam = itMap2->second.first;
			const string& strDefaultValue= itMap2->second.second;
			if(mapQuery.find(strDstParam) != mapQuery.end())
				continue;
			if(mapQuery.find(strSrcParam) != mapQuery.end())
				mapQuery[strDstParam] = mapQuery[strSrcParam];
			else
				mapQuery[strDstParam] = strDefaultValue;
		}
	}else{
		/*response.nCode=0;
		response.strMsg="错误的频道参数";
		return false;*/
	}
	
	///activex控件的op参数(由flag转换而来)的特殊处理
	if(strPageType=="addPhotoActivex" || strPageType=="addPhotoX"){
		string& strOp = mapQuery["op"];
		if(strOp == "0" )
			strOp = "upload";
		else if(strOp == "1")
			strOp = "finish";
		else if(strOp =="-1")
			strOp ="clean";
	}
	
	//避免414 url too large
	if(strPageType == "addPhotoActivex" || strPageType == "addPhotoFlash" || strPageType == "addPhotoX")	
		mapQuery["ids"] ="";	

		
	//初始化cookie
	string strCookie=mapEnv["HTTP_COOKIE"];
	vector<string>vecSeg;
	Split(strCookie,";",vecSeg);
	if(vecSeg.size()==1)		//如果请求header有多少Cookie:域,则webserver会拼成一个Cookie:域,分隔符为","(如用";"更好), UploadTest即如此
		Split(strCookie,",",vecSeg);

	for(size_t i=0; i<vecSeg.size(); i++){
		const string& strSeg=vecSeg[i];
		size_t nPos=strSeg.find("=");
		if(nPos!=string::npos){
			mapCookie[Trim(strSeg.substr(0,nPos))]=Trim(strSeg.substr(nPos+1));
		}
	}

	//初始化pUploadConf
	request.pUploadConf=g_runTimeConfig.GetUploadConf(request.Query("channel"));
	if(request.pUploadConf==NULL){
		response.nCode=0;
		response.strMsg="错误的频道参数";
		return false;
	}

	//初始化response
	response.nTotalCount = vecFormFile.size();
	response.vecFileResponse.resize(vecFormFile.size());
	for(size_t i=0; i<vecFormFile.size(); i++){
		TFileResponse& fileResponse = response.vecFileResponse[i];
		fileResponse.nCode = 1;
		fileResponse.nFileSize = vecFormFile[i].strFileData.size();
		//如果是ie的话，给的是全路径，需要截取
		if(request.Env("USER_AGENT").find("MSIE") != string::npos)
		{
			string strName = GetFileName(vecFormFile[i].strFileName);
			if(request.Query("pagetype") == "addPhotoActivex")//老版控件上传的文件名时GBK编码的
				strName = GBKToUTF8(strName);
			fileResponse.strFileName = strName;
		}else
			fileResponse.strFileName = DecodeUrl(vecFormFile[i].strFileName);
	}
	return true;
}
