/**
 * @file paramparser.h
 * @date 2009-06.29
 * */
#ifndef _PARAM_PARSE_H
#define _PARAM_PARSE_H
#include "datatype.h"
/**
 *此类负责解析fastcgi的http输入参数,解析结果如下:
 *1.post参数表
 *2.get参数表表
 *3.上传文件集合
 *4.环境变量表
 */
class CRequestParser
{
public:
	///解析fcgi请求为TRequest:环境变量列表,query,formfile列表
	bool ParseRequest(TRequest &request,TResponse& response);
	
	//
	static void ParseCGIQuery(const string &strQuery, map<string, string>& mapParam);
	
	//
	static void ParseMultiPartData(const string& strPostData,const string& strBoundary,map<string,string>& mapQueryByPost,vector<TFormFile>& vecFormFile);

	static void ParseStreamData(const string& strPostData, vector<TFormFile>& vecFormFile);

};
#endif
