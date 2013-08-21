#include <iostream>
#include <stdio.h>

using namespace std;

void test_alarm(const string&);

void test_system(){
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> [test_system] >>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	cout << "[test_system] 函数名: system" << endl;
	cout << "[test_system] 头文件: <stdio.h>" << endl;
	cout << "[test_system] 用  法: int system(const char * string);" << endl;
	cout << "[test_system] 说  明: system()会调用fork()产生子进程，由子进程来调用/bin/sh-c string来执行参数string字符串所代表的命令，此命令执行完后随即返回原调用的进程。在调用system()期间SIGCHLD 信号会被暂时搁置，SIGINT和SIGQUIT 信号则会被忽略。" << endl;
	cout << "[test_system] 返回值: 如果system()在调用/bin/sh时失败则返回127，其他失败原因返回-1。若参数string为空指针(NULL)，则返回非零值。如果 system()调用成功则最后会返回执行shell命令后的返回值，但是此返回值也有可能为system()调用/bin/sh失败所返回的127，因此最好能再检查errno 来确认执行成功。" << endl;
        cout << endl;
	cout << "[test_system] test_alarm >>>>>>>>>>" << endl;

//////////////////////
	vector<string> numbers;
	numbers.push_back("15801208687");//me
	/*
	numbers.push_back("13810091260");//han.zhang
	numbers.push_back("15210236023");//zhangnan
	numbers.push_back("13488813146");//ning.zhao
	numbers.push_back("13910672125");//yang.yu
	numbers.push_back("15929907650");//tbk
	numbers.push_back("13511015744");//tiean.zhang
	numbers.push_back("13811502518");//yuanhua.li
	numbers.push_back("13264571442");//yanan.gao
	*/

	for( vector<string>::iterator it = numbers.begin(); it != numbers.end(); ++it ){
		ostringstream alarm;
		alarm << "wget -q -O /dev/null \"http://10.22.198.81:2000/receiver?number=";
		alarm << *it;
		alarm << "&message=";
		alarm << "c0.xce overtime";
		alarm << "\"";
		test_alarm( alarm.str() );
	}

	for( vector<string>::iterator it = numbers.begin(); it != numbers.end(); ++it ){
		ostringstream alarm;
		alarm << "wget -q -O /dev/null \"http://10.22.198.81:2000/receiver?number=";
		alarm << *it;
		alarm << "&message=";
		alarm << "trust me ... c0.xce is really overtime";
		alarm << "\"";
		test_alarm( alarm.str() );
	}

//////////////////////

}

void test_alarm(const string& command ){
	cout << "[test_system] system(\"" << command << "\"" << endl;
	system( command.c_str() );
}

