#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include "utility.h"
#include "md5_finger.h"
#include "index.h"

using namespace std;

static const string DEFAULT_INDEX_FILE = "./index/index.dat";
static const string DEFAULT_DATA_DIR = "./index/data/";

static const unsigned int DEFAULT_DATA_CHUNK_SIZE = 100;

static const unsigned int NORMAL_BUF_SIZE = 1024*10;
static const unsigned int MID_BUF_SIZE = 1024 * 1024 * 100;
static const unsigned int MAX_DOC_SIZE = 10000000;

static const char DELIMETER = '\t';


bool IndexInfo::operator>(const IndexInfo& r_info)
{
	return (this->id - r_info.id) > 0 ?true:false;
}

bool IndexInfo::operator<(const IndexInfo& r_info)
{
	return ( this->id - r_info.id) < 0 ?true:false;
}

bool IndexInfo::operator==(const IndexInfo& r_info)
{
	return ( this->id == r_info.id) ?true:false;
}

static void _dump_doc(const DocInfo& doc)
{
	//cout<<"Dump_Doc::"<<doc.key<<"	"<<doc.key.size()<<"	"<<doc.value<<"	"<<doc.value.size()<<endl;
}

static void _dump_index_info(const IndexInfo& index)
{
	//cout<<"Dump_IndexInfo::"<<index.id<<"	"<<index.offset<<"	"<<index.length<<endl;
}

static void _dump_index_memory(int limit,IndexInfo* mem_index)
{
	int idx = 0;
	while( idx < limit )
	{
		//cout<<idx<<": index mem::"<<mem_index[idx].id<<"-"<<mem_index[idx].offset<<"-"<<mem_index[idx].length<<endl;
		idx++;
	}

}


Indexer::Indexer():_data_dir( DEFAULT_DATA_DIR ),_index_file( DEFAULT_INDEX_FILE ),_chunk_size(DEFAULT_DATA_CHUNK_SIZE),_index_count(0),_mem_index(NULL)
{
	_initEnv();
}

Indexer::Indexer(const char *index_file,const char *data_dir,unsigned chunk_size):_data_dir( data_dir ),_index_file( index_file ),_chunk_size( chunk_size ),_index_count(0),_mem_index(NULL)
{
	_initEnv();
}

Indexer::~Indexer()
{
	delete []_mem_index;
}

int Indexer::_initEnv()
{
	//1.1 malloc memory
	_mem_index = new IndexInfo[ MAX_DOC_SIZE ];
	if( _mem_index == NULL )
	{
		print_msg("Indexer::_initEnv() malloc memory fail!");
		return -1;
	}

	//1.2 rebuild index from index-file
	if( _rebulidIndex() == -1 )
	{
		print_msg("Indexer::_initEnv() rebuild index fail!");
		return -1;
	}
	
	print_msg("Indexer::_initEnv() finished load::"+int2s(_index_count));
	//_dump_index_memory( 100,_mem_index);

	return 1;
}

bool Indexer::sortIndex()
{
	//1.1 memory sort by qsort
	//sortIndexInfo(_mem_index,_index_count);
	Sorter<IndexInfo>::sort(_mem_index,0,_index_count);

	//1.2 dump to index file
	if( _dumpIndexToFile() == -1 )
	{
		print_msg("Indexer::sortIndex() dump index to file fail!");
		return false;
	}

	//_dump_index_memory( 100,_mem_index);

	return true;
}

int Indexer::_rebulidIndex()
{
	//1.1 open the index file
	FILE *pfile = NULL;
	pfile = fopen( _index_file.c_str() , "r");
	if( !pfile  )
	{
		print_msg("Indexer::_rebuildIndex() open index file fail!");
		return -1;
	}

	//2.1 add item to memory
	string line;
	IndexInfo tmp_index[1];
	while( fread(tmp_index,sizeof(IndexInfo),1,pfile))
	{
		IndexInfo new_index_item = tmp_index[0];

		if( _addIndexInfoToMem( new_index_item ) == -1 )
		{
			print_msg("Indexer::_rebuildIndex() add index to mem fail!");
			return -1;
		}
	}

	//_dump_index_memory( _index_count,_mem_index);

	fclose( pfile );
}

