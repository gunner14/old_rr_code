package com.renren.xoa2.client;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.impl.NodeImpl;

/**
 * 用来测试
 * 
 * @author yuan.liu1@renren-inc.com
 *
 */
public class FakeBalancer implements LoadBalancer {

	@Override
	public Node getNode(String serviceId, String version) {
		NodeImpl node = new NodeImpl();
		node.setHost("127.0.0.1");
		node.setPort(Integer.parseInt(serviceId));
		return node;
	}

	@Override
	public Node getNode(String serviceId, String version, int shardParam) {
		return getNode(serviceId, version);
	}
}
