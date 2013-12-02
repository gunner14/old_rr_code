#include "../include/user_name_client.h"
#include "../include/user_passport_client.h"
#include "../include/buddy_relation_client.h"

#include <iostream>

using namespace std;
using namespace xce::client;
int main () {
  cout << "hello" << endl;
  cout << UserNameClient::GetUserName(35178) << endl;
  cout << UserNameClient::GetUserName(57936201) << endl;
  cout << UserPassportClient::GetUserSafeStatus(35178) << endl;
  cout << UserPassportClient::GetUserStatus(35178) << endl;
  cout << BuddyRelationClient::IsFriend(35178, 57936201) << endl;
  return 0;
}