int Indexer::_dumpIndexToFile()
{
	//1.1 open the index file
	FILE *pfile = NULL;
	pfile = fopen( _index_file.c_str(), "w");
	if( !pfile  )
	{
		print_msg("Indexer::_dumpIndexToFile() open index file fail!");
		return -1;
	}

	//2.1 dump all memory items to file
	if( fwrite( _mem_index ,sizeof(IndexInfo), _index_count,pfile) < _index_count )
	{
		print_msg("Indexer::_dumpIndexToFile() add index_info fail!");
		return -1;
	}

	fclose( pfile );

	return 1;
}



int Indexer::_addIndexInfoToMem( IndexInfo& index )
{
	if( _index_count >= MAX_DOC_SIZE )
	{
		print_msg("Indexer::_addIndexInfoToMem add index to mem exceed maxium doc_size 15000000 !");
		return -1;
	}

	unsigned int *index_offset = (unsigned int*)( _mem_index + sizeof( IndexInfo ) * _index_count ); 


	_mem_index[ _index_count ] = index;

	_index_count++;

	return 1;
}

int Indexer::_addIndexInfoToSortedMem( IndexInfo& index )
{
	if( _index_count >= MAX_DOC_SIZE )
	{
		print_msg("Indexer::_addIndexInfoToSortedMem add index to mem exceed maxium doc_size!");
		return -1;
	}

	//1.1 get proper idx: 
	int idx = 0;
	if( _index_count == 0 )
	{
		idx = 0;
	}
	else
	{
		while( idx < _index_count  &&  index > _mem_index[idx] ) idx++;
	}
	
	//2.1 move elements if necessary;
	//moveElement( _mem_index,_index_count,idx,1);
	if( _index_count > 0 && idx < _index_count - 1 )
	{
		int p = _index_count - 1 ;
		while( p >= idx ) 
		{
			_mem_index[ p + 1 ] = _mem_index[p];
			p--;
		}
	}
	
	_mem_index[ idx ] = index;

	_index_count++;

	return 1;
}

int Indexer::_addValueToFile( const string& fileName,DocInfo& newdoc,IndexInfo& index )
{
	FILE *pfile = NULL;
	pfile = fopen( fileName.c_str(), "a");
	if( !pfile  )
	{
		print_msg("Indexer::_addValueToFile open data file fail!");
		return -1;
	}

	index.offset = ftell( pfile );
	index.length = newdoc.value.size();


	if( fputs( newdoc.value.c_str(),pfile) == EOF )
	{
		print_msg("Indexer::_addValueToFile add data to file fail!");
		return -1;
	}

	fclose( pfile );
	return 1;
}

int Indexer::_addIndexInfoToFile( IndexInfo& index )
{
	FILE *pfile = NULL;
	pfile = fopen( _index_file.c_str(), "a");
	if( !pfile  )
	{
		print_msg("_addIndexInfoToFile open index file fail!");
		return -1;
	}

	if( fwrite( &index , sizeof(IndexInfo),1,pfile) < 1 )
	{
		print_msg("_addIndexInfoToFile add index_info fail!");
		return -1;
	}

	fclose( pfile );
	return 1;
}

bool Indexer::addDoc( DocInfo& newdoc )
{
	//1.1 valided info
	if( newdoc.key.empty() || newdoc.value.empty() )
	{
		print_msg("Indexer::addDoc docinfo empty!");
		return false;
	}

	//2 gen index_info
	IndexInfo new_index;

	//2.1 gen md5
	MD5Finger::genMD5FingerInt( newdoc.key , new_index.id );

	//2.2 get doc file name and save data to data file
	string doc_file;
	_getDocFileName( new_index.id , doc_file );

	if( _addValueToFile( doc_file,newdoc,new_index) == -1 )
	{
		print_msg("Indexer::addDoc add doc to file fail!");
		return false;
	}

	//2.3 save index info to file
	if( _addIndexInfoToFile( new_index ) == -1 )
	{
		print_msg("Indexer::addDoc save index info to file fail!");
		return false;
	}

	//2.4 add index info to mem
	if( _addIndexInfoToMem( new_index ) == -1 )
	{
		print_msg("Indexer::addDoc add index info to memory fail!");
		return false;
	}

	return true;
}

