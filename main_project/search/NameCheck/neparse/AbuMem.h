#ifndef __AbuMem_H__
#define __AbuMem_H__

#define _DUMP_FORMAT_ "{%-20s}{%-4d}{%-30s}"

#define CALLOC_MEM(size, type, is_debug) \
    ((is_debug == true) ? \
        (CallocMem(size, type, is_debug,\
            "try to calloc at: "_DUMP_FORMAT_"", \
            (char*) __FILE__, (int)__LINE__,(char*)__FUNCTION__)) : \
        (CallocMem(size, type, is_debug)))

#define FREE_MEM(ptr, is_debug) do{ \
    FreeMem(ptr, is_debug); \
}while(0)

void *CallocMem(int size, const char *type, bool is_debug, ...);

void FreeMem(void *ptr, bool is_debug);

#endif //__AbuMem_H__
