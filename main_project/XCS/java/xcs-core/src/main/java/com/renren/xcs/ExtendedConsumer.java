package com.renren.xcs;

import java.io.IOException;

import com.renren.ad.datacenter.find.client.Consumer;
import com.renren.ad.datacenter.find.client.DataChangeListener;
import com.renren.ad.datacenter.find.core.FindException;

public class ExtendedConsumer extends Consumer {

	public ExtendedConsumer(String clusterAddress, int sessionTimeout)
			throws IOException {
		super(clusterAddress, sessionTimeout);
	}
	
    /**
     * Get address and listen data changes.
     * 获取服务的地址信息列表并且监听地址信息的变化。
     *
     * @param service 服务的唯一名称，例如com.renren.ad.find
     * @param version 支持3种格式，参考[version的格式]章节
     * @param stat 服务的状态信息，例如分区的服务则用这个表示不同的分区
     * @param node 服务节点信息
     * @param listener 监听该列表的变化，将信息变化告知
     * @return 当前的服务地址信息列表
     * @throws FindException 当发生异常时
     */
    public byte[] getAddressAndListenChange(String service, String version, String stat, String node, DataChangeListener listener) {
        StringBuilder path = new StringBuilder();
        path.append("/").append(root).append("/").append(service);
        path.append("/").append(this.getCurrentVersion(service, version)).append("/").append(stat).append("/").append(node);
        try {
            LOG.info("Try to watch data of: " + path);
			return (byte[]) this.submitWatcher(path.toString(), new DataWatcher(listener), false);
        } catch (Exception e) {
        	if (e instanceof RuntimeException) {
        		throw (RuntimeException)e;
        	}
            throw FindException.makeInstance("Failed to get meta data.", e);
        }
    }

}
