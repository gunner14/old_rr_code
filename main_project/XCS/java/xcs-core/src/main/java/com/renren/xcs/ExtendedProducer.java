package com.renren.xcs;

import java.io.IOException;

import com.renren.ad.datacenter.find.server.Producer;

public class ExtendedProducer extends Producer {

    public ExtendedProducer(String clusterAddress, int sessionTimeout) throws IOException {
        super(clusterAddress, sessionTimeout);
    }

    /**
     * Publish the service address to zookeeper. 将当前的服务地址信息发布到zookeeper集群。
     * 
     * @param service 服务的唯一名称，例如com.renren.ad.find
     * @param version 服务的版本信息，例如100
     * @param stat 服务的状态信息，例如分区的服务则用这个表示不同的分区
     * @param address 服务的地址信息，这就是本机打算发布的信息
     * @param config 服务的配置信息，例如一个key/value属性文件: disabled=true
     * @param isTmp 是否临时节点。临时节点在服务器关闭后会自动从zookeeper集群删除，否则用就存在。
     * @throws FindException假如发布失败
     */
    public void updateService(String service, int version, String stat, String address,
            byte[] config) {

        if (this.root == null) {
            throw new IllegalStateException("Root not set.");
        }
        if (version < 1) {
            throw new IllegalArgumentException("Version must greater than 0.");
        }

        StringBuilder path = new StringBuilder();
        path.append("/").append(root).append("/").append(service);
        path.append("/").append(version).append("/").append(stat);
        path.append("/").append(address);
        LOG.info("Try to update address: " + path + " config to: " + new String(config));

        super.updateNode(path.toString(), config);
    }

}
