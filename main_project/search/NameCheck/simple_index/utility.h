#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <unistd.h>
#include <functional>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

bool print_msg(const string msg);

unsigned int splitTxt(const string& text, const string& delim, vector<string>& array);

unsigned int joinTxt(const vector<string>& array,string& result);

unsigned int joinTxt(const vector<string>& array,const string& connetor,string& result);

string int2s(long long a);

string float2s(float a);

string trimStr(const string& srcStr,const string& trimStr);

string trimLeftStr(const string& srcStr,const string& trimStr);

string trimRightStr(const string& srcStr,const string& trimStr);

string removeStr( const string& srcStr,char ch );

bool replaceBuf(char *buf,char search_char,char replace_char);

bool trimBuf(char *buf,char ch);

bool extractXmlTag(const char *txt,const string& tag,string& ret);

bool xmlToUrlContent(const char *txt,string& url,string& content);

bool xmlToUrlTitleContent(const char *txt,string& url,string& title,string& content);

bool is_number_str(const string& str);
	
bool is_number_str(const char* str);

bool is_alpha_str(const string& str);
	
bool is_alpha_str(const char* str);

bool is_alnum_str(const string& str);
	
bool is_alnum_str(const char* str);

bool is_alnumsym_str(const string& str);
	
bool is_alnumsym_str(const char* str);

bool is_float_str(const string& str);
	
bool is_float_str(const char* str);

int lower_case_str(char *s);

int lower_case_str(string& s);

int lower_case_the_word(vector<string>& word_ary );

int getFileContent(const string& file,string& content);

int getFileSize(const char *filename );

void* MMapFile(const char *filename,int file_open_flag = O_RDONLY,int mem_prot_flag = PROT_READ,int mem_share_flag = MAP_FILE | MAP_SHARED );

bool UnMMapFile(const char *filename,void *ptr_mem);

int convertToBanjiao(uint16_t* ptr_wbuf, int len );

int getRandomNumber();

int getRandomNumber(int start_range,int end_range);

template <typename RandomAccessIterator>
int shuffleArray(RandomAccessIterator first,RandomAccessIterator last)
{
	int n = (last-first);
	for (int i = 0; i < n; i++ ) 
	{
		int rand_idx = getRandomNumber(i,n);
		swap (first[i],first[ rand_idx ]);
	}
}

class Logger
{
	public:
		static void logDebug(const string& ident,const string& msg ); 
		static void logError(const string& ident,const string& msg ); 
	private:
		static void _logMsg(int priority,const string& msg);
};


	template<typename Type>
bool moveElement(Type *base,int count_elem,int idx,int offset)
{
	if( count_elem > 0 && idx < count_elem - 1 && offset > 0 )
	{
		void * src = (void*)( base + sizeof(Type)*( idx ) );
		void * dst = (void*)( base + sizeof(Type)*( idx + offset ) );
		int size = ( count_elem - idx )*sizeof( Type );

		memcpy( dst, src, size );
		return true;
	}

	return false;
}



template<typename Type>
class Sorter
{
	public:
		static void sort(Type *base,int l,int r)
		{
			int i = l-1, j = r, p = l-1, q = r; 
			Type v = base[r];
			if (r <= l) return;
			for (;;) 
			{
				while ( base[++i] < v) ;
				while (v < base[--j]) if (j == l) break;
				if (i >= j) break;
				_swap( base, i , j );
				if ( base[i] == v) { p++; _swap( base, p , i ); }
				if (v == base[j]) { q--; _swap( base , j, q ); }
			}
			_swap(base,i , r ); j = i-1; i = i+1;
			for (int k = l; k < p; k++, j--) _swap(base,k, j);
			for (int k = r-1; k > q; k--, i++) _swap(base, i, k );

			sort(base, l, j);
			sort(base, i, r);
		}

		static Type* bsearch(Type* base, Type& item,int low,int high)
		{
			int mid = (low + high)/2;
			if( low > high )
			{
				return NULL;
			}

			if( base[mid] == item )
			{
				return &base[mid];
			}

			if( item < base[mid] )
				return bsearch(base,item,low,mid - 1 );
			else if( item > base[mid] )
				return bsearch(base,item,mid+1,high);

		}


	private:
		static void _swap(Type *base,int i,int j)
		{
			//cout<<"swap at "<<i<<" "<<j<<endl;
			Type tmpindex = base[j];
			base[j] = base[i];
			base[i] = tmpindex;
		}
};

#endif
