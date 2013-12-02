#ifndef __AB_CONFIG_RESOURCE_H__
#define __AB_CONFIG_RESOURCE_H__

#include "AbConfigResourceImpl.h"


/* apr_xml_elem's format
struct apr_xml_elem {
    const char *name;    // element name     
    int ns; // index into namespace array
    const char *lang;     // xml:lang for attrs/contents
    apr_text_header first_cdata; // cdata right after start tag 
    apr_text_header following_cdata; //cdata after MY end tag
    struct apr_xml_elem *parent; //parent element
    struct apr_xml_elem *next; // next (sibling) element
    struct apr_xml_elem *first_child; //first child element 
    struct apr_xml_attr *attr; //first attribute


    // used only during parsing 
    // last child element 
    struct apr_xml_elem *last_child;
    // namespaces scoped by this elem
    struct apr_xml_ns_scope *ns_scope;
    // used by modules during request processing
    // Place for modules to store private data 
    void *priv;
};
*/
/* apr_xml_attr's struct
struct apr_xml_attr {
    const char *name;    // attribute name
    int ns;     //index into namespace array
    const char *value;    // attribute value
    struct apr_xml_attr *next;     // next attribute
};
*/

class AbXmlConfigResource{
public:

	AbXmlConfigResource();
	~AbXmlConfigResource();
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
	AbXmlConfigResourceImpl * impl_;
};


#endif
