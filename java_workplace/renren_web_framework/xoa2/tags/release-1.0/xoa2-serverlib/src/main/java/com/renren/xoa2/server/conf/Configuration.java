/**
 * 
 */

package com.renren.xoa2.server.conf;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.renren.xoa2.server.impl.XoaServiceConfig;

/**
 * Configuration类为服务配置文件（eg: xoa.xml）的对象表示，将配置文件中xoa服务配置保存在<code>serviceConfigList</code>中。
 * 
 * @author jinchao.wen
 * @email jinchao.wen@opi-corp.com
 * @date 2011-12-8
 */
public class Configuration {

    private static Logger logger = LoggerFactory.getLogger(Configuration.class);

    public static final String SERVICE_TYPE_XOA2 = "xoa2";
    public static final String SERVICE_TYPE_XMQ = "xmq";

    public static final String SERVICE_TAG = "service";

    public static final String ID_ATTRIBUTE = "id";

    public static final String TYPE_ATTRIBUTE = "type";

    public static final String CLASS_ATTRIBUTE = "class";

    public static final String PORT_ATTRIBUTE = "port";
    
    public static final String LISTENER_TAG = "service-listener";
    
    public static final String MILITIA_ATTRIBUTE = "militia";
    
    public static final String WORKER_THREAD_POOL_TAG = "worker-thread-pool";
    public static final String CORE_SIZE_ATTR = "core-size";
    public static final String MAX_SIZE_ATTR = "max-size";
    public static final String MILITIA_FLAG = "flag";
    
    private List<XoaServiceConfig> serviceConfigList;
    
    public void addServiceConfig(XoaServiceConfig config){
        if( null == serviceConfigList)
            serviceConfigList = new ArrayList<XoaServiceConfig>();
        
        serviceConfigList.add(config);
    }
    
    public List<XoaServiceConfig> listServiceConfig(){
        return Collections.unmodifiableList(serviceConfigList);
    }
    
    @Override
    public String toString(){
        return serviceConfigList.toString();
    }

    public  static class ConfigurationFactory {

        /**
         * 分析confFile文件，生成Configuration对象 如果confFile文件不存在或者格式错误，返回null
         * 
         * @param confFile
         * @return
         */
        public static final Configuration newConfiguration(String confFile) {
            if(confFile == null || confFile.trim().length()==0){
                throw new IllegalArgumentException("String confFile is empty.");
            }
            
            File file = new File(confFile);
            Configuration conf = null;
            try {
                Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file);
                NodeList nodeList = doc.getElementsByTagName(SERVICE_TAG);
                if (nodeList != null && nodeList.getLength() > 0) {
                    conf = new Configuration();

                    Node node = null;
                    for (int i = 0; i < nodeList.getLength(); i++) {
                        node = nodeList.item(i);
                        NamedNodeMap attributes = node.getAttributes();
                        XoaServiceConfig config = new XoaServiceConfig();
                        String id = attributes.getNamedItem(ID_ATTRIBUTE).getNodeValue();
                        config.setServiceId(id);
                        config.setServiceClass(attributes.getNamedItem(CLASS_ATTRIBUTE).getNodeValue());
                        config.setPort(Integer.parseInt(attributes.getNamedItem(PORT_ATTRIBUTE)
                                .getNodeValue()));

                        Node typeItem = attributes.getNamedItem(TYPE_ATTRIBUTE);
                        
                        if (typeItem != null) {
                            String type = typeItem.getNodeValue();
                            if (type.equals(SERVICE_TYPE_XMQ)) {
                                config.setXmqService(true);
                            }
                        }
                        
                        if( node.getChildNodes() != null && node.getChildNodes().getLength() > 0 ){
                            NodeList children = node.getChildNodes();
                            for( int j=0; j<children.getLength(); j++ ){
                            		Node tnode = children.item(j);
                                if (tnode.getNodeName().equals(LISTENER_TAG)) {
                                    String initListenerClass = tnode.getAttributes().getNamedItem( CLASS_ATTRIBUTE ).getNodeValue();
                                    config.setInitListenerClass(initListenerClass);
                                } else if (tnode.getNodeName().equals(WORKER_THREAD_POOL_TAG)) {
                            		int coreSize = Integer.parseInt(tnode.getAttributes().getNamedItem(CORE_SIZE_ATTR).getNodeValue());
                            		int maxSize = Integer.parseInt(tnode.getAttributes().getNamedItem(MAX_SIZE_ATTR).getNodeValue());
                            			config.setThreadPoolCoreSize(coreSize);
                            			config.setThreadPoolMaxSize(maxSize);
                                } else if (tnode.getNodeName().equals(MILITIA_ATTRIBUTE)) {
                                    boolean flag = Boolean.parseBoolean(tnode.getAttributes().getNamedItem(MILITIA_FLAG).getNodeValue());
                                    config.setMilitiaFlag(flag);
                                }
                            }                         
                        }
                        conf.addServiceConfig(config);
                    }
                    
                } else {
                    logger.error(" xoa configuration file has 0 service");
                }

            } catch (SAXException e) {
                logger.error("xoa configuration file is NOT legal.", e);
            } catch (IOException e) {
                logger.error("xoa configuration file is NOT legal.", e);
            } catch (ParserConfigurationException e) {
                logger.error("xoa configuration file is NOT legal.", e);
            }

            return conf;
        }
    }
}
