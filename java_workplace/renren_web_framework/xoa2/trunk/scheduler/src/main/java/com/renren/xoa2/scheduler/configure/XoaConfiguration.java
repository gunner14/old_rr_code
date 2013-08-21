package com.renren.xoa2.scheduler.configure;

import java.io.File;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * 调度任务的配置管理
 * 
 * @author Yuan Liu (yuan.liu1@renren-inc.com)
 * @date   2012-3-31
 */
public class XoaConfiguration {
    private static Logger logger = LoggerFactory.getLogger(XoaConfiguration.class);
    
    // 入口节点
    public static final String SCHEDULER_TAG = "xoa-scheduler";

    /**
     * 当前任务启动多少个服务副本 
     */
    public static final String DUP_NUMBER = "duplication-number";
    
    /**
     * 服务时间设定，即在Zookeeper上可见的时间<br><br>
     * <b>NOTICE</b>: <br>
     *  1. 需要和空闲超时时间区分<br>
     *  2. 单位为秒 
     */
    public static final String SERVICE_TIME = "service-time";
    
    /**
     * 服务空闲超时时间，即服务已经在Zookeeper上不可见，防止强行杀死带来的复杂问题
     */
    public static final String SERVICE_IDLE_TIMEOUT = "service-idle-timeout";
    
    /**
     * 服务配置文件路径
     */
    public static final String SERVICE_CONFIG_PATH = "service-conf-path";
    
    public static final String VALUE_TAG = "value";
    
    /**
     * TODO: [可选]端口范围，在设定的范围内选择，防止占用其他已知应用端口
     */
    public static final String PORT_RANGE = "port-range";
    
    /**
     * 
     * @param confName 配置文件名
     * @return 成功 返回配置<br>
     *         失败 返回null
     */
    public static XoaJobConfigure getJobConfigure(String confName) {
        if(confName == null || confName.trim().length()==0){
            throw new IllegalArgumentException("String confName is empty.");
        }

        File file = new File(confName);
        XoaJobConfigure jobConf = null;
        try {
            Document doc =
                DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file);

            NodeList nodeList = doc.getElementsByTagName(SCHEDULER_TAG);

            if (nodeList != null && nodeList.getLength() == 1) {
                NodeList itemList = nodeList.item(0).getChildNodes();
                jobConf = new XoaJobConfigure();

                for (int i = 0; i < itemList.getLength(); i++) {
                    Node tnode = itemList.item(i);
                    if (tnode.getNodeName().equals(DUP_NUMBER)) {
                        // duplicate-number
                        int dupNumber = Integer.parseInt(tnode.getAttributes()
                                .getNamedItem(VALUE_TAG).getNodeValue());
                        jobConf.setTaskNumber(dupNumber);

                    } else if (tnode.getNodeName().equals(SERVICE_TIME)) {  
                        // service-time
                        int serviceTime = Integer.parseInt(tnode.getAttributes()
                                .getNamedItem(VALUE_TAG).getNodeValue());
                        
                        jobConf.setServiceTime(serviceTime);
                        
                    } else if (tnode.getNodeName().equals(SERVICE_IDLE_TIMEOUT)) {
                        // service-idle-timeout
                        int serviceIdleTime = Integer.parseInt(tnode.getAttributes()
                                .getNamedItem(VALUE_TAG).getNodeValue());
                        
                        jobConf.setServiceIdleTime(serviceIdleTime);
                    } else if (tnode.getNodeName().equals(SERVICE_CONFIG_PATH)) {
                        // service-config-path
                        String serviceConf = tnode.getAttributes()
                                .getNamedItem(VALUE_TAG).getNodeValue();
                        
                        jobConf.setServiceConf(serviceConf);
                    }
                }                
            } else {
                logger.error("Job configuration invalid!");
            }

        } catch (ParserConfigurationException e) {
            logger.error("Job configuration invalid!", e);
        } catch (SAXException e) {
            logger.error("Job configuration invalid!", e);
        } catch (IOException e) {
            logger.error("Job configuration invalid!", e);
        }

        return jobConf;
    }
}
