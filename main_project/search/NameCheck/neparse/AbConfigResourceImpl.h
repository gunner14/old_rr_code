#ifndef __AB_CONFIG_RESOURCE_IMPL_H__
#define __AB_CONFIG_RESOURCE_IMPL_H__

#include "AbStd.h"
#include "AbXmlConfig.h"


class AbXmlTreeNodeData{
public:
	AbXmlTreeNodeData(){
		m_pChild = NULL;
		m_pPrev = NULL;
		m_pNext = NULL;
		m_pParent = NULL;
	}
public:
	string	m_strElement;
	list<pair< string, string> > m_arrAttrs;
	AbXmlTreeNodeData* m_pChild;
	AbXmlTreeNodeData* m_pPrev;
	AbXmlTreeNodeData* m_pNext;
	AbXmlTreeNodeData* m_pParent;
};

class AbXmlConfigResourceImpl {
public:
	AbXmlConfigResourceImpl() //:ptree_(NULL),group_id_(-1) 
	{
		this->ptree_ = NULL;
	}; 
	~AbXmlConfigResourceImpl() {
	   if(this->ptree_ != NULL){
              xmlconfig_tree_destroy(this->ptree_);
              apr_terminate();
          }

	};
public:
	bool Init(const char* res_file);
	bool InitBuf(char const * szBuf, int len);
	bool GetValue(const string& path, const string& propery, string& value);
	bool GetValues(const string& path, const string& name, list<string>& values);
	AbXmlTreeNodeData* GetXmlTree(string const& path);
    apr_xml_elem const * GetXmlRawTree(string const& path);
    apr_xml_elem const * GetXmlRoot();
	void FreeTreeNode(AbXmlTreeNodeData*& treeNode);
private:
	void GetAttrs(apr_xml_elem const* el, list<pair<string, string> >& attrs);
	void GetXmlTree(apr_xml_elem const* el, AbXmlTreeNodeData* treeNode);
private:
	ab_xmlconfig_tree_t  * ptree_;
};




#endif
