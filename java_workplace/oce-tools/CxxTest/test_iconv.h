#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iconv.h>
#include <errno.h>

using namespace std;

//代码转换操作类
class CodeConverter
{
	public:
		CodeConverter( const string& source_code, const string& trans_code, const string& target_code );
		CodeConverter( const string& source_code, const string& target_code );
		~CodeConverter();

		string cnvt_direct( const string& strIn );
		string cnvt_indirect( const string& strIn );
	private:
		string _source_code;
		string _trans_code;
		string _target_code;
		bool _legal;
		enum ErrNo{ _INIT, _NOERR, _E2BIG, _EILSEQ, _EINVAL, _UNKNOWN };
		string docnvt( const iconv_t& conv, const string& str, char** itIn, size_t& lenIn, ErrNo& errNo );
};

CodeConverter::CodeConverter( const string& source_code, const string& trans_code, const string& target_code ) :
	_source_code( source_code ),
	_trans_code( trans_code ),
	_target_code( target_code ){
		if( (_source_code==_target_code)
				|| (_source_code==_trans_code)
				|| (_trans_code==_target_code) ){
			_legal = false;
		} else{
			_legal = true;
		}
}

CodeConverter::~CodeConverter(){}

CodeConverter::CodeConverter( const string& source_code, const string& target_code ) :
	_source_code( source_code ),
	_trans_code( "" ),
	_target_code( target_code ){
		if( (_source_code==_target_code)
				|| (_source_code==_trans_code)
				|| (_trans_code==_target_code) ){
			_legal = false;
		} else{
			_legal = true;
		}
}

string CodeConverter::cnvt_direct( const string& strIn ){
	iconv_t _conv_s_t;
	if( _legal ){
		_conv_s_t = iconv_open( _target_code.c_str(), _source_code.c_str() );
	}else{
		return strIn;
	}
	iconv( _conv_s_t, NULL, NULL, NULL, NULL );
	string res;
	size_t lenIn = strIn.length();
	char *itIn = const_cast<char*>( strIn.c_str() );
	while( (lenIn>0) && (itIn<strIn.c_str()+strIn.length()) ){
		ErrNo errNo = _INIT;
		string strOut = docnvt( _conv_s_t, strIn, &itIn, lenIn, errNo );
		res += strOut;
		if( errNo==_EILSEQ || errNo==_EINVAL || errNo==_UNKNOWN ){
			res += itIn[0];
			++itIn;
			--lenIn;
		}
	}
	iconv_close( _conv_s_t );
	return res;
}

string CodeConverter::cnvt_indirect( const string& strIn ){ 
	iconv_t _conv_s_t;
	iconv_t _conv_s_m;
	iconv_t _conv_m_t;
	if( _legal ){
		_conv_s_t = iconv_open( _target_code.c_str(), _source_code.c_str() );
		_conv_s_m = iconv_open( _trans_code.c_str(), _source_code.c_str() );
		_conv_m_t = iconv_open( _target_code.c_str(), _trans_code.c_str() );
	}else{
		return strIn;
	}
	iconv( _conv_s_t, NULL, NULL, NULL, NULL );
	iconv( _conv_s_m, NULL, NULL, NULL, NULL );
	iconv( _conv_m_t, NULL, NULL, NULL, NULL );
	string res;
	size_t lenIn = strIn.length();
	char *itIn = const_cast<char*>( strIn.c_str() );
	while( (lenIn>0) && (itIn<strIn.c_str()+strIn.length()) ){
		ErrNo errNo = _INIT;
		string strOut = docnvt( _conv_s_t, strIn, &itIn, lenIn, errNo );
		res += strOut;
		if( errNo!=_EILSEQ && errNo!=_EINVAL && errNo!=_UNKNOWN ){
			continue;
		}

		errNo = _INIT;
		string strTmp = docnvt( _conv_s_m, strIn, &itIn, lenIn, errNo );
		if( strTmp.size() != 0 ){
			char *itTmp = const_cast<char*>( strTmp.c_str() );
			errNo = _INIT;
			size_t lenTmp = strTmp.length();
			strOut = docnvt( _conv_m_t, strTmp, &itTmp, lenTmp, errNo);
			res += strOut;
		} else if( errNo==_EILSEQ || errNo==_EINVAL || errNo==_UNKNOWN ){
			res += itIn[0];
			++itIn;
			--lenIn;
		}
	}
	iconv_close( _conv_s_t );
	iconv_close( _conv_s_m );
	iconv_close( _conv_m_t );
	return res;
}

