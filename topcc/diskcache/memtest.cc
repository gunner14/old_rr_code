/*
 * memtest.cc
 *
 *  Created on: May 13, 2012
 *      Author: root
 *      Copyright: renren-inc
 */
#include <libmemcached/memcached.h>
#include <stdio.h>
#include <openssl/md5.h>
#include <assert.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <vector>

memcached_st *g_client;
memcached_return g_rc;
const size_t g_totaldisksize = 130 * 1024 * 1024;  // in kB

void GenRandStr(char buf[], int len_str);
std::string StrMd5(const char* str, size_t len);
void Wait(int msec);

struct ValueLenRange {
  unsigned min;
  unsigned max;
};
const ValueLenRange KValueLenRange = { 88 * 1024, 100 * 1024 };

enum TestType {
  BASIC,
  GET_SET,
  GET_FIX,
  SET_FIX,
  GET,
  SET,
  SET_RANGE
};

unsigned RandLen() {
  return rand_r(NULL) % (KValueLenRange.max - KValueLenRange.min)
      + KValueLenRange.min;
}

/**
 * Basic test: Set and then get
 */
void TestBasic() {
  printf("Run Basic Test\n");
  char buf[KValueLenRange.max];
  const unsigned iter_times = 1024 * 1024 * 100;
  uint32_t flags = 0;
  std::string md5;
  char* ret_value;
  size_t value_len;
  size_t ret_value_len;
  std::vector<std::string> keys(iter_times);
  size_t k;
  int retry;
  time_t expiration = 14 * 24 * 60 * 60;
  for (unsigned i = 0; i < iter_times; ++i) {
    value_len = RandLen();
    assert(value_len >= KValueLenRange.min);
    GenRandStr(buf, value_len);
    md5 = StrMd5(buf, value_len);
    md5.resize(md5.size() / 2);
    keys[i] = md5;

    retry = 0;
    do {
      g_rc = memcached_set(g_client, md5.c_str(), md5.size(), buf, value_len,
                           expiration, flags);
      if (g_rc != MEMCACHED_SUCCESS) {
        printf("Set_Failed: %s, errorcode: %d, retry: %d\n", md5.c_str(), g_rc,
               retry);
        Wait(400);
      } else {
        printf("Set_Success: %s\n", md5.c_str());
        break;
      }
    } while (++retry < 3);
    if (retry == 3) {
      printf("Set_Retry_Failed: %s\n", md5.c_str());
      continue;
    }
    printf("Set Success: %s\n", md5.c_str());
    // get
    k = rand_r(NULL) % (i + 1);
    // k = i;
    ret_value = memcached_get(g_client, keys[k].c_str(), keys[k].size(),
                              &ret_value_len, &flags, &g_rc);
    if (g_rc == MEMCACHED_NOTFOUND) {
      printf("NotFound: key index: %u\n", k);
      continue;
    }
    if (g_rc == MEMCACHED_SUCCESS) {
      if (ret_value_len < KValueLenRange.min) {
        printf("Error: Get but value is error, index: %d\n", k);
        free(ret_value);
        continue;
      }
      md5 = StrMd5(ret_value, ret_value_len);
      md5.resize(md5.size() / 2);
      if (strcmp(keys[k].c_str(), md5.c_str())) {
        printf("Error: Check md5 failed: index: %d, key: %s\n", k,
               keys[k].c_str());
      } else {
        printf("Get Success: key: %s\n", keys[k].c_str());
      }
      free(ret_value);
      continue;
    } else {
      printf("Get Error: rc: %d, index: %d, key: %s\n", g_rc, k,
             keys[k].c_str());
    }
  }
  printf("Run Basic Test Done\n");
}

/**
 * Get Test
 */
