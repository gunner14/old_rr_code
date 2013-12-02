/**
 * 
 * $Id: client_lib.h,v 1.3 2007/07/26 06:25:36 yhwu Exp $
 *
 */

#ifndef _CLIENT_LIB_H_
#define _CLIENT_LIB_H_

#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_PACK.h"
#include "AB_UTILITY_LOG.h"

#include "AB_DWEBDB_TYPES.h"

#include "manage_pcqueue.h"
#include "manage_log.h"

#define MAX_DB_NUM 32
#define READ_TIME_OUT 300
#define DEF_GET_PAGE_NUM 1024
#define DEF_DEL_PAGE_NUM 1024
#define MAX_CONTENT_LEN (1<<20) //1MB
#define MAX_LINK_LEN (128<<10) //256*512

#define PAGE_QUEUE_LEN (1<<10) //8192
#define DECPAGE_QUEUE_LEN  256

#define READ_PACK_BUF_SIZE_FOR_PAGE (1<<20) 
#define READ_PACK_BUF_SIZE_FOR_ADDR 8192

/**
 * Encapsulation Struct.
 */

typedef struct clientlib_conf_info{
    char dc_ip[IP_LEN];
    int dc_port;
}CLIENTLIB_CONF_INFO_S;

//--------------------------------------
//for get dbinfos..
typedef struct getdbs_taskinfo{
    int dc_socket;
    int db_num;
    DB_INFO_S *db_info_array;
}GETDBS_TASKINFO_S;

//-----------------------------------------
//record the cache status
typedef enum cache_status{
    CACHE_MISS = 0,
    CACHE_HIT = 1
}CACHE_STATUS_E;

//for get dbinfo 
typedef struct getdb_taskinfo{
    int dc_socket;
    char szDomain[MAX_SITE_LEN];
    UINT32 key1;
    UINT32 key2;
    CACHE_STATUS_E cache_status;
    DB_INFO_S db_info;    
}GETDB_TASKINFO_S;

//-----------------------------------
typedef struct page_info{
    int url_len;
    char szUrl[MAX_URL_LEN];
    int link_len;
    char szLink[MAX_LINK_LEN];
    int content_len;
    char szContent[MAX_CONTENT_LEN];
    time_t addTime;
    UINT32 blockIdx1;
    UINT32 blockIdx2;
}PAGE_INFO_S;

typedef struct getonepage_taskinfo{
    DB_INFO_S db_info;
    int db_socket;
    int request_url_len;
    char request_url[MAX_URL_LEN];
    UINT32 blockIdx1;
    UINT32 blockIdx2;
    int showContent;
    int compressPack;
    int requestPart;
    FILE *pRecordFile;
}GETONEPAGE_TASKINFO_S;

typedef struct getbydomain_taskinfo{
    DB_INFO_S db_info;
    int db_socket;
    int request_domain_len;
    char request_domain[MAX_SITE_LEN];
    UINT32 blockIdx1;
    UINT32 blockIdx2;
    int requestNum;
    int gotPageNum;
    int showContent;
    int compressPack;
    int requestPart;
    int decompressNum;
    PCQUEUE_S *pPageQueue;
    PCQUEUE_S *pDecompressQueue;
    FILE *pRecordFile;
}GETBYDOMAIN_TASKINFO_S;

typedef struct getbyseq_taskinfo{
    DB_INFO_S db_info;
    int db_socket;
    UINT32 blockIdx1;
    UINT32 blockIdx2;
    int requestNum;
    int gotPageNum;
    int showContent;
    int compressPack;
    int requestPart;
    int decompressNum;
    PCQUEUE_S *pPageQueue;
    PCQUEUE_S *pDecompressQueue;
    FILE *pRecordFile;
}GETBYSEQ_TASKINFO_S;

/**********************************
 * Structs Defined for GetPagesBySeq From MultiDBs
 *********************************************/
/**
 * GetStatus For Each DB.
 */
typedef enum got_status{
    RUNNING = 0,
    FINISHED = 1,
}GOT_STATUS_E;

//record each db's status
typedef struct getbyseq_record_item{
    char db_ip[IP_LEN];
    int db_port;
    int db_socket;
    UINT32 blockIdx1;
    UINT32 blockIdx2;
    GOT_STATUS_E status;
    int gotPageNum;
}GETBYSEQ_RECORD_ITEM_S;

typedef struct getbyseq_record{
    int totalPageNum;
    int requestPageNum;
    int db_num;
    GETBYSEQ_RECORD_ITEM_S *record_item_array;
}GETBYSEQ_RECORD_S;

//--------------
CLIENTLIB_CONF_INFO_S *g_p_ClientLib_ConfInfo;

/*********************************************************************************
 *
 *  Exported API
 *
 ********************************************************************************/

/**
 * Init ClientLib with DC ConfInfo File's URL.
 * Thus We Can Get the DC's ip&port
 */
