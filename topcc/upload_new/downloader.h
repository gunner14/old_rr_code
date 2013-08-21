#ifndef _DOWNLOADER_H_
#define _DOWNLOADER_H_
#include <string>
#include <vector>
using std::string;
using std::vector;

struct THttpArg
{
	string strReqBody;
	vector<string>vecReqHeader;
	string strResHeader;
	string strResBody;
	string strError;
};
class CDownloader
{
public:	
	CDownloader();
	int Download(const string& strUrl, THttpArg & httpArg);
private:
	static long LibCurlWriter(void *data, size_t size, size_t nmemb, void* stream);
};

extern CDownloader g_downloader;
#endif
