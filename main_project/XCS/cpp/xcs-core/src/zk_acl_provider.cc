/*
  Copyright 2012-08-28 shiqiang.ding@renren-inc.com

  提供ACL结构体
*/

#include "zk_acl_provider.h"

#include <string.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#include <vector>

namespace xcs {

std::string DigestAuthProvider::GetScheme() {
  return std::string("digest");
}

char* DigestAuthProvider::Base64(const unsigned char *input, int length) {
  BIO *bmem, *b64;
  BUF_MEM *bptr;

  b64 = BIO_new(BIO_f_base64());
  bmem = BIO_new(BIO_s_mem());
  b64 = BIO_push(b64, bmem);
  BIO_write(b64, input, length);
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bptr);

  char *buff = static_cast<char *>(malloc(bptr->length));
  memcpy(buff, bptr->data, bptr->length-1);
  buff[bptr->length-1] = 0;
  BIO_free_all(b64);
  return buff;
}

void DigestAuthProvider::Sha1(char const *input, int length,
                              unsigned char *output) {
  SHA_CTX   c;
  SHA1_Init(&c);
  SHA1_Update(&c, input, length);
  SHA1_Final(output, &c);
}

char* DigestAuthProvider::Digest(char const *input, int length) {
  unsigned char output[20];
  Sha1(input, length, output);
  return Base64(output, 20);
}

std::string DigestAuthProvider::GenerateDigest(const std::string &usr,
                                               const std::string &pwd) {
  std::string res = "";
  std::string usrPwd = usr + std::string(":") + pwd;
  char* id = Digest(usrPwd.c_str(), usrPwd.size());
  res = usr + std::string(":") + std::string(id);
  free(id);
  return res;
}

int32_t DigestAuthProvider::GeneratePerms(const std::string &perms) {
  int32_t res = 0;
  for (size_t i = 0; i < perms.size(); i++) {
    switch (perms[i]) {
    case 'a':
      res |= ZOO_PERM_ADMIN;
      break;
    case 'r':
      res |= ZOO_PERM_READ;
      break;
    case 'w':
      res |= ZOO_PERM_WRITE;
      break;
    case 'c':
      res |= ZOO_PERM_CREATE;
      break;
    case 'd':
      res |= ZOO_PERM_DELETE;
      break;
    default:
      return -1;
    }
  }
  return res;
}

bool DigestAuthProvider::AddDigestAcl(const std::string &usr,
                                      const std::string &pwd,
                                      const std::string &perms) {
  XoaACL xoa_acl;
  xoa_acl.scheme = GetScheme();
  xoa_acl.perms = GeneratePerms(perms);
  xoa_acl.id = GenerateDigest(usr, pwd);
  if (-1 == xoa_acl.perms)
      return false;
  acl_set_.insert(xoa_acl);
  return true;
}

bool DigestAuthProvider::RemoveDigestAcl(const std::string &usr,
                                         const std::string &pwd) {
  XoaACL xoa_acl;
  xoa_acl.scheme = GetScheme();
  xoa_acl.id = GenerateDigest(usr, pwd);
  acl_set_.erase(xoa_acl);
  return true;
}

void DigestAuthProvider::GenerateAclVector(struct ACL_vector *acls) {
  acls->count = 0;
  acls->data = NULL;
  allocate_ACL_vector(acls, acl_set_.size());
  std::set<XoaACL>::iterator itr = acl_set_.begin();
  for (int32_t i = 0; i < acls->count && itr != acl_set_.end(); i++, itr++) {
    acls->data[i].perms = itr->perms;
    acls->data[i].id.scheme = strdup((itr->scheme).c_str());
    acls->data[i].id.id = strdup((itr->id).c_str());
  }
}
void DigestAuthProvider::DeleteAclVector(struct ACL_vector &acls) {
  deallocate_ACL_vector(&acls);
  acls.count = 0;
}
}