int InitClientLib(char *dc_ip, int dc_port);
int DestroyClientLib();

/**
 * Return the dbinfo for the specified operation.
 */
int 
GetDBInfo_ByUrl(
        char *szUrl, 
        int url_len,
        char *db_ip,
        int *p_db_port
        );

int 
GetDBInfo_ByDomain(
        char *szDomain, 
        int domain_len, 
        char *db_ip, 
        int *p_db_port
        );

int 
GetDBsInfo( 
        int *p_db_num, 
        DB_INFO_S **ppDBInfo 
        );

/**
 * get pages by seq.
 * clientlib possesses a queue to store pages got from different db.
 */

int 
Start_GetCompressedPages(
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo
        );

int GetOneCompressedPageFromQueue(
        PAGE_INFO_S *pPageInfo,
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo);

int 
End_GetCompressedPages(
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo 
        );

int 
Start_GetDecompressedPages(
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo
        );
int 
End_GetDecompressedPages(
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo 
        );
int 
Start_GetPagesBySeq(
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo
        );
int
GetOnePageFromQueue_BySeq(
        PAGE_INFO_S *pPageInfo, 
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo
        );
int 
End_GetPagesBySeq(
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo 
        );

int 
Start_GetOnePageByUrl(
        GETONEPAGE_TASKINFO_S *pGetOnePageTaskInfo
        );

int
Start_GetPagesByDomain(
        GETBYDOMAIN_TASKINFO_S *pGetByDomainTaskInfo
        );

int 
GetOnePage_ByUrl(
        PAGE_INFO_S *pPageInfo, 
        GETONEPAGE_TASKINFO_S *pGetOnePageTaskInfo
        );

int 
GetOnePageFromQueue_ByDomain(
        PAGE_INFO_S *pPageInfo, 
        GETBYDOMAIN_TASKINFO_S *pGetByDomainTaskInfo
        );
int 
End_GetOnePageByUrl(
        GETONEPAGE_TASKINFO_S *pGetOnePageTaskInfo
        );

int 
End_GetPagesByDomain(
        GETBYDOMAIN_TASKINFO_S *pGetByDomainTaskInfo
        );


/**
 * PACK APIs
 */
int WriteGetDBByDomainReq();
int WriteGetDBsReq();
int ReadGetDBAck();
int ReadGetDBsAck(GETDBS_TASKINFO_S *pTaskInfo);

int ReadGetPageAck(char *pack_read_buf, PAGE_INFO_S *pPageInfo, int db_socket, UINT32 *pBlockIdx1, UINT32 *pBlockIdx2);

/*****************************************************************
 * 
 * Inner APIs
 *
 *****************************************************************/
/**
 * Get APIs
 */
int WriteGetOnePageByUrlReq ( 
        GETONEPAGE_TASKINFO_S *pGetOnePageTaskInfo 
        );
int WriteGetPagesByDomainReq ( 
        GETBYDOMAIN_TASKINFO_S *pGetByDomainTaskInfo 
        );
int WriteGetPagesBySeqReq ( 
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo 
        );
int ParsePackToPage ( 
        ABU_PACK_S *pPagePack, 
        PAGE_INFO_S *pPageInfo, 
        int compress_pack 
        );
int ReadGetOnePageAck( 
        PAGE_INFO_S *pPageInfo, 
        GETONEPAGE_TASKINFO_S *pGetOnePageTaskInfo
        );
int ReadGetPagesByDomainAck ( 
        GETBYDOMAIN_TASKINFO_S *pGetByDomainTaskInfo,
        char *buffer, 
        int buf_len 
        );

void *
GetDomainDecompressPages(
        void *userData
        );

int ReadGetPagesBySeqAck(
        GETBYSEQ_TASKINFO_S *pGetBySeqTaskInfo, 
        char *buffer, 
        int buf_len
        );
void * GenPagesProcess ( void *userData );
void * GenDecompressPages(
        void *userData
        );
void *
GetPagesByDomainProcess(
        void *userData
        );

/**
 * Conf&Init APIs
 */
int Init_ClientConf(char *dc_ip, int dc_port);
void Free_ClientConf();

/**
 * Func APIs
 */
int GetDomainNameByUrl(char *domain_name, char *url_buf, int url_len);
int GetDCSocket();
/**
 * Cache APIs
 */
void CLIENT_CACHE_LOCK_READING();
void CLIENT_CACHE_LOCK_WRITING();
void CLIENT_CACHE_UNLOCK_RWLOCK();
int Init_ClientCache( );
int GetDBInfo_FromCache( GETDB_TASKINFO_S *pTaskInfo );
int PutDBInfo_ToCache( );
int UpdDBInfo_FromCache( void *pValue, void *userData );
void Free_ClientCache( );
void *UpdateClientCache( void * userData);
int UpdateCacheByDC();

#endif
