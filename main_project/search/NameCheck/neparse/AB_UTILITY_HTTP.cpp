/*******************************************************************************
* File name: AB_UTILITY_HTTP.c
* Author: Zheng Xueshuang  Version: 0.1  Date: 12/30/2006
* Description: 
*             the implementation of fuctions to construct reqeust string.
*
* Funcion List:
*
*    1 struct ABU_URL_S * ABU_HTTP_parse(const char * sUrl,
*                                             ABU_URL_S * pUrlStruct);
*      The function constructs a url struct ABU_URL_S using given url.
*      The input parameter sUrl is the url to construct struct.
*      The parameter pUrlStruct is the struct to be added information.
*
*
*    2 struct ABU_HTTP_REQ_S * ABU_HTTP_REQ_create(const char * sUrl,
*                                              char * method, char * version);
*      The function construct a http request struct ABU_HTTP_REQ_S using given
*      url, method and version.
*      The parameter sUrl is url to be request. 
*      The parameter method is the detail method like GET, POST, HEAD and so on. 
*      The version is http version 1.0 or 1.1.
*
*
*    3 int  ABU_HTTP_REQ_addParam(ABU_HTTP_REQ_S * pHttpReq,
*                               const char * szParamName, char * szParamValue);
*      The fuction add parameters and values to  http reqeust struct.
*      The parameter pHttpReq is the request struct.
*      The parameter szParamName is the parameter name to be added.
*      The parameter szParamValue is the value of the szParamName.
*      The return is 0 when success and none 0 with fail.
*
*
*    4 char * ABU_HTTP_REQ_getBuf(ABU_HTTP_REQ_S *pHttpReq);
*
*      The function return a constructed http request according to 
*      struct ABU_HTTP_REQ_S.
*      The parameter pHttpReq is the struct of request.
*      The return value is the http request string.
*
*
*    5 void AB_HTTP_REQ_free(ABU_HTTP_REQ_S ** pHttpReq);
*
*      The function free the memory that ABU_HTTP_REQ_S and sRequestBuffer 
*      in ABU_HTTP_REQ_S.
*
*
*******************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "AB_UTILITY_HTTP.h"

 


ABU_URL_S * ABU_HTTP_parse(const char *sUrl, int nUrlLen, ABU_URL_S *pUrlStruct)
{
    if(sUrl == NULL|| nUrlLen < 0 || nUrlLen > MAX_URL_LEN || pUrlStruct == NULL)
        return NULL;

    int nFlag = 0;
    char *szStart = "http://";
    int i = 0;

    /*Find the position of '#'*/
    for(i = 0; i < nUrlLen; i++) {
        if(sUrl[i] == '#') {
            nUrlLen = i;
            break;
        }
    }

	//find url is start with http:// or not
    nFlag = nUrlLen >= 7 && (! strncasecmp(sUrl, szStart, 7));  /*whether the sUrl start with "http://"*/
    
    /*set pUrlStruct->sUrl*/
    if(nFlag)//include http
        strncpy(pUrlStruct->sUrl, sUrl, nUrlLen);
    else {
        strncpy(pUrlStruct->sUrl, szStart, 7);//copy http:// to sUrl
        strncpy(pUrlStruct->sUrl + 7, sUrl, nUrlLen);
    }
	pUrlStruct->wUrlLen = nFlag ? nUrlLen : (nUrlLen + 7);
	//it is for more strong code, not necessary
	if(pUrlStruct->wUrlLen < MAX_URL_LEN)
		pUrlStruct->sUrl[pUrlStruct->wUrlLen] = '\0'; 

	const char *sitebegin = nFlag ? sUrl + 7 : sUrl;
	char *siteend = (char *)sitebegin;
	int urllen = nFlag ? nUrlLen - 7 : nUrlLen;

	while(urllen > 0 && *siteend != '?' 
            && *siteend != '/' && *siteend != ':'){//find '/'
		urllen--;
		siteend++;
	}

	if(*siteend == ':')
		pUrlStruct->nPort = strtol(siteend + 1, NULL, 10);
	else
		pUrlStruct->nPort = 80;//default value

    //get site
    if((siteend-sitebegin) >= MAX_SITE_LEN) {
        strncpy(pUrlStruct->szSite, sitebegin, MAX_SITE_LEN-1);
        pUrlStruct->szSite[MAX_SITE_LEN-1] = '\0';
    }else{
        strncpy(pUrlStruct->szSite, sitebegin, siteend - sitebegin);
        pUrlStruct->szSite[siteend-sitebegin] = '\0';
    }

	if(!urllen){//urllen = 0, has no / or :
		pUrlStruct->wRUrlLen = 1;
		pUrlStruct->sRUrl[0] = '/';
		pUrlStruct->sRUrl[1] = '\0';
		pUrlStruct->sUrl[pUrlStruct->wUrlLen] = '/';
		pUrlStruct->wUrlLen++;
		if(pUrlStruct->wUrlLen < MAX_URL_LEN)
			pUrlStruct->sUrl[pUrlStruct->wUrlLen] = '\0';
    }else{//*siteend == ':' or *siteend = '/'
        if(*siteend == ':'){
			while(urllen > 0 && *siteend != '/' && *siteend != '?'){//find '/'
				urllen--;
				siteend++;
			}
		}
		if(*siteend == '/'){//find '/'
			strncpy(pUrlStruct->sRUrl, siteend, urllen);
			pUrlStruct->wRUrlLen = urllen;
			if(urllen < MAX_URL_LEN)
				pUrlStruct->sRUrl[urllen] = '\0'; //it's only for convience
		}
		if(*siteend == '?'){//find '?'
            strncpy(pUrlStruct->sRUrl, "/", 2);
			strncpy(pUrlStruct->sRUrl+1, siteend, urllen);
			pUrlStruct->wRUrlLen = urllen+1;
			if(urllen+1 < MAX_URL_LEN)
				pUrlStruct->sRUrl[urllen+1] = '\0'; //it's only for convience
		}
	}

    return pUrlStruct;
}



