/*
 * =====================================================================================
 *
 *       Filename:  IndexLoader.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/09/12 12:05:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include "aaa.pb.h"
#include "file_reader.h"
#include "IndexLoader.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/algorithm/string/regex.hpp>
using namespace std;
using namespace xce::ad;


void IndexPreprocessor::BasePreProcessor::run(){
	follower::DiffPackage inc_pack;
	fstream input(filename_.c_str(), ios::in | ios::binary);
	if(!input){
		MCE_WARN("BasePreProcessor::run ,Read file Error:" << filename_);
		return;
	}
	if (!inc_pack.ParseFromIstream(&input)) {
		MCE_WARN("BasePreProcessor::run ,Failed to parseFromIstream." << filename_ );
		return ;
	}
	std::string table = inc_pack.tablename();
	for(int i = 0; i < inc_pack.data_size(); ++i){
		IndexItem item(table);
		const follower::DiffPackage_Row& row = inc_pack.data(i);
		item.fields.push_back(row.key());
		for(int j = 0; j < row.values_size(); ++j){
			item.fields.push_back(row.values(j));
		}
		IndexPreprocessor::instance().AddBaseItem(item);
	}
	//std::vector<std::vector<std::string> > &items = IndexPreprocessor::instance()->GetBaseItems(table_name_);
 /*
	 	FILE *fp = fopen(filename_.c_str(), "rb");
  MCE_INFO("BasePreProcessor::run, filename:" << filename_ << "table:" <<  table_name_);
  if(!fp){
    MCE_WARN("BasePreProcessor::run, open file failed:" << filename_);  
    return ;
  }

  file_line_reader_t file_reader;
  if(file_reader.Create(1024*1024*100, fp)) {
    MCE_WARN("BasePreProcessor::run, file_reader create error");
    fclose(fp);
    return ;
  }

  for(;;){
    char *buf = new char[2048];
    int line_length = -1;

    while( (line_length=file_reader.GetLine(buf, 2048)) >= 0 && line_length >= 2048){
    }

    if(line_length<0) {
      if(line_length == -1){
        MCE_INFO("BasePreProcessor::run, base file read over," <<  filename_);
        break;
      }else{
        MCE_WARN("BasePreProcessor::run, base file " << filename_ << "read error,get_line return "<< line_length);
        fclose(fp);
        delete []buf;
        return;
      }
    }

		follower::DiffPackage inc_pack;
		inc_pack.ParseFromString(buf);
		std::string table = inc_pack.tablename();
		for(int i = 0; i < inc_pack.data_size(); ++i){
			IndexItem item(table);
			const follower::DiffPackage_Row& row = inc_pack.data(i);
			item.fields.push_back(row.key());
			for(int j = 0; j < row.values_size(); ++j){
				item.fields.push_back(row.values(j));
			}
    	IndexPreprocessor::instance()->AddBaseItem(item);
		}
		//MCE_INFO("Origin Content:" << string(buf));
    //boost::algorithm::split_regex(item.fields, buf, boost::regex("[ \t]+"));
    //IndexPreprocessor::instance()->AddBaseItem(item);
    delete []buf;

  }
*/
  IndexPreprocessor::instance().SetBaseReady(table_name_);
  //fclose(fp);
}



void IndexPreprocessor::RegisterTable(const std::vector<std::string> &tables){
	std::ostringstream  out;
	copy(tables.begin(), tables.end(), std::ostream_iterator<std::string>(out, ","));
	MCE_INFO("IndexPreprocessor::RegisterTable, "<< out.str());
  for(std::vector<std::string>::const_iterator it = tables.begin(); it != tables.end(); ++it){
    IndexProgressor progressor;
    progressor.table_name_ = *it;
    progressor.is_base_ready_ = false; 
    progressor.is_inc_ready_ = false; 
    index_progress_list_.push_back(progressor);
    ++table_num_;
  }
}

bool IndexPreprocessor::CurrentTableReady(){
  IndexProgressor &progressor = index_progress_list_.front();
  return progressor.is_base_ready_ && progressor.is_inc_ready_;
}

void IndexPreprocessor::GetIndex(std::string &table, std::vector<IndexItem> &base_items,
    std::vector<IndexItem> &inc_items){

  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  while(!CurrentTableReady())
    wait();
  IndexProgressor progressor = index_progress_list_.front();
  index_progress_list_.pop_front();
  std::string tn = progressor.table_name_;
  base_items = base_items_[tn];
  inc_items = base_items_plus_[tn];
  table = tn;
  ++loaded_num_;
}

