#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>


using namespace std;
using namespace boost;

#define foreach BOOST_FOREACH
//
// oce_log.2012-07-05-07
// oce_log.2012-07-05-07.1
// 
//
void process(const string & s,size_t & num_all,size_t &num_1205)
{
   ifstream file(s.c_str()); 
   if(file.fail()) {
       //cout<<"no this file "<<s<<endl;
       return ;
   }
   string temp;
   size_t _all=0;size_t _1205=0;
   while( getline(file,temp,'\n') ) 
   {
       //cout<<temp<<endl;
       if( temp.find("AddFeedMini")!=string::npos && temp.find("stype:")!=string::npos ) 
       {
           ++_all;
           if(temp.find("stype:1205")!=string::npos)
           {
               ++_1205;
           }
       }
   }
   cout<<"all:"<<_all<<" 1205:"<<_1205<<endl;
   num_all+=_all;
   num_1205+=_1205;
}
int main2()
{
   size_t num_type_all=0;
   size_t num_type_1205=0;
   string name;
   cout<<"input a file contains lognames"<<endl;
   cin>>name;
   ifstream file(name.c_str());
   if(file.fail()) {
       cout<<"file error."<<name<<endl;
   }
   vector<string> logs;
   string log;
   while(getline(file,log,'\n')) {
       logs.push_back(log);
   }
   foreach(string & log,logs)
   {
       //if(log=="")
       //     continue;
       cout<<"process log:"<<log<<endl;
       process(log,num_type_all,num_type_1205);
   }
   cout<<"all: "<<num_type_all<<endl;
   cout<<"1205: "<<num_type_1205<<endl;
   return 0;
}

int main()
{
   vector<string> inputs;
   vector<string> files;
   string line;
   cout<<"input year, mont1,day1,day2; eg: 2012,7,5,9"<<endl;
   cin>>line;
   split(inputs,line,is_any_of(","),token_compress_on);
   if( inputs.size() != 4 ) {
       cout<<"input num wrong, we need 8."<<endl;    
   }
   //cout<<
   //make file name
   int year,month1,day1,day2;
   stringstream ss;
   ss<<inputs[0];ss>>year;ss.clear();
   ss.str(inputs[1]);ss>>month1;ss.clear();
   ss.str(inputs[2]);ss>>day1;ss.clear();
   ss.str(inputs[3]);ss>>day2;ss.clear();
   //check  
   if( year>2050 || year<2005 ) {
       cout<<"year1 error. >2050,<2005"<<endl;
       return 0;
   }
   if( month1>12 || month1<1) {
       cout<<"month1 error. >12 <1"<<endl;
       return 0;
   }
   if( day1>31 || day1<1) { 
       cout<<"day1 error. >31 <1"<<endl;
       return 0;
   }
   if( day2>31 || day2<1) { 
       cout<<"day2 error. >31 <1"<<endl;
       return 0;
   }
   //cout<<year<<" "<<month1<<" "<<da
   //make file name
   //oce_log.2012-12-12-23
   string temp1,temp2,temp3;
   temp1="oce_log.";
   temp1+=lexical_cast<string>(year);
   temp1+="-";
   if(month1<10) temp1+="0";
   temp1+=lexical_cast<string>(month1);
   temp1+="-";
   temp3="./log/TimeLineFeed";
   size_t all=0;
   size_t t_1205=0;
   for(int j=day1;j<=day2;++j) {
       for(int i=0;i<24;++i) {
           temp2=temp1;        
           if(j<10) temp2+="0";
           temp2+=lexical_cast<string>(j);
           temp2+="-";
           if(i<10) temp2+="0";
           temp2+=lexical_cast<string>(i);
           for(int m=0;m<10;++m) {
               process(temp3+lexical_cast<string>(m)+"/"+temp2,all,t_1205);
               process(temp3+lexical_cast<string>(m)+"/"+temp2+".1",all,t_1205);
           }
           //files.push_back(temp);
       }
   }
   cout<<all<<" "<<t_1205<<endl;
}



