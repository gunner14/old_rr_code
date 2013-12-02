/*
 * =====================================================================================
 *
 *       Filename:  file_reader.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月03日 18时09分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <algorithm>
#include <new>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "file_reader.h"

int file_line_reader_t::Create(uint32_t buffer_size,FILE* in_file){
  if(in_file==NULL) {
    printf("NULL file pointer\n");
    return -1;
  }

  file_=in_file;
  offset_=0;

  struct stat st;
  if(fstat(fileno(file_),&st)) {
    printf("fstat failed\n");
    return -1;
  }

  fileno_=fileno(in_file);
  file_size_ = st.st_size;
  if (0 == file_size_){
    printf("bid file is empty, file_size[%lld]\n", file_size_);
    return -1;
  }

  buf_size_=std::min(file_size_,(long long)buffer_size);
  try{
    cur_buf_=new char[buf_size_+1];
  }catch(std::bad_alloc){
    printf("new buffer failed\n");
    return -1;
  }

  block_num_=file_size_/buf_size_;
  pos_=buf_size_;
  cur_buf_[pos_]=split;
  if(file_size_%buf_size_) {
    uint32_t read_byte=(file_size_%buf_size_);
    if(read_byte!=pread(fileno_,cur_buf_+buf_size_-read_byte,read_byte,0)) {
      printf("pread failed\n");
      return -1;
    }
    offset_+=read_byte;
    pos_-=read_byte;
  }
  return 0;
}

file_line_reader_t::~file_line_reader_t(){
  if(NULL!=cur_buf_) {
    delete [] cur_buf_;
  }

}

//返回文件的一行，末尾的回车会被替换成null字符
//超长的行会被截断
//返回行的长度，0表示空行
//返回-1表示结束
//返回-2表示出错
//
int file_line_reader_t::GetLine(char* out_buffer,uint32_t out_buffer_size) {
  const int start=pos_;
  //保证cur_buf_[buf_size_]=split
  while(split!=cur_buf_[pos_]) {
    pos_++;
  }
  //读到回车符
  if(pos_<buf_size_) {
    int ret=pos_-start;
    int len=std::min((int)out_buffer_size-1,ret);
    memcpy(out_buffer,cur_buf_+start,len);
    out_buffer[len]=0;
    pos_++;
    return ret;     
  }
  //没有读到下一个回车符，需要读入新的buffer
  int ret=std::min(out_buffer_size-1,pos_-start);
  memcpy(out_buffer,cur_buf_+start,ret);
  pos_=0;

  if(block_num_==0) {
    out_buffer[ret]=0;
    if (ret > 0) {
      printf("file' last line do not include \\n, string	is [%s]", out_buffer);
    }
    return -1;
  }

  if(buf_size_!=pread(fileno_,cur_buf_,buf_size_,offset_)) {
    printf("pread failed\n");
    return -2;
  }

  block_num_--;  
  offset_+=buf_size_;
  cur_buf_[buf_size_]=split;
  while(split!=cur_buf_[pos_]) {
    pos_++;
  }

  if(pos_==buf_size_) {
    printf("The inner buffer size [%u] is shorter than current line size.\n",buf_size_);
    return -2;
  }

  int len=std::min(pos_,out_buffer_size-1-ret);
  memcpy(out_buffer+ret,cur_buf_,len);
  ret+=len;
  out_buffer[ret]=0;
  pos_++;
  return ret;
}
