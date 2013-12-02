#ifndef __AbuUtility_H__
#define __AbuUtility_H__
/**
 *
 */
#include "AbuResource.hpp"
float CalcWeightSum(
        const FloatVector& weight_vector, 
        const FloatVector& damping_factor_vector
        );

void *CallocMem(int size, const char* type);

bool IsEndsStartsWithOther(const char *string_i, int len_i,
        const char *string_j, int len_j);

bool IsStrEqual(const char *buf_i, const int len_i, 
        const char *buf_j, const int len_j);

int GetEditDist(const char *left_string, int in_left_len,//len is in bytes
        const char *right_string, int in_right_len,//len is in bytes
        int offset, //compare unit..[2/1]
        const int add_weight = 1, 
        const int delete_weight = 1, 
        const int subs_weight = 2
        );

int GetSameFeat(const char *addr_i, int addr_len_i, 
        const char *addr_j, int addr_len_j, U_FEAT *feats, int feat_num);

bool GetIntFromStr(const char *str_buf, int* p_int);

int ConvertGbToUni(const char *gb_buf, int gb_len, unsigned short *uni_buf, int *p_uni_len);

int ConvertUniToGb(const unsigned short *uni_buf, int uni_len, char *gb_buf, int *p_gb_len);

StartLengthPair GetStartLenghPair(const string& field_content, const ABU_PARSE_RESULT& parse_result);
void DumpAddr(const ABU_PARSE_RESULT &parse_result);

#endif //__AbuUtility_H__
