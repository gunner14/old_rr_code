#include <string.h>
#include <assert.h>
#include "AB_UTILITY_NAME.h"
#include "abutility.h"
#include "shortname.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef MAXNAMELEN
#define MAXNAMELEN 600
#endif

typedef struct _hashnode
{
    char *str;
    struct _hashnode *next;
}hashnode;

typedef struct _segresult
{
    char **words;
    int length;
}segresult;

typedef struct _uniseg
{
    char **uc;
    int length;
}uniseg;

void seg_free(segresult *seg)
{/*{{{*/
    int i;
    for(i=0;i<seg->length;i++)
        free(seg->words[i]);
    free(seg->words);
    free(seg);
}/*}}}*/

void uni_free(uniseg *us)
{/*{{{*/
    int i;
    for(i=0;i<us->length;i++)
        free(us->uc[i]);
    free(us->uc);
    free(us);
}/*}}}*/

hashnode **hash_new(int size)
{/*{{{*/
    hashnode **hashset = (hashnode **)ab_calloc(size,sizeof(hashnode *));
    return hashset;
}/*}}}*/

void hash_add(hashnode **hashset, const char *word, const int size)
{/*{{{*/
    unsigned int hashvalue = abstru_hash(word) % size;
    hashnode *node = (hashnode *)ab_calloc(1, sizeof(hashnode));
    node->str = (char *)ab_calloc(strlen(word)+2, sizeof(char));
    strcpy(node->str, word);
    node->next = hashset[hashvalue];
    hashset[hashvalue] = node;
}/*}}}*/

bool hash_in(hashnode **hashset, const char *word, const int size)
{/*{{{*/
    unsigned int hashvalue = abstru_hash(word) % size;
    hashnode *p;
    for(p = hashset[hashvalue];p!=NULL;p=p->next)
        if(strcmp(p->str, word) == 0)
            return TRUE;
    return FALSE;
}/*}}}*/

void hash_free(hashnode **hashset, const int size)
{/*{{{*/
    int i;
    hashnode *p, *next;
    for(i=0;i<size;i++) {
        next = hashset[i];
        while(1) {
            p = next;
            if(p == NULL) break;
            next = p->next;
            free(p);
        }
    }
    free(hashset);
}/*}}}*/

const int place_set_size = 30001;
hashnode **place_set;
bool has_load_place = FALSE;
void load_place_once(const char *szDictDir)
{/*{{{*/
    if(has_load_place) return;
    has_load_place = TRUE;
    char sline[302],name[300];
    place_set = hash_new(place_set_size);
    char place_filename_list[4][30] = {"place.dict", "hotspot.dict", "town.dict", "town2.dict"};
    char filename[600];
    int place_filename_len = 4, i;
    FILE *f;
    for(i=0;i<place_filename_len;i++) {
        sprintf(filename,"%s/%s",szDictDir,place_filename_list[i]);
        f = fopen(filename, "r");
        if(f == NULL) {
            fprintf(stderr, "open %s file error!\n", filename);
            exit(-1);
        }
        while(1) {
            if(fgets(sline,300,f) == 0) break;
            sscanf(sline,"%s",name);
            hash_add(place_set, name, place_set_size);
        }
        fclose(f);
    }
}/*}}}*/

const char place_list[17][8] = {"厦", "里", "场", "区", "园", "寺", "巷", "家乐福", "mall", "市", "县", "镇", "乡", "街", "路", "道"};
const int place_list_len = 16;
bool isplace(const char *word)
{/*{{{*/
    int i;
    for(i=0;i<place_list_len;i++)
        if(abstru_endswith(word, place_list[i]))
            return TRUE;
    if(hash_in(place_set, word, place_set_size))
        return TRUE;
    return FALSE;
}/*}}}*/

const char asso_list[4][4] = {"分", "支", "店"};
const int  asso_list_len = 3;
bool isasso(const char *word)
{/*{{{*/
    int i;
    for(i=0;i<asso_list_len;i++)
        if(abstru_strstr_gb(word, asso_list[i]))
            return TRUE;
    return FALSE;
}/*}}}*/

hashnode **mer_set;
const int mer_set_size = 50011;
bool has_load_mer = FALSE;
void load_mer_once(const char *szDictDir)
{/*{{{*/
    if(has_load_mer) return;
    has_load_mer = TRUE;
    char sline[302],name[300];
    char filename[600];
    mer_set = hash_new(mer_set_size);
    sprintf(filename,"%s/merchant.feat",szDictDir);
    FILE *f = fopen(filename,"r");
    if(f == NULL) {
        fprintf(stderr, "open %s file error!\n", filename);
        exit(-1);
    }
    while(1) {
        if(fgets(sline,300,f) == 0) break;
        sscanf(sline,"%s",name);
        hash_add(mer_set, name, mer_set_size);
    }
    fclose(f);
}/*}}}*/
 