bool Indexer::addDocInc( DocInfo& newdoc )
{
	//1.1 valided info
	if( newdoc.key.empty() || newdoc.value.empty() )
	{
		print_msg("Indexer::addDocInc docinfo empty!");
		return false;
	}

	//2 gen index_info
	IndexInfo new_index;

	//2.1 gen md5
	MD5Finger::genMD5FingerInt( newdoc.key , new_index.id );

	//2.2 get doc file name and save data to data file
	string doc_file;
	_getDocFileName( new_index.id , doc_file );

	if( _addValueToFile( doc_file,newdoc,new_index) == -1 )
	{
		print_msg("Indexer::addDocInc add doc to file fail!");
		return false;
	}

	//2.3 add index info to sorted mem 
	if( _addIndexInfoToSortedMem( new_index ) == -1 )
	{
		print_msg("Indexer::addDocInc add index info to memory fail!");
		return false;
	}

	//2.4 dunp_index to index-file
	if( _dumpIndexToFile() == -1 )
	{
		print_msg("Indexer::addDocInc dump mem_index to file fail!");
		return false;
	}

	return true;
}

bool Indexer::searchDocByKey(const string& key,DocInfo& doc)
{
	if( key.empty() )
	{
		print_msg("Indexer::searchDocByKey empty key fail!");
		return false;
	}

	IndexInfo index;

	//1.1 get index info by key
	if( _getIndexInfo( key,index) == -1 )
	{
		print_msg("Indexer::searchDocByKey get index_info fail!");
		return false;
	}

	string data_file;

	//1.2 search doc from data file
	_getDocFileName(index.id,data_file);

	if( _getDocInfo( data_file , index ,doc ) == -1 )
	{
		print_msg("Indexer::searchDocByKey get doc_info fail!");
		return false;
	}

	doc.key = key;

	return true;
}

int Indexer::_getDocFileName(long long key,string& name)
{
	name = _data_dir + "/" + int2s( key % _chunk_size ) +".data";
	return 1;
}

int Indexer::_getIndexInfo(const string& key,IndexInfo& index)
{
	long long id = 0;

	//1.1 gen md5 id
	MD5Finger::genMD5FingerInt( key , id );

	//1.2 search index by md5 id
	if( _bsearchKey( id , index) == -1 )
	{
		return -1;
	}

	return 1;
}

int Indexer::_searchKey(long long key,IndexInfo& index)
{
	//simple imple sequence lookup
	int idx = 0;
	while( idx < _index_count )
	{
		//haha,i got it...
		if( _mem_index[idx].id == key )
		{
			index = _mem_index[idx];
			return 1;
		}

		idx++;
	}

	return -1;
}

int Indexer::_bsearchKey(long long key,IndexInfo& index)
{
	IndexInfo key_info;
	key_info.id = key;
	
	IndexInfo *ptr_item = Sorter<IndexInfo>::bsearch(_mem_index,key_info,0,_index_count);

	if( ptr_item == NULL )
		return -1;

	index = *ptr_item;
	return 1;
}

int Indexer::_getDocInfo(const string& fileName,IndexInfo& index,DocInfo& doc)
{
	FILE *pfile = NULL;
	pfile = fopen( fileName.c_str(), "r");
	if( !pfile  )
	{
		print_msg("Indexer::_getDocInfo open data file:"+ fileName+ "fail!");
		return -1;
	}

	if( fseek( pfile,index.offset,SEEK_SET)  != 0 )
	{
		print_msg("Indexer::_getDocInfo seek data file fail!");
		return -1;
	}

	char *buf = new char[ MID_BUF_SIZE ];
	//here need to revise:
	int length = index.length + 1;
	if( !fgets( buf,length,pfile) )
	{
		print_msg("Indexer::_getDocInfo read data file fail!");
		return -1;
	}

	doc.value = buf;

	delete []buf;
	fclose( pfile );

	return 1;
}