string CodeConverter::docnvt( const iconv_t& conv, const string& str, char** itIn, size_t& lenIn, ErrNo& errNo ){
	char buffer[ str.length()*2+10 ];
	size_t lenOut = sizeof(buffer) - 5;
	char *itOut = buffer;
	string res;
	int convres = iconv( conv, itIn, &lenIn, &itOut, &lenOut );
	if( itOut != buffer ){
		itOut[0] = '\0';
		res += buffer;
	}
	if( convres == 0){
		errNo = _NOERR;
	} else if( errno == E2BIG){
		lenOut = sizeof(buffer) - 1;
		lenIn -= strlen(buffer);
		errNo = _E2BIG;
	} else if( errno == EILSEQ){
		errNo = _EILSEQ;
	} else if( errno == EINVAL){
		errNo = _EINVAL;
	} else {
		errNo = _UNKNOWN;
	}
	return res;
}

string cnvt_simple( const string& strIn, const string& _source_code, const string& _target_code ){ 
    //LOG_INFO(cnvtrlogger,"strIn = "<<strIn);
    if (_target_code == _source_code) {
        return strIn;
    }
    iconv_t conv = iconv_open(_target_code.c_str(), _source_code.c_str());
    if (conv == (iconv_t)-1) {
        //throw Ice::SyscallException(__FILE__, __LINE__);
        return "";
    }
    iconv(conv, NULL, NULL, NULL, NULL);

    size_t lengthIn = strIn.length();
    char *sin = const_cast<char*>(strIn.c_str());
    char bufferOut[strIn.length()*2+10];
    char *sout = bufferOut;
    size_t lengthOut = sizeof(bufferOut) - 5;

    string res;
    while ((lengthIn > 0) && (sin <= strIn.c_str()+strIn.length())) {
        if (iconv(conv, &sin, &lengthIn, &sout, &lengthOut) == 0) {
            // ok, iconv success
            sout[0] = '\0';
            res += bufferOut;
            break;
            //return res.str();
        } else if (errno == E2BIG) {
            // the input string from's length is greater than bufferOut's,
            // so, store the res converted and convert the bytes left again.
            sout[0] = '\0';
            res += bufferOut;
            sout = bufferOut;
            lengthOut = sizeof(bufferOut) - 1;
            lengthIn -= strlen(bufferOut);
        } else {
            // well, encounter the invalid byte, just copy it and continue.
            sout[0] = '\0';
            res += bufferOut;
            sout = bufferOut;
            ++sin;
            --lengthIn;
        }
    }
    iconv_close(conv);
    return res;
}

string _minWord = string() + char(0xb0) + char(0xa1);
string _maxWord = string() + char(0xf7) + char(0xff);

class KeyCmp {
	public:
		bool operator()(const string& gs1, const string& gs2) const {
			const char* op1 = gs1.c_str();
			const char* op2 = gs2.c_str();
			return strcmp(op1,op2)<0;
		}
};

