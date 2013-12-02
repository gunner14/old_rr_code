/**
 * @file abstring.c
 * @author bfzhang
 * @brief the public interface implementation
 *  create the file in 2007-08-27
 */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include "errno.h"
#include "abstring.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define UINT32_MAXSIZE 0xFFFFFFFF

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

const int GB_LEN = 2;
const int ASCII_LEN = 1;

const int WHITE_SPACE_COUNT = 6;
const char WHITE_SPACE[ ] = 
{
    '\t', '\n', '\r', ' ', 0x0b,
    0x0c
};

const int ASCII_PUNCT_COUNT = 32;
const char ASCII_PUNCT[ ] = 
{
    '!', '\"', '#', '$', '%',
    '&', '\'', '(', ')', '*',
    '+', ',',  '-', '.', '/',
    ':', ';',  '<', '=', '>',
    '?', '@',  '[', '\\', ']',
    '^', '_',  '`', '{', '|',  
    '}', '~' 
};

const int GB_PUNCT_COUNT = 34;
const char * GB_PUNCT[ ] = 
{
    "£¡", "¡°" ,"££", "£¤", "£¥",
    "£¦", "¡°","£¨", "£©", "£ª",
    "£«", "£¬", "£­", "¡£", "£¯",
    "£¿", "£À", "£Û", "¡¢", "£Ý",
    "£º", "£»", "¡¶","£½", "¡·",
    "¡­¡­","¡ª¡ª","£à", "£û", "£ü",
    "£ý", "¡«", "¡±", "¡¤"
};

/************************************************/
/***private interface declaration****************/
/************************************************/
static void *ab_realloc(void *ptr,size_t size);


static void *ab_calloc(size_t nmemb,size_t size);


static  unsigned int nearest_power(unsigned int base, 
                                         unsigned int num)    
{
    if (num > UINT32_MAXSIZE / 2)
    {
        return UINT32_MAXSIZE;
    }
    else
    {
        int n = base;

        while (n < num)
            n <<= 1;
        return n;
    }
}


static void abstring_maybe_expand (abstring * string,unsigned int len); 

/************************************************/
/***public interface implementation(abstring)****/
/************************************************/
abstring* abstring_sized_new (int dfl_size)    
{
    //assert(dfl_size > 0);
    if(dfl_size <= 0)
        return NULL;
    abstring *string = (abstring*)ab_calloc (1,sizeof(abstring));

    string->allocated_len = 0;
    string->len   = 0;
    string->str   = NULL;

    abstring_maybe_expand (string, MAX (dfl_size, 2));
    string->str[0] = 0;

    return string;
}


abstring* abstring_new (const char *init)
{
    //assert(init != NULL);
    if(init == NULL)
        return NULL;

    int len = strlen (init);
      
    abstring * string = abstring_sized_new (len + 2);

    abstring_append_len (string, init, len);

    return string;
}


abgbstring * abgbstring_new(const char *init)
{
    //assert(init != NULL);
    if(init == NULL)
        return NULL;

    int len = strlen(init);
    unsigned short gbinit[len];
    int gblen = 0;
    for(int i = 0;i < len;)
    {
        if(init[i] > 0)
        {
            gbinit[gblen]  = (unsigned char)init[i];
            i++;
            gblen++;
            continue;
        }
        //gbcode-include(badcode and goodcode)
        if( (i + 1) >= len)//badcode
        {
            gbinit[gblen] = (unsigned char)init[i];
            i++;gblen++;
            break;            
        }
        else//goodcode
        {
            gbinit[gblen] = (unsigned char)init[i];
            gbinit[gblen] = (gbinit[gblen] << 8) | (unsigned char)init[i + 1];
            i += 2;gblen++;
        }        
    }

    abgbstring *gbstring = (abgbstring*)ab_calloc(1, sizeof(abgbstring));
    gbstring->gb_str = (unsigned short*)ab_calloc(gblen, sizeof(unsigned short));
    memcpy(gbstring->gb_str, gbinit, gblen * sizeof(unsigned short));
    gbstring->len = gblen;

    return gbstring;
}


abstring* abstring_new_len (const char *init,int len)    
{
    //assert(len > 0);
    if(len <= 0)
        return NULL;
    abstring *string;

    string = abstring_sized_new (len);
      
    if (init)
        abstring_append_len (string, init, len);
      
    return string;
}


void abstring_free (abstring *string,
           unsigned char free_segment)
{
   if(string == NULL)
        return;

    if (free_segment)
        free (string->str);
    
    free(string);
    string = NULL;
}


void abgbstring_free(abgbstring *gbstring,
                    unsigned char free_segment)
{
    if(gbstring == NULL)
        return;

    if(free_segment)
        free(gbstring->gb_str);

    free(gbstring);
    gbstring = NULL;
}


