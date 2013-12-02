/************************************************************
*Filename: AB_UTILITY_WORDS.h
*Author:bfzhang
*Date: 2007-01-20
*Version:
*��1��0.1v 2007-01-20 ʵ�����ķִʻ�������
*��2��0.2v 2007-02-09 �����ķִʾ��д��Ա�ע����,���Ա�ע��ѭ�����׼/�п�Ժ��׼
*��3��0.3v 2007-03-22 �޸�bug������AB_UTILITY_WORDS_UTILITY.h�����ṩ�ⲿ�ӿ�
* *Description:define some utility funcion in AB_UTILITY_WORDS module
****************************************************/
#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_LOG.h"

#ifndef AB_UTILITY_WORDS_H
#define AB_UTILITY_WORDS_H

/**macor ****************************/
#ifndef TRUE
#define TRUE 1 
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char BYTE;
#define MAX_LINE_LENGTH_4WORDS 1024

//seg result dwSizeOfWordArray's initial size
#define WEB_PAGE_WORD_NUMBER 128 

/**macro for  dict*****************************************/
#define BUCKET_SIZE 10000
#define MAX_WORD_LENGTH 1024
#define MAX_BUFFER_LENGTH (256*1024)
#define CHINESE_CHARACTER_LENGTH  2
#define OTHER_CHARACTER_LENGTH 1
#define	GET_NEXT_COMP_LENGTH(c)   ( (c < 0) ? CHINESE_CHARACTER_LENGTH : OTHER_CHARACTER_LENGTH)

/*****struct for AB_UTILITY_WORDS Module***************/
typedef struct _ABU_WORDS_WORD_S
{
	char *szWord;
	char *szWordPosTag;
	UINT32 dwOffset;
	BOOL isNextNeSepa;//�����˴���һ��λ���Ƿ��Ƿ�ΪNEʶ��ָ���
}ABU_WORDS_WORD_S;


/***pos tag defination***************************************/
/**
@desc:a	���ݴ� 	ȡӢ�����ݴ� adjective�ĵ�1����ĸ�� 
*/
extern const char *POS_A ;

/**
@desc:b	����� 	ȡ���֡��𡱵���ĸ�� 
*/
extern const char *POS_B;

/**
@desc:c	���� 	ȡӢ������ conjunction�ĵ�1����ĸ�� 
*/
extern const char *POS_C ;


/**
@desc:d	���� 	ȡ adverb�ĵ�2����ĸ�������1����ĸ���������ݴʡ� 
*/
extern const char *POS_D ;

/**
@desc:e	̾�� 	ȡӢ��̾�� exclamation�ĵ�1����ĸ�� 
*/
extern const char *POS_E;

/**
@desc:f	��λ�� 	ȡ���֡����� 
*/
extern const char *POS_F;

/**
@desc:g	���� 	����������ض�����Ϊ�ϳɴʵġ��ʸ�����ȡ���֡���������ĸ�� 
*/
extern const char *POS_G;

/**
@desc:h	ǰ�ӳɷ� 	ȡӢ�� head�ĵ�1����ĸ�� 
*/
extern const char *POS_H;

/**
@desc:i	���� 	ȡӢ����� idiom�ĵ�1����ĸ�� 
*/
extern const char *POS_I;

/**
@desc:j	������� 	ȡ���֡��򡱵���ĸ�� 
*/
extern const char *POS_J ;

/**
@desc:k	��ӳɷ�
*/
extern const char *POS_K;

/**
@desc:l	ϰ���� 	ϰ������δ��Ϊ����е㡰��ʱ�ԡ���ȡ���١�����ĸ�� 
*/
extern const char *POS_L ;

/**
@desc:m	���� 	ȡӢ�� numeral�ĵ�3����ĸ��n��u�������á� 
*/
extern const char *POS_M ;


/**
@desc:n	���� 	ȡӢ������ noun�ĵ�1����ĸ�� 
*/
extern const char *POS_N ;


/**
@desc:o	������ 	ȡӢ�������� onomatopoeia�ĵ�1����ĸ�� 
*/
extern const char *POS_O ;

/**
@desc:p	��� 	ȡӢ���� prepositional�ĵ�1����ĸ�� 
*/
extern const char *POS_P ;

/**
@desc:q	���� 	ȡӢ�� quantity�ĵ�1����ĸ�� 
*/
extern const char *POS_Q ;

/**
@desc:r ���� 	ȡӢ�� ����pronoun�ĵڶ�����ĸ����p�����ڽ��
*/
extern const char *POS_R;

/**
@desc:s	������ 	ȡӢ�� space�ĵ�1����ĸ�� 
*/
extern const char *POS_S ;

/**
@desc:t	ʱ��� 	ȡӢ�� time�ĵ�1����ĸ�� 
*/
extern const char *POS_T ;

/**
@desc:u	���� 	ȡӢ������ auxiliary 
*/
extern const char *POS_U;

/**
@desc:v	���� 	ȡӢ�ﶯ�� verb�ĵ�һ����ĸ�� 
*/
extern const char *POS_V;

/**
@desc:w	������,�ڱ����׼�б��ֻ��ָ���ı��
@Note:�˴���չ�˱��Ķ���,���������ı���Ӣ�ı��
*/
extern const char *POS_W;

/**
@desc:x	�������� 	��������ֻ��һ�����ţ���ĸ xͨ�����ڴ���δ֪�������š� 
*/
extern const char *POS_X;

/**
@desc:y	������ 	ȡ���֡������ĸ�� 
*/
extern const char *POS_Y;

/**
@desc:z	״̬�� 	ȡ���֡�״������ĸ��ǰһ����ĸ�� 
*/
extern const char *POS_Z;

/**
@desc:un	δ֪��	����ʶ��ʼ��û��Զ�����顣
            ȡӢ��Unkonwn��������ĸ��(�Ǳ����׼��CSW�ִ��ж���)
*/
extern const char *POS_UN;

/******����Ϊ�Զ���*******************************************/

/**
@desc:ns��ʾ����,ns1ָʡ��������λ
*/
extern const char *POS_NS1;

/**
@desc:ns��ʾ����,ns2ָ�ؼ�������λ
*/
extern const char *POS_NS2;

/**
@desc:ns��ʾ����,ns3ָ�ؼ�������λ
*/
extern const char *POS_NS3;


/**
@desc:aba��ʾ����������,Arabiaǰ������ĸ,���˴���"0-9��"-"����
      ����绰����ʶ��
*/
extern const char *POS_ABA;

/**
@desc:eng��ʾӢ�ĵ���,ƴ��,����Ӣ�ı�ʶ��(����a-z���)
*/
extern const char *POS_ENG;


/**
@desc:��չ���ڵ�ַʶ��
*/
extern const char *POS_ADDR;


#endif
