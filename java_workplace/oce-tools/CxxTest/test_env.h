#include <iostream>
#include <stdlib.h>

using namespace std;

void test_env(){
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> [test_env] >>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	cout << "[test_env] 函数名: getenv" << endl;
	cout << "[test_env] 头文件: <stdlib.h>" << endl;
	cout << "[test_env] 用  法: char *getenv(char *envvar);" << endl;
	cout << "[test_env] 说  明: 用来取得参数enwar环境变量的内容。enwar为环境变量的名称，存在则会返回指向该内容的指针。环境变量的格式为enwar＝value" << endl;
	cout << "[test_env] 返回值: 执行成功则返回指向该内容的指针，找不到符合的环境变量名称则返回NULL" << endl;
        cout << endl;
	cout << "[test_env] testing >>>>>>>>>>" << endl;
//////////////////////
	char *s;
	s=getenv("JAVA_HOME");
	printf( "[test_env] JAVA_HOME = %s\n",s );
	cout << "[test_env] ICE_HOME = " << getenv("ICE_HOME") << endl;
        cout << endl;
//////////////////////
}