unsigned char abstring_equal (const abstring *v,
                const abstring *v2)
{
    //assert(v != NULL && v2 != NULL);
    if(NULL == v || NULL == v2)
        return FALSE;

    abstring *left = (abstring *) v;
    abstring *right = (abstring *) v2;
  
    if(left->len != right->len)
        return FALSE;
    
    return strncmp(left->str, right->str, left->len) == 0 ? TRUE : FALSE;
}


unsigned char abstring_equal_str(const abstring *string,const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;
    
    return strcmp(string->str, str) == 0 ? TRUE : FALSE;
}


unsigned int abstring_hash (const abstring *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return 0;

    return abstru_hash(str->str);
}


void abstring_assign (abstring *string,const char *rval)
{
    //assert(string != NULL && rval != NULL);
    if(NULL == string ||NULL == rval)
        return;
    
    if (string->str != rval)
    {
        abstring_truncate (string, 0);
        abstring_append (string, rval);
    }
}


void abstring_truncate (abstring *string,int len)    
{
    //assert(string != NULL);
    //assert(len >= 0);
    if(NULL == string)
        return;
    if(len < 0)
        return;

    string->len = MIN (len, string->len);
    string->str[string->len] = 0;
}


void abstring_set_size (abstring *string,int len)    
{
    //assert(string != NULL);
    //assert(len >= 0);
    if(NULL == string)
        return;
    if(len < 0)
        return;

    if (len >= string->allocated_len)
        abstring_maybe_expand (string, len - string->len);
  
    string->len = len;
    string->str[len] = 0;
}


void abstring_insert_len (abstring *string,int pos,    
             const char *val,int len)    
{
    //assert(string != NULL && val != NULL);
    //assert(pos >= 0 && pos <= string->len);
    //assert(len >= 0);
    if(NULL == string || NULL == val)
        return;
    if( pos < 0 || pos > string->len)
        return;
    if(len < 0)
        return;


    if (val >= string->str && val <= string->str + string->len)
    {
        int offset = val - string->str;
        int precount = 0;

        abstring_maybe_expand (string, len);
        val = string->str + offset;

        if (pos < string->len)
            memmove (string->str + pos + len, string->str + pos, string->len - pos);

        if (offset < pos)
        {
            precount = MIN (len, pos - offset);
            memcpy (string->str + pos, val, precount);
        }

        if (len > precount)
            memcpy (string->str + pos + precount,
            val + precount + len,
            len - precount);
    }
    else
    {
        abstring_maybe_expand (string, len);

        if (pos < string->len)
            memmove (string->str + pos + len, string->str + pos, string->len - pos);

        if (len == 1)
            string->str[pos] = *val;
        else
            memcpy (string->str + pos, val, len);
    }

    string->len += len;

    string->str[string->len] = 0;
}


void abstring_append (abstring *string,const char *val)
{  
    //assert(string != NULL && val != NULL);
    if(NULL == string || NULL == val)
        return;

    abstring_insert_len (string, string->len, val, strlen(val));
}


void abstring_append_len (abstring *string,
                          const char *val,
                          int len)    
{
    //assert(string != NULL && val != NULL);
    //assert(len >= 0);
    if(NULL == string || NULL == val)
        return;
    if(len < 0)
        return;
    if(val[0] == 0)
        return;
    
    abstring_insert_len (string, string->len, val, len);
}


void abstring_append_c (abstring *string,
                        char c)
{
    //assert(string != NULL);
    if(NULL == string)
        return;

    abstring_insert_c (string, string->len, c);
}


void abstring_prepend (abstring *string,
                       const char *val)
{
    //assert(string != NULL && val != NULL);
    if(NULL == string || NULL == val)
        return;
    if(val[0] == 0)
        return;

    return abstring_insert_len (string, 0, val, strlen(val));
}


void abstring_prepend_len (abstring *string,
                           const char *val,
                           int len)    
{
    //assert(string != NULL && val != NULL);
    //assert(len >= 0);
    if(NULL == string || NULL == val)
        return;
    if(len < 0)
        return;

    return abstring_insert_len (string, 0, val, len);
}


void abstring_prepend_c (abstring *string,
                         char c)
{  
    //assert(string != NULL);
    if(NULL == string)
        return;

    return abstring_insert_c (string, 0, c);
}


void abstring_insert (abstring *string,
                      int pos,    
                      const char *val)
{
    //assert(string != NULL && val != NULL);
    //assert(pos <= string->len);
    if(NULL == string || NULL == val)
        return;
    if(pos > string->len)
        return;
    if(val[0] == 0)
        return;

    return abstring_insert_len (string, pos, val,strlen(val));
}


void abstring_insert_c (abstring *string,
                        int pos,    
                        char c)
{
    //assert(string != NULL);
    //assert(pos >= 0 && pos <= string->len);
    if(NULL == string)
        return;
    if(pos < 0 || pos > string->len)
        return;
    
    abstring_maybe_expand (string, 1);

  
    if (pos < string->len)
        memmove (string->str + pos + 1, string->str + pos, string->len - pos);

    string->str[pos] = c;

    string->len += 1;

    string->str[string->len] = 0;
}


