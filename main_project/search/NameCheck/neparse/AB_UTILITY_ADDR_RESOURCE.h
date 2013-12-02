/**
 *@file AB_UTILITY_ADDR_RESOURCE.h
 *@author bfzhang
 *@brief the resource for addr parsing
 * create the file in 2007-07-02
 */

#include <stdint.h>
#ifndef AB_UTILITY_ADDR_RESOURCE_H_
#define AB_UTILITY_ADDR_RESOURCE_H_

/**
 *@desc:feature word
*/
extern const uint32_t ADDR_FEAT_WORD_COUNT;
extern const char *ADDR_FEAT_WORD[ ];

/**
 *@desc:direct word
 */
extern const uint32_t DIRECT_FEAT_WORD_COUNT;
extern const char *DIRECT_FEAT_WORD[];

/*
 *@desc:province feature word
 */ 
extern const uint32_t PROVINCE_FEAT_WORD_COUNT;
extern const char *PROVINCE_FEAT_WORD[ ]; 
extern const char PROVINCE_FEAT_WORD_SHENG[];
extern const char PROVINCE_FEAT_WORD_SHI[ ];

/**
 * @desc:city feature word
 */
extern const uint32_t CITY_FEAT_WORD_COUNT;
extern const char *CITY_FEAT_WORD[ ];
extern const char CITY_FEAT_WORD_SHI[ ];

/**
 * @desc: county feature word
 */
extern const uint32_t COUNTY_FEAT_WORD_COUNT;
extern const char *COUNTY_FEAT_WORD[ ];
extern const char COUNTY_FEAT_WORD_SHI[ ];
extern const char COUNTY_FEAT_WORD_XIAN[ ];
extern const char COUNTY_FEAT_WORD_QU[ ];

/**
 * @desc:town feature word
 */
extern const uint32_t TOWN_FEAT_WORD_COUNT;
extern const char *TOWN_FEAT_WORD[ ];


/**
 * @desc:street feature word
 */
extern const uint32_t STREET_FEAT_WORD_COUNT;
extern const char *STREET_FEAT_WORD[ ];
extern const uint32_t STREET_FEAT_WORD_GBLEN[];

/**
 * @des:streetno prefix
 */
extern const uint32_t STREETNO_PREFIX_COUNT;
extern const char * STREETNO_PREFIX[];

/**
 * @desc: street number feature word
 */
extern const char STREETNO_FEAT_WORD[ ];

/**
@desc:addr suffix 
*/
extern const uint32_t ADDR_SUFFIX_COUNT;
extern const char *ADDR_SUFFIX[ ];


/**
*desc:company suffix postag
*/
extern const char CMP_SUFFIX_POSTAG[ ];

/**
 * @desc company suffix hanzi
 * £Àdesc
 */
extern const uint32_t CMP_SUFFIX_HANZI_COUNT;
extern const char *CMP_SUFFIX_HANZI[ ];

/**
*desc:company suffix word
*/
extern const uint32_t CMP_SUFFIX_WORD_COUNT; 
extern const char *CMP_SUFFIX_WORD[ ];

/**
 * @desc: addr index word
 */
extern const uint32_t ADDR_INDEX_WORD_COUNT;
extern const char *ADDR_INDEX_WORD[ ];

/**
*@desc:addr prefix word
*/
extern const uint32_t ADDR_PREFIX_COUNT;
extern const char *ADDR_PREFIX[ ];


/**
*@desc:addr prefix flag
*/
extern const uint32_t ADDR_PREFIX_FLAG_COUNT;
extern const char *ADDR_PREFIX_FLAG[ ];

extern const uint32_t ADDR_SEPARATOR_COUNT;
extern const char * ADDR_SEPARATOR[ ];

/**
 * @desc: addr rule weight
 */
    //addr string includes feature word
extern const char ADDR_RULE_FEATWORD[ ];
extern const char ADDR_RULE_FEATWORD_WEIGHT[ ];

    //addr string includes suffix word
extern const char ADDR_RULE_SUFFIXWORD[ ];
extern const char ADDR_RULE_SUFFIXWORD_WEIGHT[ ];

    //addr string includes arabic numerals
extern const char ADDR_RULE_ABA[ ];
extern const char ADDR_RULE_ABA_WEIGHT[ ];

    //addr string includes province(by segment)
extern const char ADDR_RULE_PROVINCE[ ];//ns-1
extern const char ADDR_RULE_PROVINCE_WEIGHT[ ];

    //addr string includes city(by segment)
extern const char ADDR_RULE_CITY[ ];//ns-2
extern const char ADDR_RULE_CITY_WEIGHT[ ];

    //addr string includes county(by segment)
extern const char ADDR_RULE_COUNTY[ ];//ns-3
extern const char ADDR_RULE_COUNTY_WEIGHT[ ];

    //addr string includes town    
extern const char ADDR_RULE_TOWN[ ];
extern const char ADDR_RULE_TOWN_WEIGHT[ ];

    //addr string includes street
extern const char ADDR_RULE_STREET[ ];
extern const char ADDR_RULE_STREET_WEIGHT[ ];

    //addr string includes street no
extern const char ADDR_RULE_STREETNO[ ];
extern const char ADDR_RULE_STREETNO_WEIGHT[ ];    

//negative rule:
    //addr string end with cmp suffix
extern const char ADDR_RULE_CMPSUFFIX[ ];
extern const char ADDR_RULE_CMPSUFFIX_WEIGHT[ ];

#endif /*AB_UTILITY_ADDR_RESOURCE_H_*/
