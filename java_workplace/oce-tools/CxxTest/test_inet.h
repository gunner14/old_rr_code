#include <netinet/in.h>
#include <arpa/inet.h>
#include <boost/lexical_cast.hpp>
#include <string.h>

using namespace std;

unsigned int s2i_inet_addr(const string&);
string i2s_inet_ntoa(unsigned int);

unsigned int s2i_forward(const string&);
string i2s_forward(unsigned int);

unsigned int s2i_backward(const string&);
string i2s_backward(unsigned int);

void test_inet(){
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> [test_inet] >>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	cout << "[test_inet] 函数名: inet_addr, inet_aton, inet_ntoa" << endl;
	cout << "[test_inet] 头文件: <netinet/in.h> <arpa/inet.h>" << endl;
	cout << "[test_inet] 用  法: unsigned long int inet_addr(const char *cp);" << endl;
	cout << "[test_inet]         int inet_aton(const char *cp, struct in_addr *inp);" << endl;
	cout << "[test_inet]         char * inet_ntoa(struct in_addr in);" << endl;
	cout << "[test_inet] 说  明: 用来将参数cp所指的网络地址字符串转成网络所使用的二进制的数字。网络地址字符串是以数字和点组成的字符串，如192.168.0.236" << endl;
	cout << "[test_inet] 返回值: 成功则返回非0值，失败返回-1" << endl;
        cout << endl;

	//string ip_string = "192.168.0.1";					//string
	string ip_string = "202.105.33.192";					//string
	cout << "[test_inet] string:\t\t" << ip_string << "\tsize=" << ip_string.size() << endl;

	cout << endl;

	unsigned int res_s2i_inet_addr = s2i_inet_addr( ip_string );
	string res_i2s_inet_ntoa = i2s_inet_ntoa( res_s2i_inet_addr);

	unsigned int res_s2i_forward = s2i_forward( ip_string );
	string res_i2s_forward = i2s_forward( res_s2i_forward );

	unsigned int res_s2i_backward = s2i_backward( ip_string );
	string res_i2s_backward = i2s_backward( res_s2i_backward );
}

unsigned int s2i_inet_addr( const string& ip_string ){
	cout << "[test_inet] s2i_inet(const string&) >>>>>>>>>>>>" << endl;

	const char* ip_char=ip_string.c_str();					//char*
	cout << "[test_inet] char*:\t\t" << ip_char << "\tsize=" << strlen(ip_char) << endl;

	in_addr_t ip_addrt = inet_addr( ip_char );				//in_addr_t

	unsigned int ip_uint = (unsigned int) ip_addrt;				//unsigned int
	cout << "[test_inet] uint:\t\t" << ip_uint << "\tsize=" << sizeof(ip_uint) << endl;

	long ip_long = (long) ip_addrt;						//long
	cout << "[test_inet] long:\t\t" << ip_long << "\tsize=" << sizeof(ip_long) << endl;

	long long ip_long_long = (long long) ip_addrt;				//long
	cout << "[test_inet] longlong:\t\t" << ip_long_long << "\tsize=" << sizeof(ip_long_long) << endl;

	struct in_addr ip_addr;							//in_addr
	memcpy(&ip_addrt, &ip_addr, 4);

	return ip_uint;
}

string i2s_inet_ntoa( unsigned int ip_uint ){
	cout << "[test_inet] i2s_inet_ntoa(unsigned int) >>>>>>>>>>>>" << endl;

	struct in_addr ip_addr;							//in_addr
	memcpy(&ip_addr, &ip_uint, 4);

	char* ip_char = inet_ntoa( ip_addr );					//char*
	cout << "[test_inet] char*:\t\t" << ip_char << "\tsize=" << strlen(ip_char) << endl;

	string ip_string(ip_char);						//string
	cout << "[test_inet] string:\t\t" << ip_string << "\tsize=" << ip_string.size() << endl;

	cout << endl;
	return ip_string;
}

unsigned int s2i_forward( const string& ip_string ){
	cout << "[test_inet] s2i_forward(const string&) >>>>>>>>>>" << endl;

	const char* ip_char=ip_string.c_str();					//char*
	cout << "[test_inet] char*:\t\t" << ip_char << "\tsize=" << strlen(ip_char) << endl;

	int ipSlice[4];								//int[]
	sscanf(ip_char,"%d.%d.%d.%d",&ipSlice[0],&ipSlice[1],&ipSlice[2],&ipSlice[3]);
	cout << "[test_inet] int[]:\t\t" << ipSlice[0] << " " << ipSlice[1] << " " << ipSlice[2] << " " << ipSlice[3] << "\tsize=" << sizeof(ipSlice) << endl;

	//unsigned int
	unsigned int ip_uint = (unsigned int)((((((ipSlice[0]<<8)+ipSlice[1])<<8)+ipSlice[2])<<8)+ipSlice[3]);
	cout << "[test_inet] uint(&):\t\t" << ip_uint << "\tsize=" << sizeof(ip_uint) << endl;

	//long
	long ip_long = ((long)((((((ipSlice[0]<<8)+ipSlice[1])<<8)+ipSlice[2])<<8)+ipSlice[3])) & 0xffffffff;
	cout << "[test_inet] long(&):\t\t" << ip_long << "\tsize=" << sizeof(ip_long) << endl;

	//long long
	long long ip_long_long = ((long long)((((((ipSlice[0]<<8)+ipSlice[1])<<8)+ipSlice[2])<<8)+ipSlice[3])) & 0xffffffff;
	cout << "[test_inet] longlong(&):\t" << ip_long_long << "\tsize=" << sizeof(ip_long_long) << endl;

	ip_long = (long)ip_uint;						//long
	cout << "[test_inet] long(uint):\t\t" << ip_long << "\tsize=" << sizeof(ip_long) << endl;

	ip_long_long = (long long)ip_uint;					//long long
	cout << "[test_inet] longlong(uint):\t" << ip_long_long << "\tsize=" << sizeof(ip_long_long) << endl;

	return ip_uint;
}