void abstring_erase (abstring *string,
                    int pos,
                    int len)
{
    //assert(string != NULL);
    //assert(pos >= 0 && pos <= string->len);
    //assert(len >= 0);
    //assert((pos + len) <= string->len);
    if(NULL == string)
        return;
    if(pos < 0 || pos > string->len)
        return;
    if(len < 0)
        return;
    if((pos + len) > string->len)
        return;

    memmove (string->str + pos, string->str + pos + len, string->len - (pos + len));

    string->len -= len;
  
    string->str[string->len] = 0;
}


void abstring_lower (abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return;
    int n = string->len;
    char *s = string->str;

    while (n)
    {
        if(*s >= 'A' && *s <= 'Z')
            *s += 32;
        s++;
        n--;
    }
}


void abstring_upper (abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return;

    int n = string->len;
    char *s = string->str;

    while (n)
    {
        if(*s >= 'a' && *s <= 'z')
            *s -= 32;
        s++;
        n--;
    }
}


int abstring_freq_c(abstring *string,const char c)
{
    //assert(string != NULL);
    if(NULL == string)
        return 0;
    return abstru_freq_c(string->str,c);
}


int abstring_freq_str(abstring *string,const char *sub)
{
    //assert(string != NULL);
    if(NULL == string)
        return 0;
    return abstru_freq_str(string->str,sub);
}


unsigned char abstring_hasdigit(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return FALSE;
    return abstru_hasdigit(string->str);
}


unsigned char abstring_hasletter(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return FALSE;
    return abstru_hasletter(string->str);
}


void abstring_strip(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;
    abstru_strip(string->str);
    string->len = strlen(string->str);
}


void abstring_strip_c(abstring *string, const char c)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;
    abstru_strip_c(string->str, c);
    string->len = strlen(string->str);
}


void abstring_strip_str(abstring *string,const char *strip_str)
{
    //assert(string != NULL && strip_str != NULL);
    if(NULL == string)
        return ;
    abstru_strip_str(string->str,strip_str);
    string->len = strlen(string->str);
}


void abstring_lstrip(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;
    abstru_lstrip(string->str);
    string->len = strlen(string->str);
}


void abstring_lstrip_c(abstring *string, const char c)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;
    abstru_lstrip_c(string->str, c);
    string->len = strlen(string->str);
}


void abstring_lstrip_str(abstring *string,const char *strip_str)
{
    //assert(string != NULL && strip_str != NULL);
    if(NULL == string ||NULL == strip_str)
        return;
    abstru_lstrip_str(string->str,strip_str);
    string->len = strlen(string->str);
}


void abstring_rstrip(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;

    abstru_rstrip(string->str);
    string->len = strlen(string->str);
}


void abstring_rstrip_c(abstring *string, const char c)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;
    abstru_rstrip_c(string->str, c);
    string->len = strlen(string->str);
}


void abstring_rstrip_str(abstring *string,const char *strip_str)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;
    abstru_rstrip_str(string->str,strip_str);
    string->len = strlen(string->str);
}


void abstring_reverse(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;
    if(string->len == 0)
        return;
    abstru_reverse(string->str);
}


void abstring_sbc2dbc(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return ;
    abstru_sbc2dbc(string->str);
    string->len = strlen(string->str);
}


void abstring_remove(abstring *string,const char *remove_str)
{
    //assert(string != NULL && remove_str != NULL);
    if(NULL == string || NULL == remove_str)
        return;
    abstru_remove(string->str,remove_str);
    string->len = strlen(string->str);
}


void abstring_remove_gb(abstring *string,const char *remove_str)
{
    //assert(string != NULL && remove_str != NULL);
    if(NULL == string || NULL == remove_str)
        return;
    abstru_remove_gb(string->str,remove_str);
    string->len = strlen(string->str);
}


void abstring_remove_c(abstring *string, const char c)
{
    //assert(string != NULL);
    if(NULL == string)
        return;
    abstru_remove_c(string->str, c);
    string->len = strlen(string->str);
}


void abstring_remove_badcode_gb(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return;
    abstru_remove_badcode_gb(string->str);
    string->len = strlen(string->str);
}


void abstring_remove_whitespace(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return;
    abstru_remove_whitespace(string->str);
    string->len = strlen(string->str);
}


void abstring_remove_asciipunct(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return;
    abstru_remove_asciipunct(string->str);
    string->len = strlen(string->str);
}


void abstring_remove_gbpunct(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return;
    abstru_remove_gbpunct(string->str);
    string->len = strlen(string->str);
}


void abstring_remove_punct(abstring *string)
{
    //assert(string != NULL);
    if(NULL == string)
        return;
    abstru_remove_punct(string->str);
    string->len = strlen(string->str);
}


