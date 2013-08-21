#include <vector>
#include <algorithm>

using namespace std;

struct my_greater: public std::greater<int>{
	bool operator()(const int&a,const int&b) const {
		return a>b;
	}
};

struct my_less: public std::less<int>{
	bool operator()(const int&a,const int&b) const {
		return a<b;
	}
};

void test_greater_sort(vector<int>&);
void test_less_sort(vector<int>&);
void test_lower_bound(vector<int>&);
void test_erase(vector<int>&);

void test_vector(){
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> [test_vector] >>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	cout << "[test_vector] 容器名: vector" << endl;
	cout << "[test_vector] 头文件: <vector>" << endl;
        cout << endl;
	vector<int> vec;
	vec.push_back(7);
	vec.push_back(2);
	vec.push_back(5);
	vec.push_back(1);
	vec.push_back(4);
	vec.push_back(8);
	vec.push_back(2);
	cout << "[test_vector] vector inited: ";
	for( vector<int>::iterator it=vec.begin();it!=vec.end();++it ){
		cout << *it << " ";
	}
        cout << endl << endl;

	test_greater_sort(vec);

	test_less_sort(vec);

	test_lower_bound(vec);

	test_erase(vec);

        cout << endl;
}

void test_greater_sort(vector<int>& vec){
	cout << "[test_vector] test_greater_sort >>>>>>>>>>" << endl;
	std::sort( vec.begin(),vec.end(),my_greater() );
	cout << "[test_vector] vector sorted: ";
	for( vector<int>::iterator it=vec.begin();it!=vec.end();++it ){
		cout << *it << " ";
	}
        cout << endl << endl;
}

void test_less_sort(vector<int>& vec){
	cout << "[test_vector] test_less_sort >>>>>>>>>>" << endl;
	std::sort( vec.begin(),vec.end(),my_less() );
	cout << "[test_vector] vector sorted: ";
	for( vector<int>::iterator it=vec.begin();it!=vec.end();++it ){
		cout << *it << " ";
	}
        cout << endl << endl;
}

void test_lower_bound(vector<int>& vec){
	cout << "[test_vector] test_lower_bound >>>>>>>>>>" << endl;

	int _double_one = 2;
        vector<int>::iterator it = lower_bound(vec.begin(),vec.end(),_double_one );
	cout << "[test_vector] find " << _double_one << ", position:" << it - vec.begin() << " *it=" << *it << endl;

	int _miss_one = 3;
        it = lower_bound(vec.begin(),vec.end(),_miss_one );
	cout << "[test_vector] find " << _miss_one << ", position:" << it - vec.begin() << " *it=" << *it << endl;
        cout << endl;
}

void test_erase(vector<int>& vec){
	cout << "[test_vector] test_erase >>>>>>>>>>" << endl;
	std::vector<int>::iterator it1 = find(vec.begin(),vec.end(),4);
	std::vector<int>::iterator it2 = it1;
	std::vector<int>::iterator it3 = it1;
	cout << "[test_vector] it1: " << *it1 << endl; 
	cout << "[test_vector] it2: " << *it2 << endl; 
	cout << "[test_vector] it3: " << *it3 << endl; 
	it3 = vec.erase(it2);
	cout << "[test_vector] erasing ..." << endl; 
	cout << "[test_vector] it1: " << *it1 << endl; 
	cout << "[test_vector] it2: " << *it2 << endl; 
	cout << "[test_vector] it3: " << *it3 << endl; 
        cout << endl;
}

