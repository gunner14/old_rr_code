#ifndef _AB_UTILITY_HTMLPARSE_H
#define  _AB_UTILITY_HTMLPARSE_H

#include "AB_UTILITY_COMMON.h"

#define MAX_TITLE_LEN 255
#define MAX_BODY_LEN 180*1024

/*
    Aibang.com Confidential Proprietary
    (c) Copyright Aibang.com 2007, All Rights Reserved
    $Id: AB_UTILITY_HTMLPARSE.h,v 1.6 2007/03/06 08:30:01 xszheng Exp $

*/
/** 
  * @file AB_UTILITY_HTMLPARSE.h
  * @brief to define html page parse reult structure an function 
  * @author Xueshuang Zheng
  * @date 2007-03-02
  */  

/*
 * @brief the html parse result structure
 *        sTitle: the page tile
 *        byLenOfText: the title length
 *        sBody: the html body text(remove the html label) 
 *        wLenOfBody: the body length
 */
typedef struct {
    char sTitle[MAX_TITLE_LEN];
    UINT8 byLenOfTitle;
    char sBody[MAX_BODY_LEN];
    UINT16 wLenOfBody;
} HTML_PARSE_RESULT_S;


/**
 * @name ABU_HTMLPARSE_escapeHashTableExist
 * @desc whether the html escape hash exist
 * @param none
 * @return 0: exist
 *         1: not exist, must init hash table 
 */
int ABU_HTMLPARSE_escapeHashTableExist();

/**     
 * @name     initEscapeTable
 * @desc     create a html escape hash table
 * @param    none
 * @return   0: success
 *           <0: failed
 */
int ABU_HTMLPARSE_initEscapeHashTable();

/**
 * @name     destroyEscapeTable
 * @desc     destroy a html escape hash table
 * @param    none
 * @return   none
 */
void ABU_HTMLPARSE_destroyEscapeTable();

/**
 * @name ABU_HTMLPARSE_convertToGB
 * @desc convert a given encode character sequece to GB2312 sequece
 * @param szEncode the original encode of character sequece, in param
 * @param sInBuf the character sequence to convert into GB, in param
 * @param nInBufLen the length of character sequece, in param
 * @param sOutBuf the GB character sequece of sInBuf, out param
 * @param nOutBufLen the length of sOutBuf, in/out param
 * @return 0: success 
 *         <0: failed
 */
int ABU_HTMLPARSE_convertToGB(char *szEncode, char *sInBuf, int nInBufLen,
                                              char *sOutBuf, int *nOutBufLen);

/**
 * @name ABU_HTMLPARSE_unicodeConvert
 * @desc convert unicode &#DD; to gb2312
 * @param szUnicode: the string of unicode, like &#DD;, in param
 * @param sOut: the gb2312 of unicode, out param
 * @param nOutLen: the length of sOut, in/out param
 * @return 0: success
 *         <0: failed
 */
int ABU_HTMLPARSE_unicodeConvert(const char *szUnicode,
                                       char *sOut, int *nOutLen);

/**
 * @name ABU_HTMLPARSE_unEscape
 * @desc unescape the html escape
 * @param szEscape: the html escape charaters
 * @return success: the original char
 *         failed: -1 
 */
char ABU_HTMLPARSE_unEscape(char * szEscape);

/**
 * @name ABU_HTMLPARSE_parse
 * @desc retrive title and body text from html page 
 *       unescape html escape characters 
 * @param sHtmlPage: the html page buffer, in param
 *        nHtmlPageLen: the length of html page, in param
 *        pResult: the pointer of HTML_PARSE_RESULT_S, out param
 *         
 * @return  0: success
 *          <0: error
 */
int ABU_HTMLPARSE_parse(const char *sHtmlPage, int nHtmlPageLen,
                                       HTML_PARSE_RESULT_S *pResult);

/**         
 * NOTE: the input string is gb2312 encode
 * @name ABU_HTMLPARSE_filtrateText
 * @desc unescape the html escape in sInput, 
 *       convert the unicode to gb2312,
 *       convert SBC to DBC,
 *       remove redundant space and tab
 * @param sInput: the input text string, in param
 * @param nInputLen: the length of input string, in param
 * @param sOutput: the output buffer, out param
 * @param nOutputLen: the output buffer length, in/out param
 * @return 0: success
 *         <0: failed
 */                   
int ABU_HTMLPARSE_filtrateText(const char *sInput, int nInputLen,
                                 char *sOutput, int *nOutputLen);


/**         
 * @name ABU_HTMLPARSE_encodeDetect
 * @desc detect the encode of html page
 * @param sInput: the input text string, in param
 * @param nInputLen: the length of input string, in param
 * @param szEncode: the encode of html page, out param
 * @param nEncodeLen: the szEncode buffer length, in param
 * @return 0: success
 *         <0: failed
 */
int ABU_HTMLPARSE_encodeDetect(const char *sInput, int nInputLen,
                                     char *szEncode, int nEncodeLen);

/**
 * @name ABU_HTMLPARSE_utfCharFiltrate()
 * @desc convert the mid dot, ?a to the char 
 *       that gb2312 could express
 * @param sInput: the input utf char sequence, in param
 * @param nInputLen: the length of sInput, in param
 * @param sOutput: the output utf char sequece, out param
 * @param *nOutputLen: the length of sOutput, in/out param
 * @return 0: success
 *         <0: failed 
 */
int ABU_HTMLPARSE_utfCharFiltrate(const char *sInput, int nInputLen,
                                        char *sOutput, int *nOutputLen);
#endif 