void test_iconv(){

	vector<string> names_utf8;
	names_utf8.push_back( "汤成信♫ " );
	names_utf8.push_back( "啊" );
	names_utf8.push_back( "阿" );
	names_utf8.push_back( "鼾" );
	names_utf8.push_back( "齄" );
	names_utf8.push_back( "葉湘" );
	names_utf8.push_back( "叶湘" );
	names_utf8.push_back( "张文圣³" );
	names_utf8.push_back( ".罗章铭" );
	names_utf8.push_back( "Bonny赵宇光" );
	names_utf8.push_back( "toxicaria" );
	names_utf8.push_back( "白伯纯" );
	names_utf8.push_back( "陈恶" );
	names_utf8.push_back( "陳惡" );
	names_utf8.push_back( "陈惡" );
	names_utf8.push_back( "開心" );
	names_utf8.push_back( "开心" );
	names_utf8.push_back( "赵風笔" );
	names_utf8.push_back( "风" );
	names_utf8.push_back( "姚诗❤❷❽ 洋" );
	names_utf8.push_back( "赵風笔惡" );
	names_utf8.push_back( "葉" );
	names_utf8.push_back( "叶" );
	names_utf8.push_back( "筆" );
	names_utf8.push_back( "開心" );
	names_utf8.push_back( "风筆" );

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> [test_iconv] >>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	cout << "[test_iconv] 函数名: iconv_open" << endl;
	cout << "[test_iconv] 头文件: <iconv.h>" << endl;
	cout << "[test_iconv] 用  法: iconv_t iconv_open(const char *tocode, const char *fromcode)" << endl;
	cout << "[test_iconv] 说  明: 此函数说明将要进行哪两种编码的转换,tocode是目标编码,fromcode是原编码" << endl;
	cout << "[test_iconv] 返回值: 返回一个转换句柄,供以下两个函数使用" << endl;
	cout << endl;
	cout << "[test_iconv] 函数名: iconv" << endl;
	cout << "[test_iconv] 头文件: <iconv.h>" << endl;
	cout << "[test_iconv] 用  法: size_t iconv(iconv_t cd,char **inbuf,size_t *inbytesleft,char **outbuf,size_t *outbytesleft)" << endl;
	cout << "[test_iconv] 说  明: iconv逐步扫描inbuf，每转换一个字符，就增加inbuf，减少inbytesleft，并将结果存入outbuf，结果字节数存入outbytesleft" << endl;
	cout << "[test_iconv]         遇到下列情况将停止扫描并返回：" << endl;
	cout << "[test_iconv]         	1. 多字节序列无效，这时候errno为EILSEQ，*inbuf指向第一个无效的字符；" << endl;
	cout << "[test_iconv]         	2. 有字节留在inbuf尚未转换，errno为EINVAL;" << endl;
	cout << "[test_iconv]         	3. outbuf空间不够，errno为E2BIG；" << endl;
	cout << "[test_iconv]         	4. 正常转换完备。" << endl;
	cout << "[test_iconv]         还有两种调用情况：" << endl;
	cout << "[test_iconv]         	1. inbuf或者*inbuf为NULL，outbuf和*outbuf不为NULL，iconv会设置转换状态为初始状态，并保存转换序列到*outbuf。如果outbuf空间不足，errno会设置为E2BIG，返回(size_t)(-1)；" << endl;
	cout << "[test_iconv]         	2. inbuf或者*inbuf为NULL，outbuf或者*outbuf也为NULL，iconv设置转换状态为初始状态。" << endl;
	cout << "[test_iconv] 返回值: 如果调用成功，返回转换的字节数（不可逆转调用的字节数，可逆转调用的字节数不包括在内）。否则返回-1，并设置相应的errno" << endl;
	cout << endl;
	cout << "[test_iconv] 函数名: iconv_close" << endl;
	cout << "[test_iconv] 头文件: <iconv.h>" << endl;
	cout << "[test_iconv] 用  法: int iconv_close(iconv_t cd)" << endl;
	cout << "[test_iconv] 说  明: 此函数用于关闭转换句柄,释放资源" << endl;
	cout << "[test_iconv] 返回值: 未细查" << endl;
	cout << endl;

	cout << "[test_iconv] testing cnvt_simple >>>>>>>>>>" << endl;
//////////////////////
	string name = "叶湘";
	cout << "[test_iconv] cnvt_simple(\"" << name << "\",\"utf-8\",\"gb2312\") = " << cnvt_simple(name,"utf-8","gb2312") << endl;
	cout << endl;
//////////////////////

	cout << "[test_iconv] testing cnvt_indirect >>>>>>>>>>" << endl;
//////////////////////
	cout << "[test_iconv] before sort ..." << endl;
	cout << endl;
	multimap<string,string,KeyCmp> namemap;
	CodeConverter cc("utf-8","big5","gb2312");
	for(vector<string>::iterator it=names_utf8.begin();it!=names_utf8.end();++it){
		string name_gb2312;
		if( *it != "" ){
			name_gb2312 = cc.cnvt_indirect(*it);
		} else{
			name_gb2312 = _maxWord;
		}
		if( strcmp(const_cast<char*>(name_gb2312.c_str()),const_cast<char*>(_minWord.c_str())) < 0 ){
			name_gb2312 = _maxWord + name_gb2312;
		}
		namemap.insert( multimap<string,string>::value_type(name_gb2312,*it) );
		cout << "[test_iconv] " << *it << "\t(" << name_gb2312 << ")" << endl;
	}
	cout << endl;
	cout << "[test_iconv] after sort ..." << endl;
	cout << endl;
	for(multimap<string,string,KeyCmp>::const_iterator itMap = namemap.begin();itMap!=namemap.end();++itMap){
		cout << "[test_iconv] " << itMap->second << "\t(" << itMap->first << ")" << endl;
		itMap->second;
	}
	cout << endl;
//////////////////////
}

