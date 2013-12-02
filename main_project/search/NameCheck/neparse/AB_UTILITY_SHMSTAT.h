#ifndef _AB_UTILITY_SHMSTAT_H_
#define _AB_UTILITY_SHMSTAT_H_
/*
    Aibang.com Confidential Proprietary
    (c) Copyright Aibang.com 2007, All Rights Reserved
    $Id: AB_UTILITY_SHMSTAT.h,v 1.21 2007/03/19 10:33:47 xszheng Exp $

*/
/** 
  * @file AB_UTILITY_SHMSTAT.h
  * @brief to define some stat info for all modules of Spider.  All the data will
  *        be stored into Shared Memory. The KEY has been defined below. 
  *        Note: All data will be reset after a module is restarted for any reason.
  * @author Jianbo Duan
  * @date 2007-01-29
  */


/** 
  * @brief common server stat data
  *   totalIncomming:  the total number of incomming tcp conns request
  *   totalRefused:  the total number of refused tcp conns request
  *   totalBroken:   the total number of closed tcp conns  for broken(read/write error)
  *   totalTimeout:  the total number of closed tcp conns for timeout 
   *  lastStartTime: the time when the latest server  starts up
  */
typedef struct {
    int totalIncomming;
    int totalRefused;
    int totalBroken;
    int totalTimeout;
    time_t lastStartTime;
} STAT_SERVER_S; 


/** 
  * @brief for LinkDB
  *        SHM KEY: 0x10f8785 
  *        totalPUTs:  the total number of PUT requests
  *        totalGETs:  the total number of GET requests
  *        totalDELs:  the total number of DEL requests
  */
#define STAT_LINKSTATUS_NUM 6
#define STAT_LINKSTATUS_ALL       0        //所有链接
#define STAT_LINKSTATUS_NEW       1        //新加入
#define STAT_LINKSTATUS_SUCC      2        //抓取成功
#define STAT_LINKSTATUS_FAILED_REENTRY 3   //抓取失败准备重试
#define STAT_LINKSTATUS_FAILED_FATAL 4     //抓取失败不再抓取
#define STAT_LINKSTATUS_REFUSED 5
typedef struct {
    STAT_SERVER_S serverStat;
    int totalPUTs;
    int totalGETs;
    int totalDELs;

    int totalLinks[STAT_LINKSTATUS_NUM];
    int totalSite;
} STAT_LINKDB_S;



/** 
  * @brief for WEBDB 
  *        SHM KEY: 0x30f8785 
  *        totalPUTs:  the total number of PUT requests
  *        totalGETs:  the total number of GET requests
  *        totalDELs:  the total number of DEL requests
  */
#define AB_WEBDB_MAX_BLOCKTYPE 10
#define AB_WEBDB_MAX_NUMOFFILE 512
typedef struct {
    int occupiedNum;
    int unoccupiedNum;
    int totalNum;
}BLOCK_TYPE_INFO_S;

typedef struct {
    int isExist;
    BLOCK_TYPE_INFO_S blockInfo[AB_WEBDB_MAX_BLOCKTYPE];
}FILE_BLOCK_INFO_S;

typedef struct {
    STAT_SERVER_S serverStat;
    
    int totalPUTs;
    int totalGETs;
    int totalDELs;

    int totalSucAdds;
    int totalSucUpds;
    int totalSucGets;
    int totalSucDels;
    
    int totalUnSucAdds;
    int totalUnSucUpds;
    int totalUnSucGets;
    int totalUnSucDels;
    
    unsigned long totalPages;
    
    FILE_BLOCK_INFO_S fileBlockInfo[AB_WEBDB_MAX_NUMOFFILE];
    
} STAT_WEBDB_S;




