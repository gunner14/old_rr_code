#ifndef __N_WORD_CONST_H__
#define __N_WORD_CONST_H__

#include "container.h"

using namespace segment;

static const char *NumeralAry[] = {
			"第",
			"零",
			"一",
			"二",
			"两",
			"三",
			"四",
			"五",
			"六",
			"七",
			"八",
			"九",
			"十",
			"百",
			"千",
			"万",
			"亿"
			};

static const char *QuantityAry[] = {
			"人",
			"家",
			"名",
			"台",
			"条",
			"张",
			"付",
			"盘",
			"盆",
			"碗",
			"对",
			"队",
			"架",
			"本",
			"颗",
			"棵",
			"群",
			"组",
			"套",
			"袋",
			"元",
			"盒",
			"包",
			"件",
			"座",
			"扇",
			"双",
			"只",
			"支",
			"年",
			"月",
			"日",
			"时",
			"分",
			"秒",
			"节",
			"种",
			"场",
			"手",
			"下",
			"篇",
			"片",
			"杯",
			"口",
			"倍",
			"届",
			"份",
			"根",
			"匹",
			"个",
			"次"
			    };

static StrSet NumeralDict( NumeralAry , NumeralAry + sizeof( NumeralAry )/sizeof( const char* ) );
static StrSet QuantityDict( QuantityAry , QuantityAry + sizeof( QuantityAry )/sizeof( const char*) );

#endif
