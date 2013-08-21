package com.xiaonei.commons.jade.impl.xceadapter.datasource;

import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.xiaonei.commons.jade.impl.util.XMLUtils;

/**
 * 负责从配置项生成对应的配置描述对象。
 * 
 * @author han.liao
 */
public class XceDescriptorLoader {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(XceDescriptorLoader.class);

    public static XceDescriptor create(String dbname) {

        return new DirectXceDescriptor(dbname);
    }

    public static XceDescriptor fromProps(String keyword, Map<String, String> props,
            String defaultDbname) {

        // catalog.table_name = dbname
        String dbname = props.get(keyword);

        if (dbname == null) {

            // catalog = dbname
            dbname = defaultDbname;
        }

        if (dbname != null) {

            // 输出日志
            if (logger.isDebugEnabled()) {
                logger.debug("Found config [keyword = " + keyword + "] dbname = " + dbname);
            }

            // 加载散表配置
            XceRouter partitionRouter = XceRouterLoader.fromProps(keyword + ".partition_route",
                    props);

            // 加载散库配置
            XceRouter dbRouter = XceRouterLoader.fromProps(keyword + ".db_route", props);

            if (dbRouter != null || partitionRouter != null) {

                RouterXceDescriptor descriptor = new RouterXceDescriptor(dbname);

                descriptor.setPartitionRouter(partitionRouter);
                descriptor.setDbRouter(dbRouter);

                return descriptor;

            } else {

                return new DirectXceDescriptor(dbname);
            }
        }

        return null;
    }

    public static void loadXMLDoc(Map<String, XceDescriptor> map, Document document) {

        Element element = document.getDocumentElement();

        if (!"jade-config".equalsIgnoreCase(element.getTagName())) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("XML ROOT must be <jade-config>.");
            }

            return;
        }

        for (Element child : XMLUtils.getChildren(element)) {

            if (!"catalog".equalsIgnoreCase(child.getTagName())) {
                continue;
            }

            loadCatalog(map, child);
        }
    }

    public static void loadCatalog(Map<String, XceDescriptor> map, Element element) {

        String catalog = element.getAttribute("id");

        if (catalog == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("XML node <catalog> must have 'id' attribute.");
            }

            return;
        }

        String defaultDbName = null; // 配置的  <default-dbname> 

        for (Element child : XMLUtils.getChildren(element)) {

            if ("default-dbname".equalsIgnoreCase(child.getTagName())) {

                defaultDbName = XMLUtils.getText(child);

            } else if ("table".equalsIgnoreCase(child.getTagName())) {

                String name = child.getAttribute("name");

                if (name == null) {

                    // 输出日志
                    if (logger.isErrorEnabled()) {
                        logger.error("XML node <table> must have 'name' attribute.");
                    }

                } else {

                    // 保存数据表的配置信息
                    XceDescriptor descriptor = fromXML(child, defaultDbName);

                    // 输出日志
                    if (logger.isDebugEnabled()) {
                        logger.debug("Found config [catalog = " + catalog + ", name = " + name
                                + "] dbname = " + descriptor.getDbName());
                    }

                    map.put(catalog + '.' + name, descriptor);
                }
            }
        }

        if (defaultDbName != null) {

            // 输出日志
            if (logger.isDebugEnabled()) {
                logger.debug("Found config [catalog = " + catalog + "] dbname = " + defaultDbName);
            }

            // 保存模块的配置信息
            map.put(catalog, new DirectXceDescriptor(defaultDbName));
        }
    }

    public static XceDescriptor fromXML(Element element, String defaultDbname) {

        RouterXceDescriptor descriptor = new RouterXceDescriptor(defaultDbname);

        for (Element child : XMLUtils.getChildren(element)) {

            if ("dbname".equalsIgnoreCase(child.getTagName())) {

                descriptor.setDbName(XMLUtils.getText(child));

            } else if ("db-partitions".equalsIgnoreCase(child.getTagName())) {

                descriptor.setDbRouter(XceRouterLoader.fromXML(child));

            } else if ("table-partitions".equalsIgnoreCase(child.getTagName())) {

                descriptor.setPartitionRouter(XceRouterLoader.fromXML(child));
            }
        }

        return descriptor;
    }
}