/** 
  * @brief for Parser 
  *        lastStartTime: the time when the latest server  starts up    
  *        totalPages: the total number of processed pages  
  *        totalGBPages: the total number of processed originally GB encoded pages     
  *        totalUTF8Pages: the total number of processed originally UTF8 encoded pages  
  *        totalUTF8Pages: the total number of processed originally GBK encoded pages
  *        totalUTF8Pages: the total number of processed originally BIG5 encoded pages
  *        totalUTF8Pages: the total number of processed originally ENG encoded pages   
  *        totalRedirectPages: the total number of processed redirected pages
  *        totalReConnLinkDB: the total times of reconnecting linkdb
  *        totalReConnWebDB: the total times of reconnecting webdb
  *        totalReConnCrawler: the total times of reconnecting Crawler Server 
  *  
  *        totalRecivePages: the total number of parser recived pages from crawler
  *        totalReciveFailedPages: the total number of parser recived failed pages from crawler
  *        totalSendPages: the total number of parser send pages to webdb
  *        totalSendFailedPages: the total number of parser send failed pages to webdb
  *        totalSendUrl: the total url of parser send to linkdb
  *        totalSendFailedUrl: the total url of parser send failed to linkdb
  *        totalSendDelUrl: the total del url of parser send to linkdb
  *        totalSendLinks: the total links the parser send to linkdb
  *        totalSendFailedLinks: the total links the parser send failed to linkdb
  *        currentPageQueue: the current page queue length
  *        currentResultQueue: the current result queue length
  *        SHM KEY: 0x20f8785 
  */
typedef struct {
	  time_t lastStartTime;
	  
    int totalPages;
    int totalGBPages;
    int totalUTF8Pages;
    int totalGBKPages;
    int totalBIGPages;
    int totalENGPages;
    int totalRedirectPages;
    int totalReConnLinkDB;
    int totalReConnWebDB;
    int totalReConnCrawler;

    int totalRecivePages;
    int totalReciveFailedPages;
    int totalSendPages;
    int totalSendFailedPages;
    int totalSendUrl;
    int totalSendFailedUrl;
    int totalSendDelUrl;
    int totalSendLinks;
    int totalSendFaildLinks;
    int currentPageQueue;
    int currentResultQueue;
    
    int successAnalyzePages;
    int faileAnalyzePages;
    int getPagesFromQueue;
    
    int recivePageLengthZeroNum;
    int getQueuePageLengthZeroNum;
}STAT_PARSER_S;


/** 
  * @brief  for Crawler Server
  *        totalUrlsSentToCC: the total urls sent to CrawlerClient
  *        totalSuccPagesSentFromCC: the total pages got from CrawlerClient
  *        totalUrlsDNSMissed: the total urls ignored by DNS rsolving
  *        totalUrlsDropped: the total urls ignored by other internal reasons
  *        totalUrlsFromLinkDB: the total urls got from LinkDB
  *        totalPagesToParser: the total pages sent to Parser (supposed to successfully be fetched)
  *        SHM KEY: 0x40f8785 
  */
typedef struct {
    STAT_SERVER_S serverStatOnCC;
    STAT_SERVER_S serverStatOnParser;

    int totalUrlsSentToCC;
    int totalSuccPagesSentFromCC;
    int totalSuccPages;
    int totalUrlsDNSMissed;
    int totalUrlsDropped;

    int totalUrlsFromLinkDB;
    int totalKBytesReceived;
    int KBytesReceivedPerSec;
    
    int LengthOFPageList;

    int totalPagesToParser;
}STAT_CRAWLERSERVER_S;


/** 
  * @brief  for one Crawler Client
  *        totalKBytesReceived: the total kilo-bytes received from web; reset to 0 when it overflows
  *        totalFetchedUrls: the total number of fetched urls
  *        totalSuccFetchedUrls: the total number of fetched urls successfully
  *        lastConnTime: the start time for latest stat point 
  */
typedef struct {
    int ip;
    int totalKBytesReceived;
    int totalFetchedUrls;
    int totalSuccFetchedUrls;
    time_t lastConnTime;
}STAT_CRAWLERCLIENT_S;

/* The maximum amount of CrawlerClient for statistic */ 
#define MAX_STATCC_NUM  100
/** 
  * @brief for all Crawler Clients
  *     SHM KEY: 0x50f8785 
  *     numOfClients: the current amount of Crawler Clients
  *     stat: the stat data array
  */
typedef struct {
    int numOfClients;
    STAT_CRAWLERCLIENT_S stat[MAX_STATCC_NUM];
}STAT_CRAWLERCLIENT_ALL_S;


/** 
  * @brief  for DNS Manager
  *        SHM KEY: 0x60f8785 
  *        totalResolved: the total number of resolved domain names
  *        totalUnresolved: the total number of domain names which could not be resolved or be newly added
  */
typedef struct {
    STAT_SERVER_S serverStat;
    int totalResolved;
    int totalUnresolved;
}STAT_DNSMGR_S;

#endif //_AB_UTILITY_SHMSTAT_H_
