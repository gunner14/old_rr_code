
#ifndef __AB_XML_CONFIG_H__
#define __AB_XML_CONFIG_H__


#ifdef __cplusplus
extern "C" {
#endif

struct apr_pool_t;
struct apr_xml_doc;
struct ab_xmlconfig_tree_t;
struct apr_xml_attr;
struct apr_xml_elem;

typedef  struct ab_xmlconfig_tree_t {
	apr_pool_t *pool;     /*  pool to do parse. */
        apr_xml_doc *doc;    /*  result xml config tree. */
	const char* fname;  /* config file name.  */
} ab_xmlconfig_tree_t;

void apr_terminate();


/**
 * load a xmlconfig tree from  file.
 *
 * @param fname  configure file.
 * @return the result xmlconfig_tree.
 */
ab_xmlconfig_tree_t  *xmlconfig_tree_load(const char *fname);


/**
* parse the xmlconfig tree from the buf
* 
*/
ab_xmlconfig_tree_t *xmlconfig_tree_parse(char const* szBuf, int len);

/**
 * destroy  xmlconfig tree.
 *
 */
void xmlconfig_tree_destroy(ab_xmlconfig_tree_t *config);


/**
 * debug xml tree.
 *
 */
void xmlconfig_dump_elem(const apr_xml_elem *e, int level);

/**
 *  get the first matched sub xml tree.
 *
 * @param config the loaded xml config tree.
 * @param path  the path to the xml elem, seperated by ':'.
 * @return  xml elem to be found.
 *	return NULL, if fail.
 */
const apr_xml_elem *xmlcofig_get_subtree(ab_xmlconfig_tree_t *config, const char* path);

const apr_xml_elem *xmlcofig_get_root(ab_xmlconfig_tree_t *config);

const apr_xml_attr *xmlcofig_get_attr(const apr_xml_elem *e, const char* attrname);

const char * xmlconfig_get_attr_value(ab_xmlconfig_tree_t *config, const char* path,const char * attrname);



#ifdef __cplusplus
}
#endif



#endif 
