#include "infconfigparser.h"

#include <sys/stat.h>

#include <stdio.h>
#include <fstream>
using namespace std;

bool CInfConfigParser::ParseInfConf(const string& strConfFile, map<string,map<string,string> >& mapInfConf)
{       
	struct stat st;
        if(stat(strConfFile.c_str(),&st) !=0){
                fprintf(stderr,"file not found:%s\n",strConfFile.c_str());
                return false;
        }
        if(st.st_mtime <= m_tLastModifyTime)
       		return false;
        
        m_tLastModifyTime=st.st_mtime;

        ifstream fin(strConfFile.c_str());
        if(!fin){
       	 fprintf(stderr,"open file fail:%s\n",strConfFile.c_str());
       	 return false;
        }

        mapInfConf.clear();
        string strChannel;
        string strBaseChannel;
        string strLine;
        size_t nPos=string::npos;
        while(getline(fin,strLine)){
       	 if(strLine.size()<2 || strLine[0]=='#')
       		 continue;

       	 nPos=strLine.rfind("#");
       	 if(nPos!=string::npos)
       		 strLine=strLine.substr(0,nPos);

       	 if(strLine[0]=='[' || strLine[strLine.size()-1]==']'){
       		 strChannel=strLine.substr(1,strLine.size()-2);
       		 strBaseChannel="";
       		 nPos=strChannel.find(':');
       		 if(nPos!=string::npos){
       			 strBaseChannel=strChannel.substr(nPos+1);
       			 strChannel=strChannel.substr(0,nPos);
       			 mapInfConf[strChannel]=mapInfConf[strBaseChannel];
       			 mapInfConf[strChannel]["basechannel"]=strBaseChannel;
       		 }
       	 }
       	 else{   
       		 nPos=strLine.find('=');
       		 if(nPos!=string::npos){
       			 const string& strKey=strLine.substr(0,nPos);
       			 const string& strValue=strLine.substr(nPos+1);
       			 mapInfConf[strChannel][strKey]=strValue;
       		 }
       		 else    
       			 continue;
       	 }
        }

        return true;
}

