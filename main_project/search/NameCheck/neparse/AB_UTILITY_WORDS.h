/************************************************************
*Filename: AB_UTILITY_WORDS.h
*Author:bfzhang
*Date: 2007-01-20
*Version:
*（1）0.1v 2007-01-20 实现中文分词基本功能
*（2）0.2v 2007-02-09 给中文分词具有词性标注功能,词性标注遵循北大标准/中科院标准
*（3）0.3v 2007-03-22 修改bug，增加AB_UTILITY_WORDS_UTILITY.h，以提供外部接口
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
	BOOL isNextNeSepa;//表征此词下一个位置是否是否为NE识别分隔符
}ABU_WORDS_WORD_S;


/***pos tag defination***************************************/
/**
@desc:a	形容词 	取英语形容词 adjective的第1个字母。 
*/
extern const char *POS_A ;

/**
@desc:b	区别词 	取汉字“别”的声母。 
*/
extern const char *POS_B;

/**
@desc:c	连词 	取英语连词 conjunction的第1个字母。 
*/
extern const char *POS_C ;


/**
@desc:d	副词 	取 adverb的第2个字母，因其第1个字母已用于形容词。 
*/
extern const char *POS_D ;

/**
@desc:e	叹词 	取英语叹词 exclamation的第1个字母。 
*/
extern const char *POS_E;

/**
@desc:f	方位词 	取汉字“方” 
*/
extern const char *POS_F;

/**
@desc:g	语素 	绝大多数语素都能作为合成词的“词根”，取汉字“根”的声母。 
*/
extern const char *POS_G;

/**
@desc:h	前接成分 	取英语 head的第1个字母。 
*/
extern const char *POS_H;

/**
@desc:i	成语 	取英语成语 idiom的第1个字母。 
*/
extern const char *POS_I;

/**
@desc:j	简称略语 	取汉字“简”的声母。 
*/
extern const char *POS_J ;

/**
@desc:k	后接成分
*/
extern const char *POS_K;

/**
@desc:l	习用语 	习用语尚未成为成语，有点“临时性”，取“临”的声母。 
*/
extern const char *POS_L ;

/**
@desc:m	数词 	取英语 numeral的第3个字母，n，u已有他用。 
*/
extern const char *POS_M ;


/**
@desc:n	名词 	取英语名词 noun的第1个字母。 
*/
extern const char *POS_N ;


/**
@desc:o	拟声词 	取英语拟声词 onomatopoeia的第1个字母。 
*/
extern const char *POS_O ;

/**
@desc:p	介词 	取英语介词 prepositional的第1个字母。 
*/
extern const char *POS_P ;

/**
@desc:q	量词 	取英语 quantity的第1个字母。 
*/
extern const char *POS_Q ;

/**
@desc:r 代词 	取英语 代词pronoun的第二个字母，因p已用于介词
*/
extern const char *POS_R;

/**
@desc:s	处所词 	取英语 space的第1个字母。 
*/
extern const char *POS_S ;

/**
@desc:t	时间词 	取英语 time的第1个字母。 
*/
extern const char *POS_T ;

/**
@desc:u	助词 	取英语助词 auxiliary 
*/
extern const char *POS_U;

/**
@desc:v	动词 	取英语动词 verb的第一个字母。 
*/
extern const char *POS_V;

/**
@desc:w	标点符号,在北大标准中标点只是指中文标点
@Note:此处扩展了标点的定义,标点包括中文标点和英文标点
*/
extern const char *POS_W;

/**
@desc:x	非语素字 	非语素字只是一个符号，字母 x通常用于代表未知数、符号。 
*/
extern const char *POS_X;

/**
@desc:y	语气词 	取汉字“语”的声母。 
*/
extern const char *POS_Y;

/**
@desc:z	状态词 	取汉字“状”的声母的前一个字母。 
*/
extern const char *POS_Z;

/**
@desc:un	未知词	不可识别词及用户自定义词组。
            取英文Unkonwn首两个字母。(非北大标准，CSW分词中定义)
*/
extern const char *POS_UN;

/******下面为自定义*******************************************/

/**
@desc:ns表示地名,ns1指省级行政单位
*/
extern const char *POS_NS1;

/**
@desc:ns表示地名,ns2指地级行政单位
*/
extern const char *POS_NS2;

/**
@desc:ns表示地名,ns3指县级行政单位
*/
extern const char *POS_NS3;


/**
@desc:aba表示阿拉伯数字,Arabia前三个字母,即此词由"0-9和"-"构成
      方便电话号码识别
*/
extern const char *POS_ABA;

/**
@desc:eng表示英文单词,拼音,或者英文标识符(即由a-z组成)
*/
extern const char *POS_ENG;


/**
@desc:扩展用于地址识别
*/
extern const char *POS_ADDR;


#endif
