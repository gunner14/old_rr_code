package com.renren.xcs;

import java.io.IOException;

/**
 * 基于Find的ServiceRegistry实现
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public class FindServiceRegistry implements ServiceRegistry {
    
    private Configuration config;

    private ExtendedProducer producer;

    public FindServiceRegistry(Configuration config) {
        try {
            this.config = config;
            producer = FindFactorys.getProducer(config);
        } catch (IOException e) {
            throw new RuntimeException("Failed to connecting to zookeeper cluster '"
                    + this.config.getCluster() + "'.", e);
        }
    }

    @Override
    public void publishService(String serviceId, int version, String stat, Node node, boolean isTemp)
            throws XcsException {
        try {
            producer.publishService(serviceId, version, stat, node.getName(), node.getData(),
                    isTemp);
        } catch (Exception e) {
            throw ZookeeperException.valueOf(e);
        }
    }

    @Override
    public void updateNode(String serviceId, int version, String stat, Node node)
            throws XcsException {
        try {
            producer.updateService(serviceId, version, stat, node.getName(), node.getData());
        } catch (Exception e) {
            throw ZookeeperException.valueOf(e);
        }
    }

    @Override
    public void destroy(){
        try {
            producer.close();
        } catch (Exception e) {
            throw new RuntimeException("Error to destroy.", e);
        }
    }

}
