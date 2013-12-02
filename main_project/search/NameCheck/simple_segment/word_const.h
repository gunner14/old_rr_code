#ifndef __N_WORD_CONST_H__
#define __N_WORD_CONST_H__

#include "container.h"

using namespace segment;

static const char *NumeralAry[] = {
			"��",
			"��",
			"һ",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"ʮ",
			"��",
			"ǧ",
			"��",
			"��"
			};

static const char *QuantityAry[] = {
			"��",
			"��",
			"��",
			"̨",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"Ⱥ",
			"��",
			"��",
			"��",
			"Ԫ",
			"��",
			"��",
			"��",
			"��",
			"��",
			"˫",
			"ֻ",
			"֧",
			"��",
			"��",
			"��",
			"ʱ",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"ƪ",
			"Ƭ",
			"��",
			"��",
			"��",
			"��",
			"��",
			"��",
			"ƥ",
			"��",
			"��"
			    };

static StrSet NumeralDict( NumeralAry , NumeralAry + sizeof( NumeralAry )/sizeof( const char* ) );
static StrSet QuantityDict( QuantityAry , QuantityAry + sizeof( QuantityAry )/sizeof( const char*) );

#endif
