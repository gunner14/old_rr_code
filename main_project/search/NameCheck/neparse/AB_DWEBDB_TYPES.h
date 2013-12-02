#ifndef _AB_DWEBDB_TYPES_H_
#define _AB_DWEBDB_TYPES_H_

#include "AB_UTILITY_COMMON.h"

//define the micros for dict usages...
#define MAX_PAGENUM_PER_PACK 512
#define MAX_BLOCK_SIZE (1<<16)
#define MIN_BLOCK_SIZE (1<<10)

/*************************************************
 *
 * Define Opt_Type.
 *
 */
typedef enum opttype{
    PUT_BY_URL = 1,
    GET_BY_URL = 2,
    DEL_BY_URL = 3,
	GET_BY_DOMAIN = 11,
	DEL_BY_DOMAIN = 12,
	GET_BY_SEQ = 21,
	DEL_BY_SEQ = 22,
    ADD_DB = 31,
    DEL_DB = 32,
    COLLECT_STATUS = 41,
    COLLECT_URL = 42,
    HEARTBEAT = 51
}OPTTYPE_E;

typedef enum bodytype{
    BOTH = 3,
    HTML = 2,
    LINK = 1,
    EITHER = 0
}BODYTYPE_E;

typedef struct db_info{
    char db_ip[IP_LEN];
    int db_port;
}DB_INFO_S;

/************************************************
 * Define Message ID
 *
 */

/**
 * MESSAGE_ID between Client(ClientLib) <--> DB
 */ 

#define GET_PAGE_REQ "DDB_GET_PAGE_REQ"
#define GET_PAGE_ACK "DDB_GET_PAGE_ACK"

#define PUT_PAGE_REQ "DDB_PUT_PAGE_REQ"
#define PUT_PAGE_ACK "DDB_PUT_PAGE_ACK"

#define DEL_PAGE_REQ "DDB_DEL_PAGE_REQ"
#define DEL_PAGE_ACK "DDB_DEL_PAGE_ACK"

/**
 * MESSAGE_ID between ClientLib <--> DC
 */

#define GET_DB_REQ "DDB_GET_DB_REQ"
#define GET_DB_ACK "DDB_GET_DB_ACK"

#define GET_DBS_REQ "DDB_GET_DBS_REQ"
#define GET_DBS_ACK "DDB_GET_DBS_ACK"

/**
 * MESSAGE_ID between Tools <--> DC
 */

#define COLLECT_STATUS_REQ "DDB_COLLECT_STATUS_REQ"
#define COLLECT_STATUS_ACK "DDB_COLLECT_STATUS_ACK"
 
#define ADD_DBS_REQ "DDB_ADD_DBS_REQ"
#define ADD_DBS_ACK "DDB_ADD_DBS_ACK"

#define DEL_DBS_REQ "DDB_DEL_DBS_REQ"
#define DEL_DBS_ACK "DDB_DEL_DBS_ACK"

#define MIGRATE_DOMAIN_REQ "DDB_MIGRATE_DOMAIN_REQ"
#define MIGRATE_DOMAIN_ACK "DDB_MIGRATE_DOMAIN_ACK"

#define SHMSTAT_REQ "DDB_SHMSTAT_REQ"
#define SHMSTAT_ACK "DDB_SHMSTAT_ACK"

#define DOMAIN_INFO_REQ "DDB_DOMAIN_INFO_REQ"
#define DOMAIN_INFO_ACK "DDB_DOMAIN_INFO_ACK"


/**
 * MESSAGE_ID between Tools <--> DB
 */

#define SEQ_REQ "DDB_SEQ_REQ"
#define SEQ_ACK "DDB_SEQ_ACK"

/**
 * MESSAGE_ID between DC <--> DB
 */

#define MIGRATE_PAGES_REQ "DDB_MIGRATE_PAGES_REQ"
#define MIGRATE_PAGES_ACK "DDB_MIGRATE_PAGES_ACK"

#define HEART_BEAT_PACK "DDB_HEART_BEAT_PACK"

#define SHMSTAT_REQ "DDB_SHMSTAT_REQ"
#define SHMSTAT_ACK "DDB_SHMSTAT_ACK"

/**
 * MESSAGE_ID between DB <--> DB
 */
//put page, del page
#endif

