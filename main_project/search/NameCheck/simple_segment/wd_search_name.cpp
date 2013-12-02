//#include "../include/name_entity_prober.h"
//#include "../include/segment_const.h"
//#include "../include/segment_utility.h"
//#include "../index/include/utility.h"
////#include "word_const.h"
////#include "double_hash_dict.h"
////#include "address_entity_prober.h"
////#include "segment.h"
//
//#include <string>
//#include <algorithm>
//#include <iostream>
//#include <fstream>
//
//using namespace std;
//using namespace segment;
///**
//*  init the worker
//* @param in: config file path
//* @return :  if succeed return the pointer else return NULL
//* @note   :  the object allocated should be deleted by user
//*/
//NameEntityProber * _init(const char*file){
//									   //这个_p_name_prober是_init（）函数内的，并不是全局的，所以这里_p_name_prober没有定义
//    NameEntityProber *  _p_name_prober = NULL;
//                                       //_p_name_prober对象初始化
//	_p_name_prober = new NameEntityProber();
//
//	if(  !_p_name_prober->init(file) )   //初始化 NameEntityProber对像，并加载配置文件file
//	{
//	    delete _p_name_prober;
//	    return NULL;
//	}
//	
//	
//	return _p_name_prober;
//}
//
//
//bool loadData(const char* filename, wordAry& ary){						//加载数据文件，并转换为wordAry数据类型
//	string filecontent;
//	if( getFileContent( filename , filecontent ) == -1 ) {
//	cout<<"调用getFileContent出错！\n";
//	return false;						//在utility中定义了，将文件读取到filecontent中去了，其中getFileContent函数在utility。cpp中定义了
//    
//	}
//	vector<string> lines_vec;
//	splitTxt( filecontent , "\n" , lines_vec );					        //splitTxt也是在utility中定义的   最后lines—vec中存放着数据行
//	if( lines_vec.empty() ){
//	cout<<"splitTxt出错\n";
//	return false;
//    }
//		
//		for(int j = 0;j<lines_vec.size() ;j++){
//			if( lines_vec[j].empty() )										//对读入的文件的每一行进行分别处理
//			continue;	
//			int length = lines_vec[j].length();
//            char *tmp = (char *)malloc(length + 1);
//			strncpy(tmp,lines_vec[j].c_str(),length);
//			tmp[length] = '\0';
//			//string  myString = lines_vec[j];
//			//string myString = lines_vec[j];
//			//cout<<"&myString is::"<<myString;
//			/*
//			for(int i=0;i<myString.length();i+=2){ //对每个汉字串分解成单个汉字
//					//cout<<"进入到了类型转换\n";
//					wordItem tmpWordItem ;
//					tmpWordItem.length = 2;
//					tmpWordItem.ptr_wd = &(myString[i]);
//					//tmpWordItem.ptr_wd = strcpy(new char[myString.length+1],&myString.c_str());
//					//tmpWordItem.ptr_wd = myString.c_str();
//					cout<<"loadData: tmpWordItem.ptr_wd:"<<tmpWordItem.ptr_wd<<endl;
//					ary.push_back(tmpWordItem);
//			}
//			*/
//           for(;*tmp != '\0';tmp+=2) {
//		      wordItem tmpWordItem ;
//	          tmpWordItem.length = 2;
//              tmpWordItem.ptr_wd = tmp;
//			  ary.push_back(tmpWordItem);
//
//			  cout<<"loadData: tmpWordItem.ptr_wd:"<<tmpWordItem.ptr_wd<<endl;
//		   }
//		}
//     return true;
//}
//
//int main()
//{
//	string conf_file = "/data/xce/SegmenGBK/SegmentGBK/config/segment.conf";
//	string data_file = "/data/xce/SegmenGBK/SegmentGBK/data/name_data.txt";
//    string output_file = "/data/xce/SegmenGBK/SegmentGBK/data/output.txt";
//	wordAry ary ;
//    wordAry result;
//    NameEntityProber *_p_name_prober = _init(conf_file.c_str());
//    if( _p_name_prober==NULL ) {
//        cout<<"加载config出错\n";
//        return 0;
//	}
//
//
//    if (!loadData(data_file.c_str(),ary)){
//		cout<<"加载数据文件出错\n";
//		return 0;
//	}
//
//    size_t start_idx = 0;
//    int _nsize = ary.size();
//	
//	for( int i=0;i<_nsize;i++){
//	  
//
//      if(!_p_name_prober->probeNameEntity( ary , start_idx , result )){
//	    if(start_idx<ary.size()-1) 
//		start_idx = start_idx+1;
//		else break;
//	//	cout<<" start_idx +1 is .........................."<< start_idx <<endl<<endl;
//	 }
//	//	cout<<"result.size is::"<<result.size()<<endl;
//	//	cout<<"......................\n";
//    }
//    /*
//		for (int k = 0;k<result.size();k++){
//			cout<<".....result["<<k<<"]is:"<<result[k].ptr_wd<<endl;
//		}
//	*/
//    ofstream outfile(output_file.c_str());
//    for ( int j = 0;j<result.size();j++){
//		const char * tmpPwd = result[j].ptr_wd;
//		cout << tmpPwd << endl;
//		if ( !outfile.is_open() ) return false;
//		outfile << tmpPwd << endl;
//	}
//	outfile.close();
//
//    delete _p_name_prober;
//    return 1;
//}
