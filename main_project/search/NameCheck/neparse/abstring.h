/**
 *@file abstring.h
 *@author bfzhang
 *@brief similar gstring
 *@note the string lib works well in gbcode environment
 *  this string lib just maintained in ABData/trunk/tools/utilib/
 * (1)define the abstring struct
 * (2)define the abgbstring struct
 * (3)define the abstru functions
 * create the file in 2007-08-27
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef _ABSTRING_H
#define _ABSTRING_H

extern const int WHITE_SPACE_COUNT;
extern const char WHITE_SPACE[ ]; 

extern const int ASCII_PUNCT_COUNT;
extern const char ASCII_PUNCT[ ];

extern const int GB_PUNCT_COUNT;
extern const char *GB_PUNCT[ ];
/****************************************************/
/*****abstring public api****************************/
/****************************************************/
typedef struct _abstring
{
  char  *str;
  unsigned int len;    
  unsigned int allocated_len;
}abstring;

typedef struct _abgbstring
{
    unsigned short *gb_str;//wide character
    unsigned int len;
}abgbstring;


abstring* abstring_new(const char *init);
abgbstring * abgbstring_new(const char *init);
abstring* abstring_new_len(const char *init,int len);   
abstring* abstring_sized_new(int dfl_size);  


void abstring_free(abstring *string,unsigned char free_segment);
void abgbstring_free(abgbstring *gbstring,unsigned char free_segment);


unsigned char abstring_equal(const abstring *v,const abstring *v2);
unsigned char abstring_equal_str(const abstring *string,const char *str);
unsigned int abstring_hash(const abstring *str);


void abstring_assign(abstring *string,const char *rval);


void abstring_truncate(abstring *string,int len);    


void abstring_set_size(abstring *string,int len);


void abstring_append(abstring *string,const char *val);
void abstring_append_len(abstring *string,const char *val,int len);  
void abstring_append_c(abstring *string,char c);


void abstring_prepend(abstring *string,const char *val);
void abstring_prepend_c(abstring *string,char c);
void abstring_prepend_len(abstring *string,const char *val,int len);


void abstring_insert(abstring *string,int pos, const char *val);
void abstring_insert_len(abstring *string,int pos,const char *val,int len);
void abstring_insert_c(abstring *string,int pos,char c);


void abstring_erase(abstring *string,int pos,int len);


void abstring_lower (abstring *string);
void abstring_upper (abstring *string);


int abstring_freq_c(abstring *string,const char c);
int abstring_freq_str(abstring *string,const char *sub);


unsigned char abstring_hasdigit(abstring *string);
unsigned char abstring_hasletter(abstring *string);


void abstring_strip(abstring *string);
void abstring_strip_c(abstring * string,const char c);
void abstring_strip_str(abstring *string,const char *strip_str);
void abstring_lstrip(abstring *string);
void abstring_lstrip_c(abstring *string,const char c);
void abstring_lstrip_str(abstring *string,const char *strip_str);
void abstring_rstrip(abstring *string);
void abstring_rstrip_c(abstring *string,const char c);
void abstring_rstrip_str(abstring *string,const char *strip_str);

void abstring_reverse(abstring *string);
void abstring_sbc2dbc(abstring *string);

//not saft when replace gbcode character
void abstring_remove(abstring *string,const char *remove_str);
//safe when replace gbcode character
void abstring_remove_gb(abstring *string,const char *remove_str);
void abstring_remove_c(abstring *string, const char c);
//remove the bad gb code
void abstring_remove_badcode_gb(abstring *string);
void abstring_remove_whitespace(abstring *string);
void abstring_remove_asciipunct(abstring *string);
void abstring_remove_gbpunct(abstring *string);
void abstring_remove_punct(abstring *string);

//not safe when replace gbcode character
void abstring_replace_first(abstring *string,const char *old_str,
                            const char *new_str);
//safe when replace gbcode character
void abstring_replace_first_gb(abstring *string,const char *old_str,
                            const char *new_str);
//not safe when replace gbcode character
void abstring_replace_last(abstring *string,const char *old_str,
                           const char *new_str);
//safe when replace gbcode character
void abstring_replace_last_gb(abstring *string,const char *old_str,
                           const char *new_str);
//not safe when replace gbcode character
void abstring_replace(abstring *string,const char *old_str,
                      const char *new_str);
//safe when replace gbcode character
void abstring_replace_gb(abstring *string,const char *old_str,
                       const char *new_str);

/****************************************************/
/*****abstring public api****************************/
/****************************************************/
char *abstru_strdup(const char *str);
char *abstru_strndup(const char *str, const unsigned int len);

int abstru_freq_c(const char *str,const char c);
int abstru_freq_str(const char *str,const char *sub);


unsigned char abstru_hasdigit(const char *str);
unsigned char abstru_hasletter(const char *str);
unsigned char abstru_hasgb(const char *str);

unsigned char abstru_isdigit(const char c);
unsigned char abstru_ishyphen(const char c);
unsigned char abstru_isletter(const char c);
unsigned char abstru_isdigits(const char *str);
unsigned char abstru_isletters(const char *str);


unsigned char abstru_startswith(const char *str, const char *sub);
unsigned char abstru_endswith(const char *str, const char *sub);


void abstru_strip(char *str);
void abstru_strip_c(char *str,const char c);
void abstru_strip_str(char *str,const char *strip_str);
void abstru_lstrip(char *str);
void abstru_lstrip_c(char *str, const char c);
void abstru_lstrip_str(char *str,const char *strip_str);
void abstru_rstrip(char *str);
void abstru_rstrip_c(char *str,const char c);
void abstru_rstrip_str(char *str,const char *strip_str);


void abstru_reverse(char *str);


void abstru_sbc2dbc(char *str);


void abstru_remove_c(char *str, const char c);
void abstru_remove(char *str,const char *remove_str);
void abstru_remove_gb(char *str,const char *remove_str);
void abstru_remove_badcode_gb(char *str);
void abstru_remove_whitespace(char *str);
void abstru_remove_asciipunct(char *str);
void abstru_remove_gbpunct(char *str);
void abstru_remove_punct(char *str);

/**
 * same with strstr
 */
char *abstru_strstr_gb(const char *str,const char *sub_str);


int abstru_strlen_gb(const char  *str);


unsigned int abstru_hash(const char *str);


unsigned int abstru_getcommonhead(const char *left, const char *right);

/**
 * return the editdist(left, right)
 */
unsigned int abstru_geteditdist(const char *left,
                                const unsigned int left_len, 
                                const char *right, 
                                const unsigned int right_len,
                                const unsigned int add_weight, 
                                const unsigned int delete_weight,
                                const unsigned int subs_weight);

/**
 * return the editdist(left, right)
 */
unsigned int abstru_geteditdist_gb(const unsigned short *left, 
                                   const unsigned int left_len,
                                   const unsigned short *right,
                                   const unsigned int right_len,
                                   const unsigned int add_weight,
                                   const unsigned int delete_weight,
                                   const unsigned int subs_weight);

/**
 * return the len(lcs)
 * left_lcspos is lcs's start in left
 */
unsigned int abstru_getlcs(const char *left,
                           const unsigned int left_len, 
                           const char *right,
                           const unsigned int right_len,
                           int *left_lcspos,
                           int *right_lcspos);

unsigned int abstru_getlcs_gb(const unsigned short *left,
                              const unsigned int left_len,
                              const unsigned short *right,
                              const unsigned int right_len,
                              int *left_lcspos,
                              int *right_lcspos); 
#endif 