void abstring_replace_first(abstring *string,const char *old_str,
                            const char *new_str)
{
    //assert(string != NULL && old_str != NULL && new_str != NULL);
    if(NULL == string || NULL == old_str ||NULL == new_str)
        return;
    if(string->len == 0 || old_str[0] == 0 || new_str[0] == 0)
        return;

    char *first_pos = strstr(string->str,old_str);
    if(first_pos == NULL)
        return;
    
    int pos = first_pos - string->str;
    int len_old = strlen(old_str);
    int len_new = strlen(new_str);
    abstring_erase(string,pos,len_old);
    abstring_insert_len(string,pos,new_str,len_new);
}


void abstring_replace_first_gb(abstring *string,const char *old_str,
                            const char *new_str)
{
    //assert(string != NULL && old_str != NULL && new_str != NULL);
    if(NULL == string || NULL == old_str ||NULL == new_str)
        return;
    if(string->len == 0 || old_str[0] == 0 || new_str[0] == 0)
        return;

    char *first_pos = abstru_strstr_gb(string->str,old_str);
    if(first_pos == NULL)
        return;
    
    int pos = first_pos - string->str;
    int len_old = strlen(old_str);
    int len_new = strlen(new_str);
    abstring_erase(string,pos,len_old);
    abstring_insert_len(string,pos,new_str,len_new);
}


void abstring_replace_last(abstring *string,const char *old_str,
                           const char *new_str)
{
    //assert(string != NULL && old_str != NULL && new_str != NULL);
    if(NULL == string || NULL == old_str ||NULL == new_str)
        return;
    if(string->len == 0 || old_str[0] == 0 || new_str[0] == 0)
        return;

    int len_old = strlen(old_str);
    char *last_pos = NULL;
    char *p = string->str;
    while( (p = strstr(p,old_str) ) != NULL)
    {
        last_pos = p;
        p += len_old;
    }
    
    if(last_pos == NULL)
        return;

    int pos = last_pos - string->str;
    int len_new = strlen(new_str);   
    abstring_erase(string,pos,len_old);
    abstring_insert_len(string,pos,new_str,len_new);
}


void abstring_replace_last_gb(abstring *string,const char *old_str,
                           const char *new_str)
{
    //assert(string != NULL && old_str != NULL && new_str != NULL);
    if(NULL == string || NULL == old_str ||NULL == new_str)
        return;
    if(string->len == 0 || old_str[0] == 0 || new_str[0] == 0)
        return;

    int len_old = strlen(old_str);
    char *last_pos = NULL;
    char *p = string->str;
    while( (p = abstru_strstr_gb(p,old_str) ) != NULL)
    {
        last_pos = p;
        p += len_old;
    }
    
    if(last_pos == NULL)
        return;

    int pos = last_pos - string->str;
    int len_new = strlen(new_str);   
    abstring_erase(string,pos,len_old);
    abstring_insert_len(string,pos,new_str,len_new);
}


void abstring_replace(abstring *string,const char *old_str,
                      const char *new_str)
{
    //assert(string != NULL && old_str != NULL && new_str != NULL);
    if(NULL == string || NULL == old_str ||NULL == new_str)
        return;
    if(string->len == 0 || old_str[0] == 0 || new_str[0] == 0)
        return;

    int len_old = strlen(old_str);
    int len_new = strlen(new_str);
 
    char *p= string->str;
    while( (p = strstr(p,old_str)) != NULL)
    {
        int pos = p - string->str;    
        abstring_erase(string,pos,len_old);
        abstring_insert_len(string,pos,new_str,len_new);
        p += len_new;
    }

}


void abstring_replace_gb(abstring *string,const char *old_str,
                       const char *new_str)
{
    //assert(string != NULL && old_str != NULL && new_str != NULL);
    if(NULL == string || NULL == old_str ||NULL == new_str)
        return;
    if(string->len == 0 || old_str[0] == 0 || new_str[0] == 0)
        return;
    
    int len_old = strlen(old_str);
    int len_new = strlen(new_str);
 
    char *p= string->str;
    while( (p = abstru_strstr_gb(p,old_str)) != NULL)
    {
        int pos = p - string->str;    
        abstring_erase(string,pos,len_old);
        abstring_insert_len(string,pos,new_str,len_new);
        p += len_new;
    }
}

/************************************************/
/***public interface implementation(abstru)******/
/************************************************/
char *abstru_strdup(const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return NULL;
    
    unsigned int len = strlen(str);
    char *dst = (char*)ab_calloc(len + 1, sizeof(char));
    strncpy(dst, str, len);
    
    return dst;    
}


