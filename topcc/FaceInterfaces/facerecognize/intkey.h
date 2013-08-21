#pragma once
#include <set>
using namespace std;


//-------------------------------------------------------------------------------------------------
class CIntKey
{
public:
	void AddKey(int key);
	void RemoveKey(int key);
	bool ContainKey(int key);

	bool IndexToKey(int i, int &key);
	bool KeyToIndex(int key, int &i);

	int  Size();
	void Clear();

protected:
	set<int> m_Key;
};

