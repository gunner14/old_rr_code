#include "intkey.h"


//-------------------------------------------------------------------------------------------------
void CIntKey::AddKey(int key)
{
	m_Key.insert(key);
}


//-------------------------------------------------------------------------------------------------
void CIntKey::RemoveKey(int key)
{
	m_Key.erase(key);
}


//-------------------------------------------------------------------------------------------------
bool CIntKey::ContainKey(int key)
{
	return (m_Key.find(key)!=m_Key.end());
}


//-------------------------------------------------------------------------------------------------
bool CIntKey::IndexToKey(int i, int &key)
{
	if((unsigned int)i>=m_Key.size()) return false;
	else
	{
		set<int>::iterator iter = m_Key.begin();
		advance(iter, i);
		key = *iter;
		return true;
	}
}


//-------------------------------------------------------------------------------------------------
bool CIntKey::KeyToIndex(int key, int &i)
{
	set<int>::iterator iter = m_Key.find(key);
	if(iter==m_Key.end()) return false;
	else
	{
		i = distance(m_Key.begin(), iter);
		return true;
	}
}


//-------------------------------------------------------------------------------------------------
int  CIntKey::Size()
{
	return m_Key.size();
}


//-------------------------------------------------------------------------------------------------
void CIntKey::Clear()
{
	m_Key.clear();
}