char *shoptype(const char *str)
{/*{{{*/
    int i,j,L;
    L = strlen(str);
    char *strtmp = (char *)ab_calloc(L+2, sizeof(char));
    if(L < 4){
        strcpy(strtmp, "");
        return strtmp;
    }
    for(i=4;i<=L;i++) {
        for(j=0;j<i;j++)
            strtmp[j] = str[L-i+j];
        strtmp[i] = '\0';
        if(hash_in(mer_set, strtmp, mer_set_size))
            return strtmp;
    }
    strcpy(strtmp, "");
    return strtmp;
}/*}}}*/

uniseg *get_uniseg(const char *str)
{/*{{{*/
    int len = strlen(str);
    char gbinit[len][4];
    int i,gblen = 0;
    for(i = 0;i < len;)
    {
        if(str[i] > 0)
        {
            gbinit[gblen][0] = str[i];
            gbinit[gblen][1] = '\0';
            i++; gblen++;
            continue;
        }
        //gbcode-include(badcode and goodcode)
        if( (i + 1) >= len)//badcode
        {
            gbinit[gblen][0] = str[i];
            gbinit[gblen][1] = '\0';
            i++; gblen++;
            break;
        }
        else//goodcode
        {
            gbinit[gblen][0] = str[i];
            gbinit[gblen][1] = str[i+1];
            gbinit[gblen][2] = '\0';
            i += 2; gblen++;
        }
    }
    uniseg *seg = (uniseg *)ab_calloc(1, sizeof(uniseg));
    seg->uc = (char **)ab_calloc(gblen, sizeof(char *));
    for(i=0;i<gblen;i++)
        seg->uc[i] = abstru_strdup(gbinit[i]);
    seg->length = gblen;
    return seg;
}/*}}}*/

bool issametype(const char *tp1, const char *tp2)
{/*{{{*/
    const char szhui[4] = "会";
    int i;
    if(abstru_endswith(tp1, szhui) && strcmp(tp1,tp2) != 0)
        return FALSE;
    if(strlen(tp1) ==0 || strlen(tp2) == 0)
        return TRUE;
    uniseg *useg = get_uniseg(tp2);
    for(i=0;i<useg->length;i++) {
        if(abstru_strstr_gb(tp1, useg->uc[i])) {
            uni_free(useg);
            return TRUE;
        }
    }
    uni_free(useg);
    return FALSE;
}/*}}}*/

bool isprepost(const char *str1, const char *str2)
{/*{{{*/
    if(strlen(str1) < strlen(str2))
        return FALSE;
    return abstru_startswith(str1, str2) || abstru_endswith(str1, str2);
}/*}}}*/

hashnode **seg_dict;
const int seg_dict_size = 150001;
bool has_load_seg = FALSE;
void load_seg_once(const char *szDictDir)
{/*{{{*/
    if(has_load_seg) return;
    has_load_seg = TRUE;
    char sline[302], name[300];
    char seg_filename_list[4][32] = {"basicDict.txt", "groupDict.txt", "merchant.feat", "place.dict"};
    int seg_filename_len = 4;
    char filename[600];
    seg_dict = hash_new(seg_dict_size);
    int i;
    FILE *f;
    for(i=0;i<seg_filename_len;i++) {
        sprintf(filename,"%s/%s",szDictDir,seg_filename_list[i]);
        f = fopen(filename,"r");
        if(f == NULL) {
            fprintf(stderr, "open %s file error!\n", filename);
            exit(-1);
        }
        while(1) {
            if(fgets(sline,300,f) == 0) break;
            sscanf(sline,"%s",name);
            hash_add(seg_dict, name, seg_dict_size);
        }
        fclose(f);
    }
}/*}}}*/

