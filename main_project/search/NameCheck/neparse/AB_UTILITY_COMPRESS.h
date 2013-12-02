#ifndef ABU_COMPRESS_H
#define ABU_COMPRESS_H

#include <AB_UTILITY_COMMON.h>

/**
	@desc 压缩sInBuf到sOutBuf中。如果sOutBuf大小不够,只压缩到dwOutBufSize指定的大小。剩余部分将直接丢弃。
    @param 
        输入参数: const char * sInBuf   指向待压缩的数据的指针
                  UINT32 dwInBufSize    待压缩数据数据大小
                  UINT32 * dwOutBufSize 用于存放压缩后数据的缓冲区大小
        输出参数：char * sOutBuf        指向压缩后数据存放缓冲区的指针
                  UINT32 * dwOutBufSize 压缩后数据放入sOutBuf中的数据大小
   @return 指向压缩缓冲区的指针 解压成功
           NULL                 解压失败
*/
char * ABU_COMPRESS_pack(const char * sInBuf, UINT32 dwInBufSize, char * sOutBuf, /* out */ UINT32 * dwOutBufSize /* in/out */);      // return sOutBuf


/**
	@desc 解压缩sInBuf到sOutBuf中, 成功返回指针。
	      如果sOutBuf的大小不够，返回NULL,并在dwOutBufSize中指明需要的缓冲区大小。
    @param 
        输入参数: const char * sInBuf   指向待解压的数据的指针
                  UINT32 dwInBufSize    待解压数据数据大小
                  UINT32 * dwOutBufSize 用于存放解压后数据的缓冲区大小
                  UINT32 dwSkip         用于指定解压后的前dwSkip个字节要跳过,默认为0
        输出参数：char * sOutBuf        指向解压后数据存放缓冲区的指针
                  UINT32 * dwOutBufSize 解压后数据放入sOutBuf中的数据大小
    @return 当给定的解压缓冲区不够时返回NULL，但是解压缓冲区中数据仍然有效
            成功解压时，返回解压缓冲区头指针
*/
char * ABU_COMPRESS_unpack(const char * sInBuf, UINT32 dwInBufSize, char * sOutBuf, /* out */ UINT32 * dwOutBufSize /* in/out */,UINT32 dwSkip); 


#endif
