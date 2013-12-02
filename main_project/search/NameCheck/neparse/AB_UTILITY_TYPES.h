#ifndef _AB_UTILITY_TYPES_H_
#define _AB_UTILITY_TYPES_H_

#include <time.h>

#include "AB_UTILITY_COMMON.h"

#define	URL_STATUS_NEW 	       1

// only for fetching phase; meaningless for linkdb&webdb 
#define	URL_STATUS_FETCHING  3

#define	URL_STATUS_SUCC       2
#define	URL_STATUS_FAIL_REENTRY_GROUP  0x10
// No resovled IP available 
#define	URL_STATUS_REENTRY_DNS_UNHIT URL_STATUS_FAIL_REENTRY_GROUP+3    

// HTTP Server temporary errors, 503
#define	URL_STATUS_REENTRY_SERVER_ERROR URL_STATUS_FAIL_REENTRY_GROUP+1 

// No resource, or other internal errors
#define	URL_STATUS_REENTRY_INTERNAL_ERROR URL_STATUS_FAIL_REENTRY_GROUP+2 

#define	URL_STATUS_FAIL_FATAL_GROUP  0x20
#define	URL_STATUS_FATAL_SERVER_ERROR URL_STATUS_FAIL_FATAL_GROUP+1
#define	URL_STATUS_FATAL_INVALID_URL  URL_STATUS_FAIL_FATAL_GROUP+2

/* only used for query */
#define URL_STATUS_QUERY_NEW_GROUP 0x40
#define URL_STATUS_QUERY_SUCC_GROUP 0x80


typedef struct LINK_INFO_ITEM_S 
{
    // Url
	UINT32 dwSiteSign;
    char  sUrl[MAX_URL_LEN];		
    UINT8 byLenOfUrl;

    UINT8  byCountOfRedirect; // the count of redirecting if the url is 
                              //a redirected url. System can define the 
                              //maximum allowed redirect count. 

    // Redirect URL 
    char sRedirectFromUrl[MAX_URL_LEN];
    UINT8 byLenOfRedirectFromUrl;	// when it is > 0, the page is redirected


    /**
	  @desc Url&Page Meta Info, from Parser. Total 16 bit.
	       Lowest 0~ 5th bits   : weight: 0~64
	       6th and 7th  bits  : type of the page, 0b00: index; 0b01: content; others reserved
	       8th~ 12th bits    : url depth: 0~64
	       13th~15th bits    : encode
             		0: 0b000: UNKNOWN 
  				1: 0b001: ENG
  				4:0b100: GB10830
  				5:0b101: GB2312
  				6:0b110: GBK
  				2: 0b010: BIG5
  				3: 0b011: UTF8
    */

    UINT16 wUrlMeta;

    // HTTP Info
    time_t lLastModifiedTime;

 

    //  DB Info
    UINT16 status;	
    time_t lLastTryTime;		//上一次抓取时间
    UINT32 dwCountOfTry;		//总共抓取次数
    UINT32 dwCountOfFailure;    //失败的次数
    time_t lFirstAddedTime;		//添加到linkDB的时间
    char    sReserved[64]; //保留以后使用   // 596 byte
} LINK_INFO_ITEM_S;

typedef struct PAGE_BUFFER_ITEM_S
{
    char   sUrl[MAX_URL_LEN];
    UINT8  byLenOfUrl;
    char    sPageBuffer[MAX_PAGE_LEN];     // Include HTTP Response Header
    UINT32  dwLenOfPageBuffer;
    UINT16   urlStatus;
} PAGE_BUFFER_ITEM_S;

typedef struct PAGE_INFO_ITEM_S 
{
    // Link
    LINK_INFO_ITEM_S url;

    // DB Info
    time_t lFirstUpdateTime;
    time_t lLastUpdateTime;
    UINT16 wTotalFetchTimes;

    /**
	@desc The structure of the buffer
	      0b00: NULL; 0b01: Links; 0b10: HTML; 0b11: Links + HTML
    */
    UINT8  byStructOfBuffer;   	
    UINT32 dwLenOfBuffer;

    /**
	@desc the 64bit sign of the HTML page. It is used to find same pages later.
    */
    UINT32 dwHTMLSign1;
    UINT32 dwHTMLSign2;

    /**
	@desc reserved for the future
    */
    char    sReserved[64];


    /**
	@desc The length of the buffer when Links Part is compressed. 
	       It is only used by webinfoDB for saving page. 
	       If it is >0, the structure of the buffer is like:
	       Cmpressed Links Buffer + HTML part
    */
    UINT32 dwLenOfZippedLinksBuffer; 
    /**
	@desc The length of the buffer when HTML is compressed in the buffer. 
	      It is only used by webinfoDB for saving page.  
	      If it is > 0, the structure of the buffer is like: 
         Links Part + Zipped HTML Page.
    */
    UINT32 dwLenOfZippedHTMLBuffer;  

    /**
	  @desc The following data are length-variable and may be serialized to 
       ONE buffer of type char * before transmiting , storing and compressing. 
	   The data in the buffer are organized as when uncomprssed: 
	   Anchor will be replaced by ‘0’ if its length is 0.
	   Inurl1\r\nAnchor1\r\ninurl2\r\nAnchor2\r\n…   inurlN\r\nAnchorN\r\n\r\n
	   Outurl1\r\nAnchor1\r\nouturl2\r\nAnchor2\r\n…   outurlN\r\nAnchorN\r\n\r\n
       Html Page.
	   The maximum len of the buffer is : 
	   MAX_PAGE_LEN +  MAX_PAGEWIDTH_SIZE*(MAX_URL_LEN+MAX_ANCHOR_LEN+4) +4 
    */

    char * sBuffer; // Max Length = [MAX_PAGE_LEN + 
                    //MAX_PAGEWIDTH_SIZE*(MAX_URL_LEN+MAX_ANCHOR_LEN+4] +4]
} PAGE_INFO_ITEM_S;


#endif