segresult *get_segresult(const char *str)
{/*{{{*/
    segresult *seg = (segresult *)ab_calloc(1, sizeof(segresult));
    uniseg *us = get_uniseg(str);
    char **wordspost = (char **)ab_calloc(strlen(str), sizeof(char *));
    int wordslen = 0;
    int i,j,flag,maxpos;
    char *szsuffix = (char *)ab_calloc(strlen(str)+1, sizeof(char));
    char *szmaxsuf = (char *)ab_calloc(strlen(str)+1, sizeof(char));
    char *szcattmp = (char *)ab_calloc(strlen(str)+1, sizeof(char));
    for(i=us->length-1;i>=0;) {
        flag = 0; maxpos = i;
        strcpy(szsuffix, us->uc[i]);
        for(j=i-1;j>=0&&j>=i-4;j--) {
            strcpy(szcattmp, szsuffix);
            strcpy(szsuffix, us->uc[j]);
            strcat(szsuffix, szcattmp);
            if(hash_in(seg_dict, szsuffix, seg_dict_size)) {
                strcpy(szmaxsuf, szsuffix);
                flag = 1; maxpos = j; 
            }
        }
        if(flag == 0) {
            wordspost[wordslen++] = abstru_strdup(us->uc[i]);
            i--;
        }else {
            wordspost[wordslen++] = abstru_strdup(szmaxsuf);
            i = maxpos - 1;
        }
    }
    int seglen = 0, flageven = 0, flagchn = -1;
    char *pword;
    seg->words = (char **)ab_calloc(wordslen, sizeof(char *));
    for(i=wordslen-1;i>=0;i--) {
        pword = wordspost[i];
        if(abstru_strlen_gb(pword) > 1) {
            seg->words[seglen] = (char *)ab_calloc(strlen(str)+1, sizeof(char));
            strcpy(seg->words[seglen++], pword);
            flageven = 0; flagchn = -1;
        }else {
            if(flageven == 0 || (strlen(pword) > 1 && flagchn != 1) || (strlen(pword) <= 1 && flagchn != 0)) {
                seg->words[seglen] = (char *)ab_calloc(strlen(str)+1, sizeof(char));
                strcpy(seg->words[seglen++], pword);
            }else {
                strcat(seg->words[seglen-1], pword);
            }
            flageven = 1;
            if(strlen(pword) > 1)
                flagchn = 1;
            else
                flagchn = 0;
        }
    }
    seg->length = seglen;
    uni_free(us);
    for(i=0;i<wordslen;i++)
        free(wordspost[i]);
    free(wordspost);
    free(szsuffix); free(szmaxsuf); free(szcattmp);
    return seg;
}/*}}}*/

bool isnest(const char *str1, const char *str2)
{/*{{{*/
    segresult *ws1, *ws2;
    ws1 = get_segresult(str1);
    ws2 = get_segresult(str2);
    int i,j,flag=0;
    if(ws2->length == 1 && ws1->length > 1 && abstru_endswith(ws1->words[ws1->length - 1], str2)) {
        seg_free(ws1); seg_free(ws2);
        return FALSE;
    }
    if(ws2->length == 1) {
        for(i=0;i<ws1->length;i++)
            if(abstru_startswith(ws1->words[i], str2)) {
                flag = 1; break;
            }
        if(flag == 0) {
            seg_free(ws1); seg_free(ws2);
            return FALSE;
        }
    }
    i = j = 0;
    while(i < ws1->length && j < ws2->length)
    {
        if(isprepost(ws1->words[i], ws2->words[j])) {
            i++; j++;
        }else {
            i++;
        }
    }
    if(j == ws2->length) {
        seg_free(ws1); seg_free(ws2);
        return TRUE;
    }else {
        seg_free(ws1); seg_free(ws2);
        return FALSE;
    }
}/*}}}*/

const char short_list[15][4] = {"学", "院", "局", "委", "办", "协", "大", "小", "中", "幼", "校", "旅", "社", "会"};
const int short_list_size = 14;
bool isshort(const char *str1, const char *str2)
{/*{{{*/
    int flag = 0, i, j, k;
    for(i=0;i<short_list_size;i++)
        if(abstru_endswith(str1, short_list[i]) || abstru_endswith(str2, short_list[i]))
            flag = 1;
    if(flag == 0) return FALSE;
    segresult *ws1 = get_segresult(str1);
    uniseg *us2 = get_uniseg(str2);
    uniseg *us1;
    i = j = 0;
    while(i<ws1->length && j<us2->length) {
        us1 = get_uniseg(ws1->words[i]);
        if(strcmp(us1->uc[0], us2->uc[j]) == 0) {
            j++;
            for(k=1;k<us1->length;k++) {
                if(j<us2->length && strcmp(us1->uc[k], us2->uc[j]) == 0)
                    j++;
            }
        }else if(strcmp(us1->uc[us1->length - 1], us2->uc[j]) == 0)
            j++;
        i++;
        uni_free(us1);
    }
    if(j == us2->length) {
        seg_free(ws1); uni_free(us2);
        return TRUE;
    }
    seg_free(ws1); uni_free(us2);
    return FALSE;
}/*}}}*/

