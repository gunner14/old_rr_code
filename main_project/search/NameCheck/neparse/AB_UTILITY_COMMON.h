#ifndef ABSpider_H
#define ABSpider_H

#define MAX_URL_LEN  256
#define MAX_SITE_LEN 64
#define MAX_ANCHORTEXT_LEN  64
#define MAX_PAGE_LEN 180*1024
#define MAX_PAGEWIDTH_SIZE  512
#define MAX_HTTP_HEADER_SIZE  4096
#define MAX_HTTP_ENCODETYPE_LEN 32
#define MAX_HTML_TITLE_LEN  256
#define IP_LEN 16
#define MAX_RESERVEDDATA_LEN 128
#define MAX_FILE_LEN 128
#define MAX_HTTPMETHOD_LEN 8
#define MAX_HTTPVERISON_LEN 8
#define MAX_HTTPHEADER_LEN 256
#define MAX_SCHEME_LEN  16
#define MAX_HTTP_PARAMS_SIZE 16
#define MAX_HTTPPARAM_NAME_LEN 16
#define MAX_HTTPPARAM_VALUE_LEN 64
#define MAX_CHARSET_LEN  	256			
#define MAX_ENCODING_LEN 	256				
#define MAX_AGENT_LEN  		256		
#define MAX_IPGROUP_NUM 10	


/** 
  * @brief the common type defintions
  */

/*edit by xhe, 2007 1.16*/
//typedef char bool;
#include <stdbool.h>

typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef char BOOL;      		//0 false, 1 true
typedef unsigned char UINT8;
typedef unsigned long long UINT64; 


/** 
  * @brief the common macros defintions
  */
#define ABU_FREE(x) if (x) { free(x); x=NULL;} 
#ifdef AB_DEBUG
#define ABU_ASSERT(x) { if (!(x)) \
    { fprintf(stderr, "%s-%d: ABU_ASSERT failed!\n", __FILE__, __LINE__); exit(-1);} } 
#else
#define ABU_ASSERT(x)
#endif

#endif
