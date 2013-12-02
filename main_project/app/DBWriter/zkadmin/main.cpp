/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月01日 17时13分56秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include <iostream>
#include <sstream>
#include "../share/DBShare.cpp"

using namespace std;
using namespace xce::dbmanager;

const string ERROR_PATH = "";
const string ERROR_DATA = "__________________________________________";
const char WHITE_SPACE = ' ';
void printHelpInfo() {
  cout << "> display path, 展示path下面的所有子节点" << endl;
  cout << "> create path, 创建节点" << endl;
  cout << "> delete path, 删除path节点，包括它的所有子孙节点" << endl;
  cout << "> getData path, 展示path对应的数据" << endl;
  cout << "> setData path data, 设置path对应的数据" << endl;
}

string getOneWord(const string& line, size_t& start) {
  ostringstream os;
  while (start < line.length() && line[start] == WHITE_SPACE) {
    start++;
  }
  while (start < line.length() && line[start] != WHITE_SPACE) {
    os << line[start];
    start++;
  }
  return os.str();
}

int main(int argc, char* argv[]) {
  ZkManagerPtr zkManager = new ZkManager();
  zkManager->init();
  while (true) {
    cout << "> 请输入命令，或者exit退出\n";
    string line;
    getline(cin, line);
    if (line == "exit") {
      cout << "> bye!" << endl;
      break;
    }
    string cmd = "", path = ERROR_PATH, data = ERROR_DATA;
    size_t start = 0;
    cmd = getOneWord(line, start);
    if (start < line.length()) {
      path = getOneWord(line, start);
    }
    if (start < line.length()) {
      data = getOneWord(line, start);
    }
    if (path == ERROR_PATH) {
      cout << "> 非法路径，请重试!" << endl;
      continue;
    }
    if (cmd == "display") {
      vector<string> children = zkManager->readChildren(path);
      for (vector<string>::const_iterator it = children.begin(); it != children.end(); ++it) {
        cout << ">    " << *it << endl;
      }
    } else if (cmd == "create") {
      zkManager->createNode(path, ZOO_PERSISTENT);
    } else if (cmd == "delete") {
      zkManager->deleteNode(path);
    } else if (cmd == "getData") {
      string data = zkManager->readData(path);
      cout << "> [" << data << "]" << endl;
    } else if (cmd == "setData" && data != ERROR_DATA) {
    } else {
      cout << "> 错误输入，请重试!" << endl;
      continue;
    }
  }
  return 0;
}
