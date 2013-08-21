#include "requestparser.h"
#include "runtimeconfig.h"
#include "memcache.h"
#include "processdispatcher.h"
#include "util.h"

#include <signal.h>
#include <fcgi_stdio.h>
#include <iostream>
using namespace std;

void SigIgnore(int x)
{
	cout<<"SigIgnore:"<<x<<endl;
}

int main()
{
	signal(SIGPIPE,SigIgnore);

	TRequest request;
	TResponse response;
	CRequestParser requestparser;
	CProcessDispatcher dispatcher;

	bool bParseOk=true;
	bool bProcessOk=true;
	string strOutput;
	time_t tLastInitTime=0;

	while (FCGI_Accept() >= 0){
		
		//配置文件30分钟初始化1次
		if(time(0)-tLastInitTime>1800){
			Log("重新Load配置");
			g_runTimeConfig.Init();
			g_memcache.Init();
			tLastInitTime=time(0);
		}
		
		//解析输入请求
		bParseOk=requestparser.ParseRequest(request,response);
		
		/*printf("Content-Type: text/html; charset=utf-8\r\n\r\n");
		printf("<pre>lvwei\n");
		char *p=getenv("QUERY_STRING");
		printf("query=%s\n",p?p:"");
		string strQuery=request.Query();
		printf("query2=%s\n",p?p:"");
		continue;*/
		//处理请求
		if(bParseOk){
			bProcessOk=dispatcher.Process(request,response);

			//输出
			response.Output(request,strOutput);
			printf("%s\n", strOutput.c_str());
			fflush(stdout);
		}else{
			Log("parse request error %s %s %s", request.Env().c_str(), request.Query().c_str(), request.Cookie().c_str());
			string strErrorput = "Status:500\r\n";
			strErrorput += "\r\n";
			strErrorput += "bad request";
			printf("%s\n", strErrorput.c_str());
			fflush(stdout);
		}
	}

	return 0;
}
