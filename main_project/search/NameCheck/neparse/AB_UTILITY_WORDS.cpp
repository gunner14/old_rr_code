/********************************************************************
*Filename:AB_UTILITY_WORD.c
*Author:bfzhang
*Date: 2007-01-20
*Description: define the implementation of some utility interface
*********************************************************************/
#include "AB_UTILITY_WORDS.h" 
#include "time.h"
/****pos tag defination************************************************/
/***�����׼*/

/**
@desc:a	���ݴ� 	ȡӢ�����ݴ� adjective�ĵ�1����ĸ�� 
*/
const char *POS_A = "a";

/**
@desc:b	����� 	ȡ���֡��𡱵���ĸ�� 
*/
const char *POS_B = "b";

/**
@desc:c	���� 	ȡӢ������ conjunction�ĵ�1����ĸ�� 
*/
const char *POS_C = "c";

/**
@desc:d	���� 	ȡ adverb�ĵ�2����ĸ�������1����ĸ���������ݴʡ� 
*/
const char *POS_D = "d";

/**
@desc:e	̾�� 	ȡӢ��̾�� exclamation�ĵ�1����ĸ�� 
*/
const char *POS_E = "e";

/**
@desc:f	��λ�� 	ȡ���֡����� 
*/
const char *POS_F = "f";

/**
@desc:g	���� 	����������ض�����Ϊ�ϳɴʵġ��ʸ�����ȡ���֡���������ĸ�� 
*/
const char *POS_G = "g";

/**
@desc:h	ǰ�ӳɷ� 	ȡӢ�� head�ĵ�1����ĸ�� 
*/
const char *POS_H = "h";

/**
@desc:i	���� 	ȡӢ����� idiom�ĵ�1����ĸ�� 
*/
const char *POS_I = "i";

/**
@desc:j	������� 	ȡ���֡��򡱵���ĸ�� 
*/
const char *POS_J = "j";

/**
@desc:k	��ӳɷ�
*/
const char *POS_K = "k";

/**
@desc:l	ϰ���� 	ϰ������δ��Ϊ����е㡰��ʱ�ԡ���ȡ���١�����ĸ�� 
*/
const char *POS_L = "l";

/**
@desc:m	���� 	ȡӢ�� numeral�ĵ�3����ĸ��n��u�������á� 
*/
const char *POS_M = "m";

/**
@desc:n	���� 	ȡӢ������ noun�ĵ�1����ĸ�� 
*/
const char *POS_N = "n";


/**
@desc:o	������ 	ȡӢ�������� onomatopoeia�ĵ�1����ĸ�� 
*/
const char *POS_O = "o";

/**
@desc:p	��� 	ȡӢ���� prepositional�ĵ�1����ĸ�� 
*/
const char *POS_P = "p";

/**
@desc:q	���� 	ȡӢ�� quantity�ĵ�1����ĸ�� 
*/
const char *POS_Q = "q";

/**
@desc:r ���� 	ȡӢ�� ����pronoun�ĵڶ�����ĸ����p�����ڽ�� 
*/
const char *POS_R = "r";

/**
@desc:s	������ 	ȡӢ�� space�ĵ�1����ĸ�� 
*/
const char *POS_S = "s";


/**
@desc:t	ʱ��� 	ȡӢ�� time�ĵ�1����ĸ�� 
*/
const char *POS_T = "t";

/**
@desc:u	���� 	ȡӢ������ auxiliary 
*/
const char *POS_U = "u";


/**
@desc:v	���� 	ȡӢ�ﶯ�� verb�ĵ�һ����ĸ�� 
*/
const char *POS_V = "v";

/**
@desc:w	������,�ڱ����׼�б��ֻ��ָ���ı��
@Note:�˴���չ�˱��Ķ���,���������ı���Ӣ�ı��
*/
const char *POS_W = "w";

/**
@desc:x	�������� 	��������ֻ��һ�����ţ���ĸ xͨ�����ڴ���δ֪�������š� 
*/
const char *POS_X = "x";

/**
@desc:y	������ 	ȡ���֡������ĸ�� 
*/
const char *POS_Y = "y";

/**
@desc:z	״̬�� 	ȡ���֡�״������ĸ��ǰһ����ĸ�� 
*/
const char *POS_Z = "z";

/**
@desc:un	δ֪��	����ʶ��ʼ��û��Զ�����顣
            ȡӢ��Unkonwn��������ĸ��(�Ǳ����׼��CSW�ִ��ж���)
*/
const char *POS_UN = "un";

/******����Ϊ�Զ���*******************************************/
/**
@desc:ns��ʾ����,ns0һ��ָ������,������չʹ��
*/
const char *POS_NS0 = "ns-0";

/**
@desc:ns��ʾ����,ns1ָʡ��������λ
*/
const char *POS_NS1 = "ns-1";

/**
@desc:ns��ʾ����,ns2ָ�ؼ�������λ
*/
const char *POS_NS2 = "ns-2";

/**
@desc:ns��ʾ����,ns3ָ�ؼ�������λ
*/
const char *POS_NS3 = "ns-3";

/**
@desc:ns��ʾ����,ns4ָ����������λ
*/
const char *POS_NS4 = "ns-4";

/**
@desc:ns��ʾ����,ns5ָ�弶������λ
*/
const char *POS_NS5 = "ns-5";

/**
@desc:aba��ʾ����������,Arabiaǰ������ĸ,���˴���"0-9��-"����
      ����绰����ʶ��
*/
const char *POS_ABA = "aba";

/**
@desc:eng��ʾӢ�ĵ���,ƴ��,����Ӣ�ı�ʶ��(����a-z���)
*/
const char *POS_ENG = "eng";

/**
@desc:��չ���ڵ绰ʶ��
*/
const char *POS_TEL = "tel";

/**
@desc:��չ���ڵ�ַʶ��
*/
const char *POS_ADDR = "addr";