string i2s_forward( unsigned int ip_uint ){
	cout << "[test_inet] i2s_forward(unsigned int ip_uint) >>>>>>>>>>" << endl;

	int ipSlice[4];								//int[]
	for(int i=0;i<4;++i){
		ipSlice[i] = (ip_uint>>((3-i)*8)) & 0xff;
	}
	cout << "[test_inet] int[]:\t\t" << ipSlice[0] << " " << ipSlice[1] << " " << ipSlice[2] << " " << ipSlice[3] << "\tsize=" << sizeof(ipSlice) << endl;

	ostringstream ip_oss;							//ostringstream
	ip_oss << ipSlice[0] << "." << ipSlice[1] << "." << ipSlice[2] << "." << ipSlice[3];
	string ip_string = ip_oss.str();					//string
	cout << "[test_inet] string:\t\t" << ip_string << "\tsize=" << ip_string.size() << endl;

	cout << endl;
	return ip_string;
}

unsigned int s2i_backward( const string& ip_string ){
	cout << "[test_inet] s2i_backward(const string&) >>>>>>>>>>" << endl;

	const char* ip_char=ip_string.c_str();					//char*
	cout << "[test_inet] char*:\t\t" << ip_char << "\tsize=" << strlen(ip_char) << endl;

	int ipSlice[4];								//int[]
	sscanf(ip_char,"%d.%d.%d.%d",&ipSlice[3],&ipSlice[2],&ipSlice[1],&ipSlice[0]);
	cout << "[test_inet] int[]:\t\t" << ipSlice[0] << " " << ipSlice[1] << " " << ipSlice[2] << " " << ipSlice[3] << "\tsize=" << sizeof(ipSlice) << endl;

	//unsigned int
	unsigned int ip_uint = (unsigned int)((((((ipSlice[0]<<8)+ipSlice[1])<<8)+ipSlice[2])<<8)+ipSlice[3]);
	cout << "[test_inet] uint(&):\t\t" << ip_uint << "\tsize=" << sizeof(ip_uint) << endl;

	//long
	long ip_long = ((long)((((((ipSlice[0]<<8)+ipSlice[1])<<8)+ipSlice[2])<<8)+ipSlice[3])) & 0xffffffff;
	cout << "[test_inet] long(&):\t\t" << ip_long << "\tsize=" << sizeof(ip_long) << endl;

	//long long
	long long ip_long_long = ((long long)((((((ipSlice[0]<<8)+ipSlice[1])<<8)+ipSlice[2])<<8)+ipSlice[3])) & 0xffffffff;
	cout << "[test_inet] longlong(&):\t" << ip_long_long << "\tsize=" << sizeof(ip_long_long) << endl;

	ip_long = (long)ip_uint;						//long
	cout << "[test_inet] long(uint):\t\t" << ip_long << "\tsize=" << sizeof(ip_long) << endl;

	ip_long_long = (long long)ip_uint;					//long long
	cout << "[test_inet] longlong(uint):\t" << ip_long_long << "\tsize=" << sizeof(ip_long_long) << endl;

	return ip_uint;
}

string i2s_backward( unsigned int ip_uint ){
	cout << "[test_inet] i2s_backward(unsigned int ip_uint) >>>>>>>>>>" << endl;

	int ipSlice[4];								//int[]
	for(int i=0;i<4;++i){
		ipSlice[i] = (ip_uint>>(i*8)) & 0xff;
	}
	cout << "[test_inet] int[]:\t\t" << ipSlice[0] << " " << ipSlice[1] << " " << ipSlice[2] << " " << ipSlice[3] << "\tsize=" << sizeof(ipSlice) << endl;

	ostringstream ip_oss;							//ostringstream
	ip_oss << ipSlice[0] << "." << ipSlice[1] << "." << ipSlice[2] << "." << ipSlice[3];
	string ip_string = ip_oss.str();					//string
	cout << "[test_inet] string:\t\t" << ip_string << "\tsize=" << ip_string.size() << endl;

	cout << endl;
	return ip_string;
}

