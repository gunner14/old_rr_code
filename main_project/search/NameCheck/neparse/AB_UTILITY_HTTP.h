/*******************************************************************************
* File name: AB_UTILITY_HTTP.h
* Author: Zheng Xueshuang  Version: 0.1  Date: 12/30/2006
* Description: 
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

#ifndef _AB_UTILITY_HTTP_H
#define _AB_UTILITY_HTTP_H

#include "AB_UTILITY_COMMON.h"

typedef struct ABU_URL_S {
    char  sUrl[MAX_URL_LEN];
    UINT16 wUrlLen;
    char  szSite[MAX_SITE_LEN];
    unsigned short  nPort;
    char  sRUrl[MAX_URL_LEN];
    UINT16 wRUrlLen;
} ABU_URL_S;

typedef struct ABU_HTTP_REQ_S {
    struct ABU_URL_S url; 
    char  szMethod[MAX_HTTPMETHOD_LEN];       /*inparam: GET, POST, HEAD,*/
    char  szVersion[MAX_HTTPVERISON_LEN]; 	  /*inparam:  1.0/1.1*/
    char  szNames[MAX_HTTP_PARAMS_SIZE][MAX_HTTPPARAM_NAME_LEN];
    char  szValues[MAX_HTTP_PARAMS_SIZE][MAX_HTTPPARAM_VALUE_LEN];
    UINT8 byParamsSize;
    char  * sRequestBuffer; /* a complete http request */
    UINT16 wRequestBufferLen; 
} ABU_HTTP_REQ_S;

/********************************************************************************
* Function:       ABU_HTTP_parse
* Description:    construct a ABU_URL_S structure using given url
* Input:          sUrl --in parameter, not null, char *
* Output:         pUrlStruct --out parameter, a struct of ABU_URL_S         
* Return:         the constructed ABU_URL_S
*********************************************************************************/

ABU_URL_S * ABU_HTTP_parse(const char * sUrl, int nUrlLen, ABU_URL_S * pUrlStruct/* out */);

/********************************************************************************
* Function:       ABU_HTTP_REQ_create
* Description:    create a http request structure ABU_HTTP_REQ_S
* Input:          sUrl -- the request url, not null, char *
*                 method -- the method of request, not null, char*
*                 version -- the http version 1.0 or 1.1, not null, char *
* Output:
* Return:        the constructed ABU_HTTP_REQ_S
********************************************************************************/
ABU_HTTP_REQ_S * ABU_HTTP_REQ_create(const char * sUrl, int nUrlLen, char * method, char * version);


/*******************************************************************************
* Function:       ABU_HTTP_REQ_addParam
* Description:    add param and value to ABU_HTTP_REQ_S structure
* Input:          pHttpReq  -- the pointer structure of ABU_HTTP_REQ_S, not null
*                 szParamName  -- the request parameter, not null, char *
*                 szParamValue -- the value of parameter, not null char *
* Output:         pHttpReq --the structure of ABU_HTTP_REQ_S that 
*                            added param and value
*
* Return:         0 -- success; not 0 ----- fail.
*******************************************************************************/
int  ABU_HTTP_REQ_addParam(ABU_HTTP_REQ_S * pHttpReq, const char * szParamName, 
                                                      char * szParamValue);


/*******************************************************************************
* Function:       ABU_HTTP_REQ_getBuf
* Description:    construct a reqeust string using given ABU_HTTP_REQ_S
* Input:          pHttpReq -- the structure of ABU_HTTP_REQ_S that to construct 
*                             request string, not null, ABU_HTTP_REQ_S *
* 
* Output:
* Return:         the request string
*******************************************************************************/
char * ABU_HTTP_REQ_getBuf(ABU_HTTP_REQ_S *pHttpReq);


/******************************************************************************
* Function:       ABU_HTTP_REQ_free
* Description:    free the memory of ABU_HTTP_REQ_free and sRequestBuffer
* Input:          pHttpReq -- the pointer's pointer of ABU_HTTP_REQ_s
* 
* Output:
* Return:
******************************************************************************/
void ABU_HTTP_REQ_free(ABU_HTTP_REQ_S ** pHttpReq);

#endif