char *abstru_strndup(const char *str, const unsigned int len)
{
    //assert(str != NULL);
    if(NULL == str)
        return NULL;
    
    if(len > strlen(str))
    {
        fprintf(stderr,"abstru_strndup error:[%s]'s len < %u\n", str, len);
        exit(-1);
    }
    
    char *dst = (char*)ab_calloc(len + 1, sizeof(char));
    strncpy(dst, str,len);

    return dst;
}


int abstru_freq_c(const char *str,const char c)
{
    //assert(str != NULL);
    if(NULL == str)
        return 0;
    if(c == 0)
        return 0;

    int count = 0;
    char *p = (char *)str;
    while(*p != 0)
    {
        if(*p == c)
            count += 1;
        p++;
    }      

    return count;
}


int abstru_freq_str(const char *str,const char *sub)
{
    //assert(str != NULL && sub != NULL);
    if(NULL == str || NULL == sub)
        return 0;
    if(str[0] == 0 || sub[0] == 0)
        return 0;

    int len_sub = strlen(sub);
    int count = 0;
    char *p = (char *)str;
    while( (p = strstr(p,sub)) != NULL)
    {
        count += 1;
        p += len_sub;
    }

    return count;
}


unsigned char abstru_hasdigit(const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;
    if(str[0] == 0)
        return FALSE;
    
    char *p = (char *)str;
    while(*p)
    {
        if(*p >= '0' && *p <= '9')
            return TRUE;
        p += 1;
    }

    return FALSE;
}


unsigned char abstru_hasletter(const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;

    if(str[0] == 0)
        return FALSE;

    char *p = (char *)str;
    while(*p)
    {
        if( (*p >= 'A' && *p <= 'Z') ||
            (*p >= 'a' && *p <= 'z'))
            return TRUE;            
        p += 1;
    }

    return FALSE;
}

unsigned char abstru_hasgb(const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;
    if(str[0] == 0)
        return FALSE;

    char *p = (char *)str;
    while(*p)
    {
        if(*p < 0)
            return TRUE;
        p += 1;
    }   
    return FALSE;    
}


unsigned char abstru_isdigit(const char c)
{
    if(c >= '0' && c <= '9')
        return TRUE;
    return FALSE;
}


unsigned char abstru_ishyphen(const char c)
{
    if(c == '-')
        return TRUE;
    return FALSE;
}


unsigned char abstru_isletter(const char c)
{
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return TRUE;
    return FALSE;
}


unsigned char abstru_isdigits(const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;
    if(str[0] == 0)
        return FALSE;
    
    char *p = (char *)str;
    while(*p)
    {
        if(*p < '0' || *p >= '9')
            return FALSE;
        p++;
    }

    return TRUE;
}


unsigned char abstru_isletters(const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return FALSE;
    if(str[0] == 0)
        return FALSE;

    char *p = (char *)str;
    while(*p)
    {
        unsigned char isletter = FALSE;
        if((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z'))
            isletter = TRUE;
        if(isletter == FALSE)
            return FALSE;
        p++;
    }

    return TRUE;
}


unsigned char abstru_startswith(const char *str, const char *sub)
{
    //assert(str != NULL && sub != NULL);
    if(NULL == str || NULL == sub)
        return FALSE;

    int sub_len = strlen(sub);
    return strncmp(str, sub, sub_len) == 0?TRUE:FALSE;
}


unsigned char abstru_endswith(const char *str, const char *sub)
{
    //assert(str != NULL && sub != NULL);
    if(NULL == str || NULL == sub)
        return FALSE;
    
    int str_len = strlen(str);
    int sub_len = strlen(sub);
    if(str_len < sub_len)
        return FALSE;
    return strncmp(str + str_len - sub_len, sub, sub_len) == 0?TRUE:FALSE;    
}


void abstru_reverse(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    int len = strlen(str);
    if(len == 0)
        return;

    char c;
    for(int i = 0;i < len/2;i++)
    {
        c = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = c; 
    }
}


void abstru_strip(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    abstru_lstrip(str);
    abstru_rstrip(str);
}


void abstru_strip_c(char *str,const char c)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    
    abstru_lstrip_c(str, c);
    abstru_rstrip_c(str, c);
}


void abstru_strip_str(char *str,const char *strip_str)
{
    //assert(str != NULL && strip_str != NULL);
    if(NULL == str || NULL == strip_str)
        return;
    abstru_lstrip_str(str,strip_str);
    abstru_rstrip_str(str,strip_str);
}


void abstru_lstrip(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;

    int len = strlen(str);
    while(len > 0)
    {
        unsigned char iswhitespace = FALSE;
        for(int i = 0;i < WHITE_SPACE_COUNT;i++)
        {
            if(str[0] == WHITE_SPACE[i])
            {
                iswhitespace = TRUE;
                break;
            }
        }
        if(iswhitespace == FALSE)
            break;
        if(len > 1)
            memmove(str, str + 1, len - 1);
        len -= 1;
    }
    str[len] = 0;
}


void abstru_lstrip_c(char *str, const char c)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;

    int len = strlen(str);
    while(len > 0 && str[0] == c)
    {
        if(len > 1)
            memmove(str, str + 1, len - 1);
        len -= 1;
    }
    str[len] = 0; 
}