const int replace_list_size = 99;
char **replace_list_first;
char **replace_list_second;
bool has_load_replace = FALSE;
void load_replace_once(const char *szDictDir)
{/*{{{*/
    if(has_load_replace) return;
    has_load_replace = TRUE;
    replace_list_first = (char **)ab_calloc(replace_list_size, sizeof(char *));
    replace_list_second = (char **)ab_calloc(replace_list_size, sizeof(char *));
    char sline[302], name1[300], name2[300];
    char filename[600];
    sprintf(filename,"%s/abbreviation.txt",szDictDir);
    FILE *f = fopen(filename,"r");
    if(f == NULL) {
        fprintf(stderr, "open %s file error!\n", filename);
        exit(-1);
    }
    int p = 0;
    while(1) {
        if(fgets(sline,300,f) == 0) break;
        sscanf(sline,"%s%s",name1,name2);
        replace_list_first[p] = abstru_strdup(name1);
        replace_list_second[p] = abstru_strdup(name2);
        p++;
    }
    fclose(f);
}/*}}}*/

bool anymatch(const char *name1, const char *name2, int p)
{/*{{{*/
    char *first, *second, *newstr;
    char *spt1, *spt2;
    abstring *abstr;
    if(p == replace_list_size) {
        spt1 = shoptype(name1);
        spt2 = shoptype(name2);
        if(!issametype(spt1, spt2)) {
            free(spt1); free(spt2);
            return FALSE;
        } else if(isnest(name1, name2) || isshort(name1, name2)) {
            free(spt1); free(spt2);
            return TRUE;
        } else {
            free(spt1); free(spt2);
            return FALSE;
        }
    }else {
        abstr = abstring_new(name2);
        first = replace_list_first[p];
        second = replace_list_second[p];
        if(abstru_strstr_gb(name2, first)) {
            abstring_replace_last_gb(abstr, first, second);
            newstr = abstr->str;
            newstr[abstr->len] = '\0';
            if(anymatch(name1, newstr, p+1)) {
                abstring_free(abstr, 1);
                return TRUE;
            }
        }
        abstring_free(abstr, 1);
        if(anymatch(name1, name2, p+1))
            return TRUE;
        return FALSE;
    }
}/*}}}*/

bool iscandidate(uint16_t *r1, uint16_t len1, uint16_t *r2, uint16_t len2)
{/*{{{*/
    int dp[257][257], i, j, ans;
    if(r1[len1-1] == '\n')
        len1--;
    if(r2[len2-1] == '\n')
        len2--;
    /*whether the last charactor is always chr(10) */
    //assert(len1 <= 256 && len2 <= 256 && len1 >= len2);
    if(len1 > 256 || len2 > 256 || len1 < len2)
        return FALSE;

    for(i=0;i<=len1;i++)
        dp[0][i] = dp[i][0] = 0;
    for(i=1;i<=len1;i++)
        for(j=1;j<=len2;j++) {
            if(dp[i-1][j] > dp[i][j-1])
                dp[i][j] = dp[i-1][j];
            else
                dp[i][j] = dp[i][j-1];
            if(r1[i-1] == r2[j-1] && dp[i-1][j-1] + 1 > dp[i][j])
                dp[i][j] = dp[i-1][j-1] + 1;
        }
    ans = dp[len1][len2];
    if(ans >= len2 && ans >= 2)
        return TRUE;
    else
        return FALSE;
}/*}}}*/

