#include "mfsupload.h"
#include <stdio.h>
#include <boost/thread/thread.hpp>
#include <sstream>
#include <iostream>
#include"mfsbundle/mooseclient/moose_c.h"
#include"mfsbundle/bundle/bundle.h"
#include "base3/logging.h"

#define TEST true

namespace net {
MfsPost::MfsPost()
:now_jobs_(0),
will_exit_(false),
max_jobs_(5)
{
}


bool MfsPost::Start(){
 work_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &ios_)));
 return true;
}


bool MfsPost::PostFile(const FileData& filedata){
   
   ios_.post(boost::bind(&MfsPost::HandleNewJob,this,filedata)); 
   std::cout<<"pool inside post:\n";
   return true;
}

void MfsPost::HandleNewJob(const FileData& filedata){
  LOG(WARNING)<<"<>Handle new job ";
  size_t filesize=64*1024;
  std::string prefix="photo_test";
  std::string storage="";
  std::string postfix=".jpg";
  LOG(WARNING)<<"handle mfs post  start";
  boost::shared_ptr<bundle::Writer>writer_tmp=boost::shared_ptr<bundle::Writer>(bundle::Writer::Allocate(prefix.c_str(),postfix.c_str(),filesize,storage.c_str()));
  if(writer_tmp)
    LOG_IF(WARNING,TEST)<<"<>has bundle!!!!! ";
  size_t written;
  std::string url;
  if(writer_tmp){
    int ret=writer_tmp->BatchWrite(filedata.first.c_str(),filedata.second,&written,&url);
  }
}

void MfsPost::Close(){
  will_exit_=true;
  work_->join();
}

bool MfsPool::Start(){

  mp_.clear();
  current_=0;
  max_works_ =5;
  for(int i =0;i < max_works_; i++) {
    boost::shared_ptr<MfsPost> mp(new MfsPost);
    if(mp->Start() ==false){
      LOG_IF(WARNING,TEST)<<"<>can not start mfspost"; 
      return false;
    }
    mp_.push_back(mp);
  }
  return true;
}

bool MfsPool::PostFile(const FileData &filedata){

  LOG_IF(WARNING,TEST)<<"<> try to POST mfs pool size"<<mp_.size();
  current_++;
  if(mp_.empty())
    return false;
  if(mp_[current_ % max_works_]->PostFile(filedata))
    return true;
  else
    return false;  
}

void MfsPool::Close(){

  std::vector<boost::shared_ptr<MfsPost> >::iterator it= mp_.begin();
  for(; it!=mp_.end();it++){
    (*it)->Close();
  }
  
  }
}



