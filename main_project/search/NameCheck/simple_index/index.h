#ifndef __INDEX_H_
#define __INDEX_H__

#include <string>

using namespace std;

class IndexInfo
{
	public:
		bool operator>(const IndexInfo &r_index);
		bool operator<(const IndexInfo &r_index);
		bool operator==(const IndexInfo &r_index);
	

		long long id;
		unsigned int offset;
		unsigned int length;
};

struct DocInfo
{
	string key;
	string value;
};

class Indexer
{
	public:
		Indexer(const char *index_file,const char *data_dir,unsigned chunk_size);
		Indexer();
		
		~Indexer();

		bool addDoc(DocInfo& newdoc );
		bool addDocInc(DocInfo& newdoc );
		bool searchDocByKey(const string& key,DocInfo& doc);
		bool sortIndex();

	private:
		int _initEnv();
		int _rebulidIndex();
		int _dumpIndexToFile();

		int _addValueToFile(const string& fileName,DocInfo& newdoc,IndexInfo& index);

		int _addIndexInfoToMem( IndexInfo& index );
		int _addIndexInfoToFile( IndexInfo& index );
		
		int _addIndexInfoToSortedMem( IndexInfo& index );

		int _seekIndex(const char *key,IndexInfo& index);

		int _getDocFileName(long long key,string& name);
		
		int _getIndexInfo(const string& key,IndexInfo& index);
		int _searchKey(long long id,IndexInfo& index);
		int _bsearchKey(long long id,IndexInfo& index);
		int _getDocInfo(const string& fileName,IndexInfo& index,DocInfo& doc);
		
	private:
		string _data_dir;
		string _index_file;

		IndexInfo *_mem_index;
		unsigned int _index_count;
		unsigned int _chunk_size;
};

class IndexSearcher
{
	public:
		IndexSearcher(const char *index_file,const char *data_file);
		~IndexSearcher();
		
		bool searchDocByKey(const string& key,DocInfo& doc);
		bool searchDocByOffset(size_t idx , DocInfo& doc);
		size_t getIndexCount();
		
	private:
		int _initEnv();
		int _load_index();
		int _load_data();
		
		int _close_index();
		int _close_data();
		
		int _getIndexInfo(const string& key,IndexInfo& index);
		int _getDocInfo(IndexInfo& index,DocInfo& doc);
		int _bsearchKey(long long id,IndexInfo& index);
		
	private:
		string _data_file;
		string _index_file;
		
		IndexInfo *_mem_index;
		void *_mem_data;

		unsigned int _index_count; 
};

#endif