/*
It provides support for the encodings:

European languages
ASCII, ISO-8859-{1,2,3,4,5,7,9,10,13,14,15,16}, KOI8-R, KOI8-U, KOI8-RU,
CP{1250,1251,1252,1253,1254,1257}, CP{850,866}, Mac{Roman,CentralEurope,
Iceland,Croatian,Romania}, Mac{Cyrillic,Ukraine,Greek,Turkish}, Macintosh

Semitic languages
ISO-8859-{6,8}, CP{1255,1256}, CP862, Mac{Hebrew,Arabic}

Japanese
EUC-JP, SHIFT_JIS, CP932, ISO-2022-JP, ISO-2022-JP-2, ISO-2022-JP-1

Chinese
EUC-CN, HZ, GBK, GB18030, EUC-TW, BIG5, CP950, BIG5-HKSCS,
ISO-2022-CN, ISO-2022-CN-EXT

Korean
EUC-KR, CP949, ISO-2022-KR, JOHAB

Armenian
ARMSCII-8

Georgian
Georgian-Academy, Georgian-PS

Tajik
KOI8-T

Thai
TIS-620, CP874, MacThai

Laotian
MuleLao-1, CP1133

Vietnamese
VISCII, TCVN, CP1258

Platform specifics
HP-ROMAN8, NEXTSTEP

Full Unicode
UTF-8
UCS-2, UCS-2BE, UCS-2LE
UCS-4, UCS-4BE, UCS-4LE
UTF-16, UTF-16BE, UTF-16LE
UTF-32, UTF-32BE, UTF-32LE
UTF-7
C99, JAVA

Full Unicode, in terms of uint16_t or uint32_t
(with machine dependent endianness and alignment)
UCS-2-INTERNAL, UCS-4-INTERNAL

Locale dependent, in terms of `char' or `wchar_t'
(with machine dependent endianness and alignment,
and with OS and locale dependent semantics) char, wchar_t
The empty encoding name "" is equivalent to "char":
it denotes the locale dependent character encoding.

When configured with the option --enable-extra-encodings,
it also provides support for a few extra encodings:

European languages
CP{437,737,775,852,853,855,857,858,860,861,863,865,869,1125}

Semitic languages
CP864

Japanese
EUC-JISX0213, Shift_JISX0213, ISO-2022-JP-3

Turkmen
TDS565

Platform specifics
RISCOS-LATIN1
*/

