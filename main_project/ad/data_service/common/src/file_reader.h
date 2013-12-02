/*
 * =====================================================================================
 *
 *       Filename:  file_reader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012��09��03�� 18ʱ09��53��
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


class file_line_reader_t{

  public:

    file_line_reader_t():cur_buf_(NULL),file_(NULL),pos_(0),file_size_(0),block_num_(0){};

    int Create(uint32_t buffer_size,FILE* in_file);

    ~file_line_reader_t();

    /* 
     *�����ļ���һ�У�ĩβ�Ļس��ᱻ�滻��null�ַ�
     *�������лᱻ�ض�
     *�����еĳ��ȣ�0��ʾ����
     *����-1��ʾ����
     *����-2��ʾ����
     */

    int GetLine(char* out_buffer,uint32_t out_buffer_size);

  private:

    char *cur_buf_;
    uint32_t buf_size_;
    FILE* file_;
    uint32_t pos_;
    int fileno_;
    static const char split='\n';
    long long file_size_,block_num_,offset_;

};
