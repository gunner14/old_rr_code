#include "IntSerialize.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
  int testNum;
  cout << "Please input the testNum " << endl;
  cin >> testNum;
  string strNum = xce::journey::util::IntSerialize::serialize(testNum);
  cout << "The IntSerialize::serialize() :" << xce::journey::util::IntSerialize::serialize(testNum) << endl;
  cout << "The IntSerialize::unserialize() :" << xce::journey::util::IntSerialize::unserialize(strNum) << endl; 
  return 0;
}