ABU_HTTP_REQ_S * ABU_HTTP_REQ_create(const char *sUrl, int nUrlLen, 
                                                char *method, char *version)
{
    /*assert((sUrl != NULL)&&(method != NULL)&&(version != NULL));*/

    if(sUrl == NULL || nUrlLen < 0 || nUrlLen > MAX_URL_LEN || \
			method == NULL || version == NULL)
        return NULL;

    ABU_HTTP_REQ_S *pHttpReq;
    pHttpReq = (ABU_HTTP_REQ_S *)calloc(1, sizeof(ABU_HTTP_REQ_S));
    memset(pHttpReq, 0, sizeof(ABU_HTTP_REQ_S));    

    ABU_HTTP_parse(sUrl, nUrlLen, &pHttpReq->url);  /*construct ABU_URL_S*/
    
    strncpy(pHttpReq->szMethod, method, MAX_HTTPMETHOD_LEN);
    strncpy(pHttpReq->szVersion, version, MAX_HTTPVERISON_LEN);
    return pHttpReq;   
}


int  ABU_HTTP_REQ_addParam(ABU_HTTP_REQ_S * pHttpReq, 
                              const char *szParamName, char *szParamValue)
{
    /*assert((pHttpReq != NULL)&&(szParamName != NULL)&&(szParamValue != NULL));*/

    if(pHttpReq == NULL || szParamName == NULL || szParamValue == NULL)
        return -1;

    /*The paramter name is null, return*/
    if(*szParamName == '\0')
        return 0;

    int i;
    for(i = 0; i < pHttpReq->byParamsSize; i++)
    {
		if( ! strcasecmp(pHttpReq->szNames[i], szParamName)) {
			strncpy(pHttpReq->szValues[i], szParamValue, MAX_HTTPPARAM_VALUE_LEN);
			return 0;
		}
    }
	//add new one
    /*the param is max size*/
    if(pHttpReq->byParamsSize >= MAX_HTTP_PARAMS_SIZE)
        return -1;

    strncpy(pHttpReq->szNames[pHttpReq->byParamsSize], szParamName, MAX_HTTPPARAM_NAME_LEN); 
    strncpy(pHttpReq->szValues[pHttpReq->byParamsSize], szParamValue, MAX_HTTPPARAM_VALUE_LEN);
    pHttpReq->byParamsSize++;
    return 0;
}

