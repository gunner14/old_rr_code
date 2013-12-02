#include "base/common.h"
#include "base/md5.h"
#include "base/sha2.h"
#include "base/stringdigest.h"
#include "base/stringencode.h"

namespace xce {

std::string MD5(const std::string& data) {
  MD5Context ctx;
  MD5Init(&ctx);
  MD5Update(&ctx, const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(data.data())), static_cast<unsigned int>(data.size()));
  unsigned char digest[16];
  MD5Final(digest, &ctx);
  std::string hex_digest;
  for (int i=0; i<16; ++i) {
    hex_digest += hex_encode(digest[i] >> 4);
    hex_digest += hex_encode(digest[i] & 0xf);
  }
  return hex_digest;
}

std::string SHA256(const std::string& data) {
  sha256_ctx ctx[1];
  sha256_begin(ctx);
  sha256_hash(const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(data.data())), static_cast<unsigned int>(data.size()), ctx);
  unsigned char digest[32];
  sha256_end(digest, ctx);
  std::string hex_digest;
  for (int i=0; i<32; ++i) {
    hex_digest += hex_encode(digest[i] >> 4);
    hex_digest += hex_encode(digest[i] & 0xf);
  }
  return hex_digest;
}

void MD5(unsigned char* digest, const char* data, size_t length) {
  MD5Context ctx;
  MD5Init(&ctx);

  MD5Update(&ctx, const_cast<unsigned char *>(
    reinterpret_cast<const unsigned char *>(data)
    ), length);
  MD5Final(digest, &ctx);
}

void SHA256(unsigned char* digest, const char* data, size_t length) {
  sha256_ctx ctx[1];
  sha256_begin(ctx);
  sha256_hash(const_cast<unsigned char *>(
        reinterpret_cast<const unsigned char *>(data)), length, ctx);
  sha256_end(digest, ctx);
}

}