void abstru_lstrip_str(char *str,const char *strip_str)
{
    //assert(str != NULL && strip_str != NULL);
    if(NULL == str || NULL == strip_str)
        return;

    int len = strlen(str);
    int len_strip = strlen(strip_str);
    if(len == 0 || len_strip == 0)
        return;
    if(len < len_strip)
        return;

    int start = 0;
    while(strncmp(str + start,strip_str,len_strip) == 0)
    {
        start += len_strip;
    }

    memmove(str,str + start,len - start);
    
    str[len - start] = 0;
}


void abstru_rstrip(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;
    
    int len = strlen(str);
    char *p = str + len - 1;
    while(*p && p >= str)
    {
        unsigned char iswhitespace = FALSE;
        for(int i = 0;i < WHITE_SPACE_COUNT;i++)
        {
            if(WHITE_SPACE[i] == *p)
            {
                iswhitespace = TRUE;
                break;
            }
        }
        if(iswhitespace == FALSE)
            break;
        
        char *q = p - 1;
        *p = 0;
        p = q; 
    }    
}


void abstru_rstrip_c(char *str,const char c)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;

    int len = strlen(str);
    char *p = str + len -1;
    while(*p && p >= str && *p == c)
    {
        char *q = p - 1;
        *p = 0;
        p = q;
    }
}


void abstru_rstrip_str(char *str,const char *strip_str)
{
    //assert(str != NULL && strip_str != NULL);
    if(NULL == str || NULL == strip_str)
        return;
    int len = strlen(str);
    int len_strip = strlen(strip_str);
    if(len == 0 || len_strip == 0)
        return;
    if(len < len_strip)
        return;

    while(strncmp(str + len - len_strip,strip_str,len_strip) == 0)
    {
        len -= len_strip;
    }
    
    str[len] = 0;
}

void abstru_sbc2dbc(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;

    int len = strlen(str);
    int cover = 0;
    
    for(int i = 0;i < len;)
    {
        if(str[i] > 0)
        {
            str[cover++] = str[i++];
            continue;
        }
        if(str[i] == -93)//sbc character,except sbc space
        {
            i++;
            str[cover++] = str[i++] + 128;
        }
        else if(str[i] == -95 && str[i + 1] == -95)
        {
            str[cover++] = ' ';
            i += 2;
        }
        else
        {
            str[cover++] = str[i++];
            str[cover++] = str[i++];
        }
    }

    str[cover] = 0;
}


void abstru_remove(char *str,const char *remove_str)
{
    //assert(str != NULL && remove_str != NULL);
    if(NULL == str || NULL == remove_str)
        return;
    
    //step1:check
    if(str[0] == 0 || remove_str[0] == 0)
        return;

    int len = strlen(str);
    int len_remove = strlen(remove_str);
    if(len_remove > len)
        return;

    //step2:get freq
    int pos[len / len_remove];
    memset(pos,0,sizeof(pos));

    int count = 0;
    for(int i = 0;i < len;)
    {
        char *p = strstr(str + i,remove_str);
        if(p == NULL)
            break;
        pos[count] = p - str;
        i = p - str + len_remove;
        count += 1;        
    }
    if(count == 0)
        return;
    
    //step3:cat 
    int new_cover = pos[0];
    int old_cover = pos[0];
    for(int i = 0;i < count;i++)
    {
        old_cover += len_remove;
        int len_segment = 0;
        if((i + 1) < count)
            len_segment = pos[i + 1] - pos[i] - len_remove;
        else
            len_segment = len - old_cover;
        memmove(str + new_cover,str + old_cover,len_segment);
        new_cover += len_segment;
        old_cover += len_segment;
    }    
    str[new_cover] = 0;
}


void abstru_remove_c(char *str,const char c)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    
    if(str[0] == 0)
        return;

    int len = strlen(str);
    for(int i = 0;i < len;)
    {
        if(str[i] != c)
        {
            i++;
            continue;
        }
        if(i < (len - 1))
            memmove(str + i, str + i + 1, len - i - 1);
        len -= 1;
    }
    str[len] = 0;
}

