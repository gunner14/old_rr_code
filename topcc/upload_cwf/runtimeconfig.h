#ifndef _RUNTIME_CONFIG_H_
#define _RUNTIME_CONFIG_H_

#include "datatype.h"


class CRunTimeConfig
{
public:
	void Init();

	///<解析channel.conf
	void ParseUploadConf(const string& strConfFile);

	///<获取UploadConf
	const TUploadConf* GetUploadConf(const string& channel) const;	
	
	///<判断strAncestorChannel是否是strChannel的祖先栏目
	bool IsAncestorChannal(const string& strChannel, const string& strAncestorChannel);

	///<解析参数一致配置文件到m_mapReferer2QueryNormalize
	void ParseQueryNormalizeConf(const string& strConfFile);
private:	
	///
	bool ParseSaveGroupConf(const string& strLine, vector<TSaveGroupConf>& vecSaveGroupConf);
	///
	bool ParseSaveImageConf(const string& strLine, vector<TSaveImageConf>& vecSaveImageConf);

public:	
	map<string,map<string,pair<string,string> > >m_mapPageType2QueryNormalize;
private:	
	map<string,TUploadConf> m_mapUploadConf;	///<栏目名到配置的映射
};

extern CRunTimeConfig g_runTimeConfig;
#endif
