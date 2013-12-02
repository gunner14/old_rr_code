/*
  Copyright 2012-08-28 shiqiang.ding@renren-inc.com
                       dinggangyilang@gmail.com
  This file is part of XCS.

  提供ACL结构体
*/

#ifndef XCS_SRC_ZK_ACL_PROVIDER_H_
#define XCS_SRC_ZK_ACL_PROVIDER_H_

#include <vector>
#include <set>
#include <string>

#include "zookeeper.h"

namespace xcs {

struct XoaACL {
  int32_t   perms;
  std::string scheme;
  std::string id;

  bool operator < (const struct XoaACL &res) const {
      if (scheme == res.scheme) {
          if (id == res.id) {
            return perms < res.perms;
          }
          return id < res.id;
      }
      return scheme < res.scheme;
  }
};
class DigestAuthProvider {
 public:
  DigestAuthProvider() {
    acl_set_.clear();
  }
  std::string GenerateDigest(const std::string &usr, const std::string &pwd);
  bool AddDigestAcl(const std::string &usr,
                    const std::string &pwd,
                    const std::string &perms);
  bool RemoveDigestAcl(const std::string &usr, const std::string &pwd);
  void GenerateAclVector(struct ACL_vector *acls);
  void DeleteAclVector(struct ACL_vector &acls);

 private:
  char*   Base64(const unsigned char *input, int length);
  void    Sha1(char const *input, int length, unsigned char *output);
  char*   Digest(char const *input, int length);
  int32_t GeneratePerms(const std::string &perms);
  std::string GetScheme();

  std::set<XoaACL> acl_set_;
};
}

#endif  // XCS_SRC_ZK_ACL_PROVIDER_H_
