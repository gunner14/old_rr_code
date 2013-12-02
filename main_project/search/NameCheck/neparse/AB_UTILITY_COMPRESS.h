#ifndef ABU_COMPRESS_H
#define ABU_COMPRESS_H

#include <AB_UTILITY_COMMON.h>

/**
	@desc ѹ��sInBuf��sOutBuf�С����sOutBuf��С����,ֻѹ����dwOutBufSizeָ���Ĵ�С��ʣ�ಿ�ֽ�ֱ�Ӷ�����
    @param 
        �������: const char * sInBuf   ָ���ѹ�������ݵ�ָ��
                  UINT32 dwInBufSize    ��ѹ���������ݴ�С
                  UINT32 * dwOutBufSize ���ڴ��ѹ�������ݵĻ�������С
        ���������char * sOutBuf        ָ��ѹ�������ݴ�Ż�������ָ��
                  UINT32 * dwOutBufSize ѹ�������ݷ���sOutBuf�е����ݴ�С
   @return ָ��ѹ����������ָ�� ��ѹ�ɹ�
           NULL                 ��ѹʧ��
*/
char * ABU_COMPRESS_pack(const char * sInBuf, UINT32 dwInBufSize, char * sOutBuf, /* out */ UINT32 * dwOutBufSize /* in/out */);      // return sOutBuf


/**
	@desc ��ѹ��sInBuf��sOutBuf��, �ɹ�����ָ�롣
	      ���sOutBuf�Ĵ�С����������NULL,����dwOutBufSize��ָ����Ҫ�Ļ�������С��
    @param 
        �������: const char * sInBuf   ָ�����ѹ�����ݵ�ָ��
                  UINT32 dwInBufSize    ����ѹ�������ݴ�С
                  UINT32 * dwOutBufSize ���ڴ�Ž�ѹ�����ݵĻ�������С
                  UINT32 dwSkip         ����ָ����ѹ���ǰdwSkip���ֽ�Ҫ����,Ĭ��Ϊ0
        ���������char * sOutBuf        ָ���ѹ�����ݴ�Ż�������ָ��
                  UINT32 * dwOutBufSize ��ѹ�����ݷ���sOutBuf�е����ݴ�С
    @return �������Ľ�ѹ����������ʱ����NULL�����ǽ�ѹ��������������Ȼ��Ч
            �ɹ���ѹʱ�����ؽ�ѹ������ͷָ��
*/
char * ABU_COMPRESS_unpack(const char * sInBuf, UINT32 dwInBufSize, char * sOutBuf, /* out */ UINT32 * dwOutBufSize /* in/out */,UINT32 dwSkip); 


#endif