char * ABU_HTTP_REQ_getBuf(ABU_HTTP_REQ_S * pHttpReq)
{
    /*assert(pHttpReq != NULL);*/
    if(pHttpReq == NULL)
        return NULL;
    
    int nRequestBufferLen = 0;
	int used;

    /*add method length*/
    pHttpReq->wRequestBufferLen = 0;
    if(strncmp(pHttpReq->szVersion, "1.1", 3) == 0) {
        pHttpReq->wRequestBufferLen = strlen(pHttpReq->szMethod);
        pHttpReq->wRequestBufferLen += (pHttpReq->url).wRUrlLen;
        pHttpReq->wRequestBufferLen += strlen(pHttpReq->szVersion);
        pHttpReq->wRequestBufferLen += strlen((pHttpReq->url).szSite);
        pHttpReq->wRequestBufferLen += 19;
    }
    else {
        pHttpReq->wRequestBufferLen = strlen(pHttpReq->szMethod);
        pHttpReq->wRequestBufferLen += (pHttpReq->url).wUrlLen;
        pHttpReq->wRequestBufferLen += strlen(pHttpReq->szVersion);
        pHttpReq->wRequestBufferLen += 11;
    }

    int i = 0;
    /*add params and values length*/
    for(i = 0; i < pHttpReq->byParamsSize; i++) {
        pHttpReq->wRequestBufferLen += strlen(pHttpReq->szNames[i]);
        pHttpReq->wRequestBufferLen += strlen(pHttpReq->szValues[i]);
        pHttpReq->wRequestBufferLen += 4;
    }

    /*malloc memory*/
    pHttpReq->sRequestBuffer = (char *)calloc(1, pHttpReq->wRequestBufferLen + 1);
    /*construct request buffer*/
	used = strlen(pHttpReq->szMethod);
    strncpy(pHttpReq->sRequestBuffer, pHttpReq->szMethod, used);
    nRequestBufferLen = used;
    
    strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, " ", 1);
    nRequestBufferLen++;

    if(strncmp(pHttpReq->szVersion, "1.1", 3) == 0) {
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen,
                (pHttpReq->url).sRUrl, (pHttpReq->url).wRUrlLen);
        nRequestBufferLen += (pHttpReq->url).wRUrlLen;

        strncpy(pHttpReq->sRequestBuffer+nRequestBufferLen, " ", 1);
        nRequestBufferLen++;

        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, "HTTP/", 5);
        nRequestBufferLen += 5;

        used = strlen(pHttpReq->szVersion);
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, 
                pHttpReq->szVersion, used);
        nRequestBufferLen += used;

        strncpy(pHttpReq->sRequestBuffer+nRequestBufferLen, "\r\n", 2);
        nRequestBufferLen += 2;

        strncpy(pHttpReq->sRequestBuffer+nRequestBufferLen, "Host: ", 6);
        nRequestBufferLen += 6;

        used = strlen(pHttpReq->url.szSite);
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, (pHttpReq->url).szSite, used);
        nRequestBufferLen += used;

        strncpy(pHttpReq->sRequestBuffer+nRequestBufferLen, "\r\n", 2);
        nRequestBufferLen += 2;
    }
    else {
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen,
                (pHttpReq->url).sUrl, (pHttpReq->url).wUrlLen);
        nRequestBufferLen += (pHttpReq->url).wUrlLen;

        strncpy(pHttpReq->sRequestBuffer+nRequestBufferLen, " ", 1);
        nRequestBufferLen++;

        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, "HTTP/", 5);
        nRequestBufferLen += 5;

        used = strlen(pHttpReq->szVersion);
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, 
                pHttpReq->szVersion, used);
        nRequestBufferLen += used;

        strncpy(pHttpReq->sRequestBuffer+nRequestBufferLen, "\r\n", 2);
        nRequestBufferLen += 2;
    }
    /*add param names and values into request buffer*/
    for(i = 0; i < pHttpReq->byParamsSize; i++) {
		used = strlen(pHttpReq->szNames[i]);
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, pHttpReq->szNames[i], used);
        nRequestBufferLen += used;
       
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, ": ", 2);
        nRequestBufferLen += 2;
        
		used = strlen(pHttpReq->szValues[i]);
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, pHttpReq->szValues[i], used);
        nRequestBufferLen += used;
       
        strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, "\r\n", 3);
        nRequestBufferLen = nRequestBufferLen + 2;
    }
    strncpy(pHttpReq->sRequestBuffer + nRequestBufferLen, "\r\n", 3);//add '\0' to end
    nRequestBufferLen += 2;
    
    return pHttpReq->sRequestBuffer;
}


void ABU_HTTP_REQ_free(ABU_HTTP_REQ_S ** pHttpReq)
{
    /*assert(pHttpReq != NULL);*/
    if(pHttpReq == NULL)
        return;

    ABU_FREE((*pHttpReq)->sRequestBuffer);
    ABU_FREE(*pHttpReq);
}