void TestGet() {
  printf("Run Get Test\n");
  srand((unsigned int) time(NULL));
  char* src, *dest;
  char* newline;
  int filedes;
  struct stat info;
  uint32_t flags = 0;
  size_t ret_value_len;
  unsigned retry;
  size_t key_len;
  char key_buf[64];
  const size_t lower = 0;
  const double hit_ratio = 0.2;
  const size_t upper = (size_t) (g_totaldisksize / hit_ratio / 94.5);
  memcached_return rc;
  for (size_t i = 0; i < size_t(-1); ++i) {
    size_t index = rand_r(NULL) % (upper - lower) + lower;
    key_len = snprintf(key_buf, sizeof(key_buf), "%lu", index);
    dest = memcached_get(g_client, key_buf, key_len, &ret_value_len, &flags,
                         &rc);
    if (!dest) {
      if (rc == MEMCACHED_NOTFOUND) {
        printf("notfound\n");
        continue;
      }
      if (rc != MEMCACHED_SUCCESS) {
        printf("Get_Failed: rc: %d, %s\n, ", rc, key_buf);
        Wait(40);
      } else {
        // Wait(10);
      }
      continue;
    }
    if (ret_value_len < KValueLenRange.min) {
      free(dest);
      printf("Get_Failed: %s, value_len: %d\n", key_buf, ret_value_len);
      Wait(40);
      continue;
    }
    if (memcmp(dest + ret_value_len - key_len, key_buf, key_len)) {
      printf("Get_Failed: %s, check_value_failed\n", key_buf);
    } else {
      printf("Get ok\n");
    }
    free(dest);
  }

  printf("Run Get Test Done\n");
}
/**
 * Set Test
 */
void TestSet() {
  printf("Run Set Test\n");
  uint32_t flags = 3721;
  char* ret_value;
  size_t key_len;
  size_t ret_value_len;
  std::string value_buf(KValueLenRange.max, 'a');
  time_t expiration = 7 * 24 * 3600;
  memcached_return rc;
  char key_buf[64];
  const size_t total_items = static_cast<size_t>(g_totaldisksize / 94.5);
  bool slowdown = false;
  for (size_t i = 0; i < size_t(-1); ++i) {
    if (i > 0 && i % total_items == 0) {
      i = 0;
      slowdown = true;
      printf("slow down\n");
    }
    if (i % 10000 == 0) {
      printf("current: %d\n", i);
    }
    unsigned value_len = RandLen();
    key_len = snprintf(key_buf, sizeof(key_buf), "%lu", i);
    memmove(const_cast<char*>(value_buf.c_str()) + value_len - key_len, key_buf,
            key_len);
    rc = memcached_set(g_client, key_buf, key_len, value_buf.c_str(), value_len,
                       expiration, flags);
    if (rc != MEMCACHED_SUCCESS) {
      Wait(100);
      printf("Set_Failed: %s\n", key_buf);
    } else if (slowdown) {
      Wait(10);
    }
  }
  printf("Run Set Test Done\n");
}
/**
 * SetRange Test
 */
void TestSetRange(size_t low, size_t high) {
  printf("Run SetRange Test\n");
  uint32_t flags = 3721;
  char* ret_value;
  size_t key_len;
  size_t ret_value_len;
  std::string value_buf(KValueLenRange.max, 'a');
  time_t expiration = 7 * 24 * 3600;
  memcached_return rc;
  char key_buf[64];
  for (size_t i = low;; ++i) {
    if (i > 0 && i % high == 0) {
      i = low;
      printf("rewind\n");
    }
    if (i % 10000 == 0) {
      printf("current: %d\n", i);
    }
    unsigned value_len = RandLen();
    key_len = snprintf(key_buf, sizeof(key_buf), "%lu", i);
    memmove(const_cast<char*>(value_buf.c_str()) + value_len - key_len, key_buf,
            key_len);
    rc = memcached_set(g_client, key_buf, key_len, value_buf.c_str(), value_len,
                       expiration, flags);
    if (rc != MEMCACHED_SUCCESS) {
      Wait(100);
      printf("Set_Failed: %s\n", key_buf);
    }
  }
  printf("Run Set Test Done\n");
}
/**
 * Try to get, if not found then set
 */