int short_name_simi(const char *szDictDir,
                    const ABU_NAME_S *pNameUnitLeft,
                    const ABU_NAME_S *pNameUnitRight)
{/*{{{*/
    ABU_NAME_S *left, *right;
    if(pNameUnitLeft->stpNameAttach->gbOriginNameLen > pNameUnitRight->stpNameAttach->gbOriginNameLen) {
    /* make the length of $left larger than $right */
        left = (ABU_NAME_S *)pNameUnitLeft; 
        right = (ABU_NAME_S *)pNameUnitRight;
    }else {
        left = (ABU_NAME_S *)pNameUnitRight; 
        right = (ABU_NAME_S *)pNameUnitLeft;
    }
    if(!iscandidate(left->stpNameAttach->gbOriginName, left->stpNameAttach->gbOriginNameLen, 
                right->stpNameAttach->gbOriginName, right->stpNameAttach->gbOriginNameLen))
        return 0;
    char *P1, *P2, *Q1, *Q2, *R1, *R2;
    load_place_once(szDictDir);
    load_mer_once(szDictDir);
    load_replace_once(szDictDir);
    load_seg_once(szDictDir);
    P1 = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    if(left->stpNameRecord->szCity != NULL && !abstru_strstr_gb(left->stpNameRecord->szStem, left->stpNameRecord->szCity)) {
        strcpy(P1, left->stpNameRecord->szCity);
        strcat(P1, left->stpNameRecord->szStem);
    }else
        strcpy(P1, left->stpNameRecord->szStem);
    if(left->stpNameRecord->szType != NULL)
        strcat(P1, left->stpNameRecord->szType);
    if(left->stpNameRecord->szOrg != NULL)
        strcat(P1, left->stpNameRecord->szOrg);

    P2 = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    if(right->stpNameRecord->szCity != NULL && !abstru_strstr_gb(right->stpNameRecord->szStem, right->stpNameRecord->szCity)) {
        strcpy(P2, right->stpNameRecord->szCity);
        strcat(P2, right->stpNameRecord->szStem);
    }else
        strcpy(P2, right->stpNameRecord->szStem);
    if(right->stpNameRecord->szType != NULL)
        strcat(P2, right->stpNameRecord->szType);
    if(right->stpNameRecord->szOrg != NULL)
        strcat(P2, right->stpNameRecord->szOrg);

    Q1 = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    if(left->stpNameRecord->szAssoPrefix == NULL)
        strcpy(Q1, "");
    else
        strcpy(Q1, left->stpNameRecord->szAssoPrefix);
    if(left->stpNameRecord->szAssoSuffix != NULL)
        strcat(Q1, left->stpNameRecord->szAssoSuffix);

    Q2 = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    if(right->stpNameRecord->szAssoPrefix == NULL)
        strcpy(Q2, "");
    else
        strcpy(Q2, right->stpNameRecord->szAssoPrefix);
    if(right->stpNameRecord->szAssoSuffix != NULL)
        strcat(Q2, right->stpNameRecord->szAssoSuffix);

    R1 = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    if(left->stpNameRecord->szAppendix == NULL)
        strcpy(R1, "");
    else
        strcpy(R1, left->stpNameRecord->szAppendix);

    R2 = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    if(right->stpNameRecord->szAppendix == NULL)
        strcpy(R2, "");
    else
        strcpy(R2, right->stpNameRecord->szAppendix);

    char *PRQ = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    char *PQ1, *PQ2;
    int ans = -1;
    strcpy(PRQ, P1);
    strcat(PRQ, R1);
    strcat(PRQ, Q1);
    PQ1 = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    strcpy(PQ1, P1);
    strcat(PQ1, Q1);
    PQ2 = (char *)ab_calloc(MAXNAMELEN, sizeof(char));
    strcpy(PQ2, P2);
    strcat(PQ2, Q2);
    bool p2isplace = isplace(P2);
    if(p2isplace && !isshort(P1,P2) && !abstru_endswith(PRQ, P2)) //P1+R1+Q1.endswith(P2)
        ans = 0;
    if(strlen(Q1) == 0 && strlen(R1) == 0 && strlen(Q2) == 0 && strlen(R2) == 0) {
        if(anymatch(P1,P2,0) && !p2isplace)
            ans = 75;
    }else if(strlen(Q1) == 0 && strlen(R1) != 0 && strlen(Q2) == 0 && strlen(R2) == 0) {
        /*if(isasso(R1) && !isasso(P2)) */
        if(anymatch(P1, P2, 0) && !p2isplace) {
            ans = 60; /*P2 may be place, strictly setting*/
        }
    }else if(strlen(Q1) != 0 && strlen(Q2) == 0 && strlen(R2) == 0) {
        if(anymatch(Q1, P2, 0) || anymatch(PQ1, P2, 0))
            ans = 80;
    }else if(strlen(R1) == 0 && strlen(Q1) != 0 && strlen(R2) == 0 && strlen(Q2) != 0) {
        if((anymatch(P1, P2, 0) && anymatch(Q1, Q2, 0)) || anymatch(PQ1, PQ2, 0))
            ans = 90;
    }else
        ans = 70;
    free(P1);free(Q1);free(R1);
    free(P2);free(Q2);free(R2);
    free(PRQ);free(PQ1);free(PQ2);
    /****
    hash_free(place_set, place_set_size);
    hash_free(mer_set, mer_set_size);
    hash_free(seg_dict, seg_dict_size);
    int p;
    for(p=0;p<replace_list_size;p++) {
        free(replace_list_first[p]); free(replace_list_second[p]);
    }
    free(replace_list_first); free(replace_list_second);
    ****/
    if(ans != -1)
        return ans;
    else
        return 0;
}/*}}}*/