void abstru_remove_gb(char *str,const char *remove_str)
{
    //assert(str != NULL && remove_str != NULL);
    if(NULL == str || NULL == remove_str)
        return;
    
    //step1:check
    if(str[0] == 0 || remove_str[0] == 0)
        return;

    int len = strlen(str);
    int len_remove = strlen(remove_str);
    if(len_remove > len)
        return;

    //step2:get freq
    int pos[len / len_remove];
    memset(pos,0,sizeof(pos));

    int count = 0;
    for(int i = 0;i < len;)
    {
        char *p = abstru_strstr_gb(str + i,remove_str);
        if(p == NULL)
            break;
        pos[count] = p - str;
        i = p - str + len_remove;
        count += 1;        
    }
    if(count == 0)
        return;
    
    //step3:cat 
    int new_cover = pos[0];
    int old_cover = pos[0];
    for(int i = 0;i < count;i++)
    {
        old_cover += len_remove;
        int len_segment = 0;
        if((i + 1) < count)
            len_segment = pos[i + 1] - pos[i] - len_remove;
        else
            len_segment = len - old_cover;
        memmove(str + new_cover,str + old_cover,len_segment);
        new_cover += len_segment;
        old_cover += len_segment;
    }    
    str[new_cover] = 0;
}

void abstru_remove_badcode_gb(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;
    
    int len = strlen(str);
    for(int i = 0;i < len;)
    {
        //ascii
        if(str[i] > 0)
        {
            i += ASCII_LEN;
            continue;
        }

        //gbcode
        //case1:half gb code->string end
        if((i + 1) >= len)
        {
            str[ i ] = 0;
            len -= ASCII_LEN;
            break;
        }

        //case2:half gb code
        if(str[i + 1] > 0)
        {
            memmove(str + i,str + i + ASCII_LEN,len - i - ASCII_LEN);
            len -= ASCII_LEN;
            continue;
        }
        
        i += GB_LEN;        
    }
    str[len] = 0;
}


void abstru_remove_whitespace(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;
    
    for(int i = 0;i < WHITE_SPACE_COUNT;i++)
    {
        abstru_remove_c(str, WHITE_SPACE[i]);
    }
}


void abstru_remove_asciipunct(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;

    for(int i = 0;i < ASCII_PUNCT_COUNT;i++)
    {
        abstru_remove_c(str, ASCII_PUNCT[i]);
    }
}


void abstru_remove_gbpunct(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;

    for(int i = 0;i < GB_PUNCT_COUNT;i++)
    {
        abstru_remove_gb(str,GB_PUNCT[i]);
    }     
}

void abstru_remove_punct(char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return;
    if(str[0] == 0)
        return;
    abstru_remove_asciipunct(str);
    abstru_remove_gbpunct(str);
}


char *abstru_strstr_gb(const char *str,const char *sub_str)
{
    //assert(str != NULL && sub_str != NULL);
    if(NULL == str || NULL == sub_str)
        return NULL;
    if(str[0] == 0 || sub_str[0] == 0)
        return NULL;

    int len = strlen(str);
    int len_sub = strlen(sub_str);
    for(int i = 0;i < len;)
    {
        if(strncmp(str + i,sub_str,len_sub) == 0)
            return (char *)(str + i);
        if(str[i] > 0)
        {
            i++;
            continue;
        }
        else
        {
            i  += 2;
        }
    }

    return NULL;
}


int abstru_strlen_gb(const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return 0;
    if(str[0] == 0)
        return 0;
    
    int len = strlen(str);
    int len_gb = 0;
    for(int i = 0;i < len;)
    {
        if(str[i] > 0)
        {
            len_gb += 1;
            i += 1;
        }
        else
        {
            if((i + 1) > len)
                return len_gb;
            i += 2;
            len_gb += 1;        
        }
    }
    return len_gb;        
}


unsigned int abstru_hash(const char *str)
{
    //assert(str != NULL);
    if(NULL == str)
        return 0;
    char *p = (char *)str;
    int n = strlen(str);    
    unsigned int h = 0;

    while (n--)
    {
        h = (h << 5) - h + *p;
        p++;
    }

    return h;
}


unsigned int abstru_getcommonhead(const char *left,
                                  const char *right)
{
    //assert(left != NULL && right != NULL);
    if(NULL == left || NULL == right)
        return 0;
    char *p = (char *)left;
    char *q = (char *)right;
    
    unsigned int common = 0;
    while(*p && *q && *p == *q)
    {
        p++;
        q++;
        common++;
    }

    return common;
}

unsigned int abstru_geteditdist(const char *left,
                                unsigned int left_len,
                                const char *right,
                                unsigned int right_len, 
                                const unsigned int add_weight, 
                                const unsigned int delete_weight,
                                const unsigned int subs_weight)
{
    //assert(left != NULL && right != NULL);
    if(NULL == left)
        return right_len;
    if(NULL == right)
        return left_len;
    
    if(left_len == 0)
        return right_len;
    if(right_len == 0)
        return left_len;
   
    int matrix[left_len + 1][right_len + 1];
    for(int i = 0;i <= left_len;i++)
        matrix[i][0] = i;
    for(int j =0;j <= right_len;j++)
        matrix[0][j] = j;

    int cost = 0;
    for(int i = 1; i <= left_len;i++)
    {
        for(int j = 1;j <= right_len;j++)
        {
            cost = (left[i - 1] == right[j - 1])? 0 : subs_weight;
            matrix[i][j] = MIN(MIN(matrix[i - 1][j] + add_weight, 
                                   matrix[i][j - 1] + delete_weight),
                               matrix[i - 1][j - 1] + cost);
        }
    }

    return matrix[left_len][right_len];
}

