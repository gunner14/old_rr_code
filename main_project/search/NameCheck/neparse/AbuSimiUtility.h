#ifndef __AbuSimiUtility_H__
#define __AbuSimiUtility_H__
/**
 *
 */
int CalcStreetSimi(const unsigned short *addr_i, int start_i, int len_i,
        const unsigned short *addr_j, int start_j, int len_j,
        const char *street_code_i, int street_code_len_i,
        const char *street_code_j, int street_code_len_j);

int CalcSubdistrictSimi(const unsigned short *u_addr_i, int start_i, int len_i, 
        const unsigned short *u_addr_j, int start_j, int len_j, 
        const char *subdistrict_code_i, int subdistrict_code_len_i,
        const char *subdistrict_code_j, int subdistrict_code_len_j);
#endif //__AbuSimiUtility_H__