void TestGetSet(FILE* fp) {
  printf("Run GetSet Test\n");
  size_t len = 512;
  char* path = static_cast<char*>(malloc(len));
  ssize_t path_len;
  char* src, *dest;
  char* newline;
  int filedes;
  struct stat info;
  uint32_t flags = 3721;
  uint32_t ret_flags = 3721;
  char* ret_value;
  size_t ret_value_len;
  unsigned retry = 0;
  while ((path_len = getline(&path, &len, fp)) != -1) {
    newline = strchr(path, '\n');
    if (newline) {
      path_len = newline - path;
      path[path_len] = '\0';
    }
    dest = memcached_get(g_client, path, path_len, &ret_value_len, &ret_flags,
                         &g_rc);
    if (g_rc == MEMCACHED_FAILURE) {
      printf("Get_Failed: %s\n", path);
      continue;
    }
    filedes = open(path, O_RDONLY);
    fstat(filedes, &info);
    if (info.st_size > 800 * 1024) {
      printf("Skip_File: %s\n", path);
      close(filedes);
      continue;
    }
    src = static_cast<char*>(mmap(NULL, info.st_size, PROT_READ, MAP_SHARED,
                                  filedes, 0));
    if (g_rc == MEMCACHED_SUCCESS) {
      if (ret_value_len != info.st_size) {
        printf("Get_Error: src_len: %d, get_len: %d\n", info.st_size,
               ret_value_len);
      } else if (memcmp(src, dest, ret_value_len) != 0) {
        printf("Get_Error: Content_Error\n");
      } else {
        printf("Get Success\n");
      }
      free(dest);
      munmap(src, info.st_size);
      close(filedes);
      continue;
    }
    // set
    retry = 0;
    do {
      g_rc = memcached_set(g_client, path, path_len, src, info.st_size, 18000,
                           flags);
      if (g_rc != MEMCACHED_SUCCESS) {
        printf("Set_Failed: %s, errorcode: %d, retry: %d\n", path, g_rc, retry);
      } else {
        printf("Set_Success: %s\n", path);
        break;
      }
      Wait(2000);
    } while (++retry < 3);
    munmap(src, info.st_size);
    close(filedes);
  }
  printf("Run GetSet Test Done\n");
}

/**
 * Set fix length item
 */
void TestSetFix(FILE* fp, const unsigned value_len) {
  printf("Run SetFix Test\n");
  size_t len = 512;
  char* path = static_cast<char*>(malloc(len));
  ssize_t path_len;
  char* src, *dest;
  char* newline;
  int filedes;
  struct stat info;
  uint32_t flags = 3721;
  uint32_t ret_flags = 3721;
  char* ret_value;
  size_t ret_value_len;
  unsigned retry = 0;
  std::string value;
  value.resize(value_len);
  time_t expiration = 15 * 24 * 3600;
  pid_t pid = getpid();
  while ((path_len = getline(&path, &len, fp)) != -1) {
    newline = strchr(path, '\n');
    if (newline) {
      path_len = newline - path;
      path[path_len] = '\0';
    }
    // set
    retry = 0;
    do {
      g_rc = memcached_set(g_client, path, path_len, value.c_str(), value_len,
                           expiration, flags);
      if (g_rc != MEMCACHED_SUCCESS) {
        printf("pid: %d, Set_Failed: %s, errorcode: %d, retry: %d\n", pid, path,
               g_rc, retry);
      } else {
        printf("Set_Success: %s\n", path);
        break;
      }
      Wait(400);
    } while (++retry < 3);
  }
  printf("Run SetFixTest Done\n");
}

/**
 * GetFix Test
 */
void TestGetFix(FILE* fp, const unsigned value_len) {
  printf("Run GetFix Test\n");
  size_t len = 512;
  char* path = static_cast<char*>(malloc(len));
  ssize_t path_len;
  char* src, *dest;
  char* newline;
  int filedes;
  struct stat info;
  uint32_t flags = 0;
  size_t ret_value_len;
  unsigned retry;
  while ((path_len = getline(&path, &len, fp)) != -1) {
    retry = 0;
    newline = strchr(path, '\n');
    if (newline) {
      path_len = newline - path;
      path[path_len] = '\0';
    }
    retry = 0;
    do {
      dest = memcached_get(g_client, path, path_len, &ret_value_len, &flags,
                           &g_rc);
      if (g_rc == MEMCACHED_NOTFOUND) {
        printf("Get NotFound: %s\n", path);
        break;
      }
      if (g_rc != MEMCACHED_SUCCESS) {
        printf("Get_Failed: %s\n", path);
      } else {
        if (ret_value_len != value_len) {
          printf("Get_Failed: value_len: %d, path: %s\n", ret_value_len, path);
        } else {
          printf("Get_Success: %s\n", path);
          break;
        }
      }
      Wait(400);
    } while (++retry < 3);
    free(dest);
  }

  printf("Run GetFixTest Done\n");
}
void InitCache(const char* host, int port) {
  g_client = memcached_create(NULL);
  g_rc = memcached_server_add(g_client, host, port);
  memcached_behavior_set(g_client, MEMCACHED_BEHAVIOR_DISTRIBUTION,
                         MEMCACHED_DISTRIBUTION_CONSISTENT);  // consistent hash
  memcached_behavior_set(g_client, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);

  // in seconds
  memcached_behavior_set(g_client, MEMCACHED_BEHAVIOR_DEAD_TIMEOUT, 30);
  memcached_behavior_set(g_client, MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, 1);
  // NOTE: do not use MEMCACHED_BEHAVIOR_SERVER_FAILURE_LIMIT
  memcached_behavior_set(g_client, MEMCACHED_BEHAVIOR_REMOVE_FAILED_SERVERS, 3);
}

