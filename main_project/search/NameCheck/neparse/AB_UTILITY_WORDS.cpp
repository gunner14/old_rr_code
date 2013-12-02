/********************************************************************
*Filename:AB_UTILITY_WORD.c
*Author:bfzhang
*Date: 2007-01-20
*Description: define the implementation of some utility interface
*********************************************************************/
#include "AB_UTILITY_WORDS.h" 
#include "time.h"
/****pos tag defination************************************************/
/***北大标准*/

/**
@desc:a	形容词 	取英语形容词 adjective的第1个字母。 
*/
const char *POS_A = "a";

/**
@desc:b	区别词 	取汉字“别”的声母。 
*/
const char *POS_B = "b";

/**
@desc:c	连词 	取英语连词 conjunction的第1个字母。 
*/
const char *POS_C = "c";

/**
@desc:d	副词 	取 adverb的第2个字母，因其第1个字母已用于形容词。 
*/
const char *POS_D = "d";

/**
@desc:e	叹词 	取英语叹词 exclamation的第1个字母。 
*/
const char *POS_E = "e";

/**
@desc:f	方位词 	取汉字“方” 
*/
const char *POS_F = "f";

/**
@desc:g	语素 	绝大多数语素都能作为合成词的“词根”，取汉字“根”的声母。 
*/
const char *POS_G = "g";

/**
@desc:h	前接成分 	取英语 head的第1个字母。 
*/
const char *POS_H = "h";

/**
@desc:i	成语 	取英语成语 idiom的第1个字母。 
*/
const char *POS_I = "i";

/**
@desc:j	简称略语 	取汉字“简”的声母。 
*/
const char *POS_J = "j";

/**
@desc:k	后接成分
*/
const char *POS_K = "k";

/**
@desc:l	习用语 	习用语尚未成为成语，有点“临时性”，取“临”的声母。 
*/
const char *POS_L = "l";

/**
@desc:m	数词 	取英语 numeral的第3个字母，n，u已有他用。 
*/
const char *POS_M = "m";

/**
@desc:n	名词 	取英语名词 noun的第1个字母。 
*/
const char *POS_N = "n";


/**
@desc:o	拟声词 	取英语拟声词 onomatopoeia的第1个字母。 
*/
const char *POS_O = "o";

/**
@desc:p	介词 	取英语介词 prepositional的第1个字母。 
*/
const char *POS_P = "p";

/**
@desc:q	量词 	取英语 quantity的第1个字母。 
*/
const char *POS_Q = "q";

/**
@desc:r 代词 	取英语 代词pronoun的第二个字母，因p已用于介词 
*/
const char *POS_R = "r";

/**
@desc:s	处所词 	取英语 space的第1个字母。 
*/
const char *POS_S = "s";


/**
@desc:t	时间词 	取英语 time的第1个字母。 
*/
const char *POS_T = "t";

/**
@desc:u	助词 	取英语助词 auxiliary 
*/
const char *POS_U = "u";


/**
@desc:v	动词 	取英语动词 verb的第一个字母。 
*/
const char *POS_V = "v";

/**
@desc:w	标点符号,在北大标准中标点只是指中文标点
@Note:此处扩展了标点的定义,标点包括中文标点和英文标点
*/
const char *POS_W = "w";

/**
@desc:x	非语素字 	非语素字只是一个符号，字母 x通常用于代表未知数、符号。 
*/
const char *POS_X = "x";

/**
@desc:y	语气词 	取汉字“语”的声母。 
*/
const char *POS_Y = "y";

/**
@desc:z	状态词 	取汉字“状”的声母的前一个字母。 
*/
const char *POS_Z = "z";

/**
@desc:un	未知词	不可识别词及用户自定义词组。
            取英文Unkonwn首两个字母。(非北大标准，CSW分词中定义)
*/
const char *POS_UN = "un";

/******下面为自定义*******************************************/
/**
@desc:ns表示地名,ns0一般指国家名,留待扩展使用
*/
const char *POS_NS0 = "ns-0";

/**
@desc:ns表示地名,ns1指省级行政单位
*/
const char *POS_NS1 = "ns-1";

/**
@desc:ns表示地名,ns2指地级行政单位
*/
const char *POS_NS2 = "ns-2";

/**
@desc:ns表示地名,ns3指县级行政单位
*/
const char *POS_NS3 = "ns-3";

/**
@desc:ns表示地名,ns4指乡镇级行政单位
*/
const char *POS_NS4 = "ns-4";

/**
@desc:ns表示地名,ns5指村级行政单位
*/
const char *POS_NS5 = "ns-5";

/**
@desc:aba表示阿拉伯数字,Arabia前三个字母,即此词由"0-9和-"构成
      方便电话号码识别
*/
const char *POS_ABA = "aba";

/**
@desc:eng表示英文单词,拼音,或者英文标识符(即由a-z组成)
*/
const char *POS_ENG = "eng";

/**
@desc:扩展用于电话识别
*/
const char *POS_TEL = "tel";

/**
@desc:扩展用于地址识别
*/
const char *POS_ADDR = "addr";