unsigned int abstru_geteditdist_gb(const unsigned short *left, 
                                   const unsigned int left_len,
                                   const unsigned short *right,
                                   const unsigned int right_len,
                                   const unsigned int add_weight,
                                   const unsigned int delete_weight,
                                   const unsigned int subs_weight)
{
    //assert(left != NULL && right != NULL);
    if(NULL == left)
        return right_len;
    if(NULL == right)
        return left_len;
    
    if(left_len == 0)
        return right_len;
    if(right_len == 0)
        return left_len;

    int matrix[left_len + 1][right_len + 1];
    for(int i = 0;i <= left_len;i++)
        matrix[i][0] = i;
    for(int j = 0;j <= right_len;j++)
        matrix[0][j] = j;

    int cost = 0;
    for(int i = 1;i <= left_len;i++)
    {
        for(int j = 1;j <= right_len;j++)
        {
            cost = left[i - 1] == right[j - 1]?0:subs_weight;
            matrix[i][j] = MIN(MIN(matrix[i - 1][j] + add_weight,
                                   matrix[i][j - 1] + delete_weight),
                               matrix[i - 1][j - 1] + cost);
        }
    }

    return matrix[left_len][right_len];
}


unsigned int abstru_getlcs(const char *left,
                           const unsigned int left_len, 
                           const char *right,
                           const unsigned int right_len,
                           int *left_lcspos,
                           int *right_lcspos)
{
    //assert(left != NULL && right != NULL && left_lcspos != NULL);
    if(NULL == left || NULL == right || NULL == left_lcspos)
        return 0;
    
    int matrix[left_len + 1][right_len + 1];
    *left_lcspos = 0;
    *right_lcspos = 0;
    int lcs_len = 0;

    for(int row = 0;row <= left_len;row++)
    {
        for(int col = 0;col <= right_len;col++)
        {
            matrix[row][col] = 0;
        }
    }

    for(int row = 0; row < left_len;row++)
    {
        for(int col = 0; col < right_len;col++)
        {
            if(left[row] == right[col])
            {
                matrix[row + 1][col + 1] = matrix[row][col] + 1;
                if(matrix[row + 1][col + 1] > lcs_len)
                {
                    lcs_len = matrix[row + 1][col + 1];
                    *left_lcspos = row + 1 - lcs_len;
                    *right_lcspos = col + 1 - lcs_len;
                }
            }
            else
            {
                matrix[row][col] = 0;
            }
        }
    }

    return lcs_len;
}


unsigned int abstru_getlcs_gb(const unsigned short *left,
                              const unsigned int left_len,
                              const unsigned short *right,
                              const unsigned int right_len,
                              int *left_lcspos,
                              int *right_lcspos)
{
    //assert(left != NULL && right != NULL && left_lcspos != NULL);
    if(NULL == left || NULL == right || NULL == left_lcspos)
        return 0;
    
    int matrix[left_len + 1][right_len + 1];
    *left_lcspos = 0;
    int lcs_len = 0;

    for(int row = 0;row <= left_len;row++)
    {
        for(int col = 0;col <= right_len;col++)
        {
            matrix[row][col] = 0;
        }
    }

    for(int row = 0; row < left_len;row++)
    {
        for(int col = 0; col < right_len;col++)
        {
            if(left[row] == right[col])
            {
                matrix[row + 1][col + 1] = matrix[row][col] + 1;
                if(matrix[row + 1][col + 1] > lcs_len)
                {
                    lcs_len = matrix[row + 1][col + 1];
                    *left_lcspos = row + 1 - lcs_len;
                    *right_lcspos = col + 1 - lcs_len;
                }
            }
            else
            {
                matrix[row][col] = 0;
            }
        }
    }

    return lcs_len;
}

/************************************************/
/***private interface implementation*************/
/************************************************/
void *ab_realloc(void *ptr,size_t size)
{
    void *p;
    if( (p = realloc(ptr,size) ) == NULL)
    {
        fprintf(stderr,"Realloc error:can't relloc %u bytes\n",size);
        exit(0);
    }
    return p;
}

void *ab_calloc(size_t nmemb,size_t size)
{
    void *p;
    if( (p = calloc(nmemb,size)) == NULL)
    {
        fprintf(stderr,"Calloc error:can't calloc %u bytes\n",size);
        exit(0);
    }
    return p;
}


static void abstring_maybe_expand (abstring* string,unsigned int len) 
{
    if (string->len + len >= string->allocated_len)
    {
        string->allocated_len = nearest_power (1, string->len + len + 1);
        string->str = (char*)ab_realloc (string->str, string->allocated_len);
    }
}
