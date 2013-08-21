/* builder.h */

/*    Copyright 2009 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef BSON_BUILDER_H__
#define BSON_BUILDER_H__

#include <stdio.h>
#include <string.h>
#include <string>

#include "base/string_piece.h"
#include "base/logging.h"

namespace bson {

class StringBuilder;

class BufBuilder {
public:
  BufBuilder(int initsize = 512) : size_(initsize), data_(0) {
    if ( size_ > 0 ) {
      data_ = (char *) malloc(size_);
    }
    used_ = 0;
  }
  ~BufBuilder() {
    kill();
  }

  void kill() {
    if ( data_ ) {
      free(data_);
      data_ = 0;
    }
  }

  void reset( int maxSize = 0 ){
    used_ = 0;
    if ( maxSize && size_ > maxSize ){
      free(data_);
      data_ = (char*)malloc(maxSize);
      size_ = maxSize;
    }            
  }

  /* leave room for some stuff later */
  char* skip(int n) { return grow(n); }

  /* note this may be deallocated (realloced) if you keep writing. */
  char* buf() { return data_; }
  const char* buf() const { return data_; }

  /* assume ownership of the buffer - you must then free() it */
  void decouple() { data_ = 0; }

  void appendChar(char j){
    *((char*)grow(sizeof(char))) = j;
  }
  void appendNum(char j){
    *((char*)grow(sizeof(char))) = j;
  }
  void appendNum(short j) {
    *((short*)grow(sizeof(short))) = j;
  }
  void appendNum(int j) {
    *((int*)grow(sizeof(int))) = j;
  }
  void appendNum(unsigned j) {
    *((unsigned*)grow(sizeof(unsigned))) = j;
  }
  void appendNum(bool j) {
    *((bool*)grow(sizeof(bool))) = j;
  }
  void appendNum(double j) {
    *((double*)grow(sizeof(double))) = j;
  }
  void appendNum(long long j) {
    *((long long*)grow(sizeof(long long))) = j;
  }
  void appendNum(unsigned long long j) {
    *((unsigned long long*)grow(sizeof(unsigned long long))) = j;
  }

  void appendBuf(const void *src, size_t len) {
    memcpy(grow((int) len), src, len);
  }

  void appendStr(const arch::StringPiece &str , bool includeEOO = true ) {
    const int len = str.size() + ( includeEOO ? 1 : 0 );
    memcpy(grow(len), str.data(), len);
  }

  int len() const {
    return used_;
  }

  void setlen( int newLen ){
    used_ = newLen;
  }

  /* returns the pre-grow write position */
  inline char* grow(int by) {
    int oldlen = used_;
    used_ += by;
    if ( used_ > size_ ) {
      grow_reallocate();
    }
    return data_ + oldlen;
  }

  int getSize() const { return size_; }

private:
  /* "slow" portion of 'grow()'  */
  void grow_reallocate(){
    int a = size_ * 2;
    if ( a == 0 )
      a = 512;
    if ( used_ > a )
      a = used_ + 16 * 1024;

    LOG_ASSERT(a < 64 * 1024 * 1024);

    data_ = (char *) realloc(data_, a);
    size_= a;
  }

  char *data_;
  int used_;
  int size_;

  friend class StringBuilder;
};

#if defined(_WIN32)
#pragma warning( disable : 4996 )
#endif

class StringBuilder {
public:
  StringBuilder( int initsize = 256 )
    : buf_( initsize ){
  }
  // ::mongo::assert( z >= 0 );
#define SBNUM(val, maxSize, macro) \
  int prev = buf_.used_; \
  int z = sprintf( buf_.grow(maxSize) , macro , (val) );  \
  buf_.used_ = prev + z; \
  return *this; 

  StringBuilder& operator<<( double x ){
    SBNUM( x , 25 , "%g" );
  }
  StringBuilder& operator<<( int x ){
    SBNUM( x , 11 , "%d" );
  }
  StringBuilder& operator<<( unsigned x ){
    SBNUM( x , 11 , "%u" );
  }
  StringBuilder& operator<<( long x ){
    SBNUM( x , 22 , "%ld" );
  }
  StringBuilder& operator<<( unsigned long x ){
    SBNUM( x , 22 , "%lu" );
  }
  StringBuilder& operator<<( long long x ){
    SBNUM( x , 22 , "%lld" );
  }
  StringBuilder& operator<<( unsigned long long x ){
    SBNUM( x , 22 , "%llu" );
  }
  StringBuilder& operator<<( short x ){
    SBNUM( x , 8 , "%hd" );
  }
  StringBuilder& operator<<( char c ){
    buf_.grow( 1 )[0] = c;
    return *this;
  }
#undef SBNUM

  void appendDoubleNice( double x ){
    int prev = buf_.used_;
    char * start = buf_.grow( 32 );
    int z = sprintf( start , "%.16g" , x );
    // assert( z >= 0 );
    buf_.used_ = prev + z;
    if( strchr(start, '.') == 0 && strchr(start, 'E') == 0 && strchr(start, 'N') == 0 ){
      write( ".0" , 2 );
    }
  }

  void write( const char* buf, int len){
    memcpy( buf_.grow( len ) , buf , len );
  }

  void append( const arch::StringPiece& str ){
    memcpy( buf_.grow( str.size() ) , str.data() , str.size() );
  }
  StringBuilder& operator<<( const arch::StringPiece& str ){
    append( str );
    return *this;
  }

  // access

  void reset( int maxSize = 0 ){
    buf_.reset( maxSize );
  }

  std::string str() const {
    return std::string(buf_.data_, buf_.used_);
  }

private:
  BufBuilder buf_;
};

} // namespace bson

#endif