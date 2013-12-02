/****************************************************************************
*Filename:AB_UTILITY_NE_ADDR_RESOURCE.h
*Author:bfzhang
*Version: 0.1
* change log
* (1) create the file in 2007-02-27
*Desc: define the ne addr resource
*****************************************************************************/

#ifndef _AB_UTILITY_NE_ADDR_RESOURCE_H
#define _AB_UTILITY_NE_ADDR_RESOURCE_H
/**
@desc:中国地名特征词库,将其加入到分词词库中
*/
extern const char *CHIN_ADDR_FEAT_WORD[];
extern const unsigned int CHIN_ADDR_FEAT_WORD_NUMBER;

/**
@desc:中国地名结束符词库，将其加入到分词词库中,例如“旁”，“侧”，“内”
*/
extern const char *CHIN_ADDR_SUFFIX_W0RD[];
extern const unsigned int CHIN_ADDR_SUFFIX_WORD_NUMBER;
/**
*@desc:运营单位名词库，将其加入到分词词库中
*      这些名词会对地址识别形成干扰，例如“饭店”，“餐厅“等等
*/
extern const char *CHIN_ADDR_PROPER_NOUN[];
extern const unsigned int CHIN_ADDR_PROPER_NOUN_NUMBER;

/**
*@desc:地址前缀 "地址" "座落" "位于"
*      这些词将有助于地址的识别
*/
extern const char *CHIN_ADDR_STRU_WORD[];
extern const unsigned int CHIN_ADDR_STRU_WORD_NUMBER;
/**
*@desc:地址前缀词紧跟之后的词 “：” “于”
*/
extern const char *CHIN_ADDR_STRU_WORD_SUFFIX[];
extern const unsigned int CHIN_ADDR_STRU_WORD_SUFFIX_NUMBER;
#endif