void IndexPreprocessor::SetBaseReady(const std::string &table){
  for(std::list<IndexProgressor>::iterator it = index_progress_list_.begin(); it != index_progress_list_.end(); ++it){
    if(it->table_name_ == table){
      if(it->is_base_ready_){
        MCE_INFO("IndexPreprocessor::SetBaseReady table:" << table << ", base index is already ready"); 
        return;
      }else{
        MCE_INFO("IndexPreprocessor::SetBaseReady table:" << table << ", set base ready!");
        it->is_base_ready_ = true;
        if(it->is_inc_ready_){
          IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
          ready_table_list_.push_back(table);
          notify();
        }
        return;
      }
    }else{
      if(!it->is_base_ready_){
        MCE_INFO("IndexPreprocessor::SetBaseReady set base(" << table << ") ready, but table( " << it->table_name_ << ") is not ready\n"); 
      }
    }
  }
}

void IndexPreprocessor::SetIncReady(const std::string &table){
  for(std::list<IndexProgressor>::iterator it = index_progress_list_.begin(); it != index_progress_list_.end(); ++it){
    if(it->table_name_ == table){
      if(it->is_inc_ready_){
        MCE_INFO("IndexPreprocessor::SetIncReady table:" << table << ", inc index is already ready");
        return;
      }else{
        MCE_INFO("IndexPreprocessor::SetIncReady table:" << table  << ", set inc ready");
        it->is_inc_ready_ = true;
        if(it->is_base_ready_){
          IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
          ready_table_list_.push_back(table); 
          notify();
        }
        return;
      }
    }else{
      if(!it->is_inc_ready_){
        MCE_INFO("IndexPreprocessor::SetIncReady set inc(" << table << ") ready, but table(" << it->table_name_ << ") is not ready\n");
      }
    }
  }
}

bool IndexPreprocessor::IsBaseReady(const std::string &table){
  std::list<IndexProgressor>::iterator it = index_progress_list_.begin();
  for(; it != index_progress_list_.end(); ++it){
    if(it->table_name_ == table){
      break;
    }
  }
  if(it == index_progress_list_.end()){
    MCE_WARN("IndexPreprocessor::IsBaseReady table:" << table << " is not registerd, IsBaseReady failed");
    return false;
  }else{
    return it->is_base_ready_;
  }
}

bool IndexPreprocessor::IsIncReady(const std::string &table){
  std::list<IndexProgressor>::iterator it = index_progress_list_.begin();
  for(; it != index_progress_list_.end(); ++it){
    if(it->table_name_ == table){
      break;
    }
  }
  if(it == index_progress_list_.end()){
    MCE_WARN("IndexPreprocessor::IsIncReady table:" << table << " is not registerd, IsIncReady failed");
    return false;
  }else{
    return it->is_inc_ready_;
  }
}

std::vector<IndexItem>& IndexPreprocessor::GetBaseItems(const std::string &tn){
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  return base_items_[tn];
}

std::vector<IndexItem>& IndexPreprocessor::GetBasePlusItems(const std::string &tn){
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  return base_items_plus_[tn];
}

std::vector<IndexItem> IndexPreprocessor::GetIncItems(){
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  while(inc_items_.empty()){
    wait();
  }
  std::vector<IndexItem> incs;
  incs.swap(inc_items_);
  return incs;
}

void IndexPreprocessor::AddIncItem(const IndexItem &item){
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  if(IsIncReady(item.table)){
    inc_items_.push_back(item);
    notify();
  }else{
    base_items_plus_[item.table].push_back(item);
  }
}

void IndexPreprocessor::AddBaseItem(const IndexItem &item){
	//MCE_DEBUG("IndexPreprocessor::AddBaseItem, table:"<< item.table << " size:" << item.fields.size());
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  base_items_[item.table].push_back(item);
}

int IndexPreprocessor::PreprocessBaseIndex(const std::string &table, const std::string &file){
  IceUtil::Handle<BasePreProcessor> base_preprocessor = new BasePreProcessor(file, table);
  base_preprocessor->process();
  return 0;
}


void IndexPreprocessor::BasePreProcessor::process(){
  start().detach();
}