int main(int argc, char** argv) {
  int port = 11211;
  char c;
  TestType type = GET_SET;
  int temp;
  FILE* fp = NULL;
  int value_len = 4 * 1024;
  std::string host = "127.0.0.1";
  size_t low = 0, high = 0;
  while (-1 != (c = getopt(argc, argv, "h:"
                           "p:"
                           "t:"
                           "f:"
                           "s:"
                           "r:"))) {
    switch (c) {
      case 'h':
        host.assign((const char*) optarg);
        break;
      case 'p':
        port = atol(optarg);
        break;
      case 't':
        temp = atol(optarg);
        type = (TestType) temp;
        break;
      case 'f':
        fp = fopen(optarg, "r");
        break;
      case 's':
        value_len = atol(optarg);
        break;
      case 'r':
        sscanf((const char*) optarg, "%lu:%lu", &low, &high);
        printf("low: %lu, high: %lu\n", low, high);
        break;
      default:
        break;
    }
  }

  InitCache(host.c_str(), port);

  if (type == BASIC) {
    TestBasic();
  } else if (type == GET_SET) {
    TestGetSet(fp);
  } else if (type == GET_FIX) {
    TestGetFix(fp, value_len);
  } else if (type == SET_FIX) {
    TestSetFix(fp, value_len);
  } else if (type == GET) {
    TestGet();
  } else if (type == SET) {
    TestSet();
  } else if (type == SET_RANGE) {
    TestSetRange(low, high);
  }
  return 0;
}

char Dec2HexChar(int16_t n) {
  if (0 <= n && n <= 9) {
    return static_cast<char>(int16_t('0') + n);
  } else if (10 <= n && n <= 15) {
    return static_cast<char>(int16_t('a') + n - 10);
  } else {
    return static_cast<char>(0);
  }
}

std::string Str2Hex(unsigned char *input, int len) {
  std::string result;
  for (int i = 0; i < len; i++) {
    char c = input[i];
    int16_t a = c & 0xF0;
    int16_t b = c & 0x0F;
    a = a >> 4;
    result.push_back(Dec2HexChar(a));
    result.push_back(Dec2HexChar(b));
  }
  return result;
}

std::string StrMd5(const char* str, size_t len) {
  unsigned char *md5 = MD5((const unsigned char*) str, len, NULL);
  return Str2Hex(md5, MD5_DIGEST_LENGTH);
}

static std::string gen_alnums() {
  std::string ret;
  ret.reserve(26 + 10);
  for (int i = 'a'; i <= 'z'; ++i) {
    ret.push_back(static_cast<char>(i));
  }
  for (int i = '0'; i <= '9'; ++i) {
    ret.push_back(static_cast<char>(i));
  }
  return ret;
}
static const std::string alnums = gen_alnums();

void GenRandStr(char buf[], int len_str) {
  static int num_alnums = alnums.size();
  for (int i = 0; i < len_str; ++i) {
    buf[i] = alnums[rand_r(NULL) % num_alnums];
  }
}

void Wait(int msec) {
  struct timeval timeout = { 0, 0 };
  timeout.tv_usec = msec * 1000;
  select(0, NULL, NULL, NULL, &timeout);
}