IndexSearcher::IndexSearcher(const char *index_file,const char *data_file):_data_file( data_file ),_index_file( index_file )
{
	_mem_index = NULL;
	_mem_data = NULL;
	_index_count = 0;
	
	_initEnv();
}

IndexSearcher::~IndexSearcher()
{
	_close_index();
	_close_data();
}

int IndexSearcher::_initEnv()
{
	//1.1 load index
	if( _load_index() == -1 )
	{
		print_msg("IndexSearcher::_initEnv() load index fail!");
		return -1;
	}

	//1.2 load data
	if( _load_data() == -1 )
	{
		print_msg("IndexSearcher::_initEnv() load data fail!");
		return -1;
	}
	
	print_msg("IndexSearcher::_initEnv() finished load::"+int2s(_index_count));
	//_dump_index_memory( 100,_mem_index);
}

int IndexSearcher::_load_index()
{
	void *ptr_mem = MMapFile( _index_file.c_str() );
	if( ptr_mem == NULL )
		return -1;

	_mem_index = (IndexInfo*) ptr_mem;

	int file_size = getFileSize( _index_file.c_str() );
	_index_count = file_size/sizeof(IndexInfo);
	
	return 1;
}

int IndexSearcher::_load_data()
{
	void *ptr_mem = MMapFile( _data_file.c_str() );
	if( ptr_mem == NULL )
		return -1;

	_mem_data = (char*) ptr_mem;
	
	return 1;
}

int IndexSearcher::_close_index()
{
	if( !UnMMapFile(_index_file.c_str(), _mem_index ) )
	{
		print_msg("IndexSearcher::_close_index fail!");
		return -1;
	}

	return 1;
}

int IndexSearcher::_close_data()
{
	if( !UnMMapFile(_data_file.c_str(), _mem_data ) )
	{
		print_msg("IndexSearcher::_close_data fail!");
		return -1;
	}

	return 1;
}


bool IndexSearcher::searchDocByKey(const string& key,DocInfo& doc)
{
	if( key.empty() )
	{
		print_msg("IndexSearcher::searchDocByKey empty key fail!");
		return false;
	}

	IndexInfo index;

	//1.1 get index info by key
	if( _getIndexInfo( key,index) == -1 )
	{
		print_msg("IndexSearcher::searchDocByKey get index_info fail!");
		return false;
	}

	if( _getDocInfo( index ,doc ) == -1 )
	{
		print_msg("IndexeSearcher::searchDocByKey get doc_info fail!");
		return false;
	}

	doc.key = key;

	return true;
}

bool IndexSearcher::searchDocByOffset(size_t idx , DocInfo& doc)
{
	if( idx >= getIndexCount() )
		return false;
	
	IndexInfo index = *(_mem_index + idx);
	if( _getDocInfo( index ,doc ) == -1 )
	{
		print_msg("IndexeSearcher::searchDocByKey get doc_info fail!");
		return false;
	}

	doc.key = "";
	return true;
}

int IndexSearcher::_getIndexInfo(const string& key,IndexInfo& index)
{
	long long id = 0;

	//1.1 gen md5 id
	MD5Finger::genMD5FingerInt( key , id );

	//1.2 search index by md5 id
	if( _bsearchKey( id , index) == -1 )
	{
		return -1;
	}

	return 1;
}

int IndexSearcher::_bsearchKey(long long key,IndexInfo& index)
{
	IndexInfo key_info;
	key_info.id = key;
	
	IndexInfo *ptr_item = Sorter<IndexInfo>::bsearch(_mem_index,key_info,0,_index_count);

	if( ptr_item == NULL )
		return -1;

	index = *ptr_item;
	return 1;
}


int IndexSearcher::_getDocInfo(IndexInfo& index,DocInfo& doc)
{
	doc.value.assign( (char*)_mem_data+index.offset, index.length );
	return 1;
}

size_t IndexSearcher::getIndexCount()
{
	return _index_count;
}

