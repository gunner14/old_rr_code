/*
 * common_struct.h
 *
 *  Created on: May 14, 2012
 *      Author: root
 *      Copyright: renren-inc
 */

#ifndef TOPCC_DISKCACHE_COMMON_STRUCT_H_
#define TOPCC_DISKCACHE_COMMON_STRUCT_H_
#include <stdint.h>
#include <stddef.h>

typedef unsigned int rel_time_t;
typedef uint16_t KeyLengthType;
typedef uint16_t CheckCodeType;
typedef uint32_t ValueLengthType;

enum DataGetStatus {
  kDataGetOK,
  kCheckCodeError,
  kDataGetBufOverflow
};

#define LEVEL_BITS 2
#define FILE_BITS 20
#define BLOCK_BITS 7
#define OFFSET_BITS 24
#define VALID_BITS 1

typedef struct {
  uint32_t level_ :LEVEL_BITS;
  uint32_t file_ :FILE_BITS;
  uint32_t block_ :BLOCK_BITS;
  uint32_t valid_ :VALID_BITS;
} DiskFileBlockAddr;

extern const DiskFileBlockAddr kInvalidDiskFileBlockAddr;

typedef struct {
  const DiskFileBlockAddr* data_;
  uint32_t refcount;
} DiskFileBlockAddrRefPtr;

#pragma pack(4)
typedef struct {
  DiskFileBlockAddrRefPtr* block_addr_;
  uint32_t offset_in_block_;
} DiskDataAddr;
#pragma pack()

typedef struct _DiskLevelDir {
  char dir[256];
  size_t size;
} DiskLevelDir;
typedef struct _DiskLevelConf {
  DiskLevelDir** dirs;
  size_t size;
  size_t capacity;
} DiskLevelConf;

#endif  // TOPCC_DISKCACHE_COMMON_STRUCT_H_
