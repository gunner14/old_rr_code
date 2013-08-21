package com.renren.xoa2.registry.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.datacenter.find.client.NodeChangeListener;
import com.renren.ad.datacenter.find.client.Consumer;
import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.Service;

public class FindXoaRegistry extends AbstractXoaRegistry {
	
	protected static final Log LOG = LogFactory.getLog(FindXoaRegistry.class);
	private static final List<String> DEFAULT_PHONE_LIST = new ArrayList<String>();
	private static Consumer consumer;
	
    static {
        DEFAULT_PHONE_LIST.add("18611951882");
		try {
			Properties props = new Properties();
			props.load(FindXoaRegistry.class.getResourceAsStream("find-conf.properties"));
			consumer = new Consumer(props.getProperty("zk_cluster"),
					Integer.parseInt(props.getProperty("session_timeout")));
			consumer.setRoot(props.getProperty("root"));
			consumer.addAuthInfo(props.getProperty("user"), props.getProperty("passwd"));
		} catch (Exception e) {
			LOG.error("Failed to init Find Consumer!", e);
		}
	}

	@Override
	protected Service loadService(String serviceId, String version,
			int shardParam) {
		if (consumer == null) {
			throw new IllegalStateException("Consumer was not initialized.");
		}
		Service ser = new Service();
		ser.setId(serviceId);
		ser.setAlarmPhones(DEFAULT_PHONE_LIST);
		FindListener listener = new FindListener(ser);
		List<String> list = consumer.getAddressAndListenChange(serviceId, version,
				Integer.toString(shardParam), listener);
		ser.setServiceList(addressToShards(list));
		return ser;
	}
	
	public static List<Node> addressToShards(List<String> list) {
		List<Node> nodes = new ArrayList<Node>();
		if (list != null && list.size() != 0) {
			for (String s : list) {
				NodeImpl n = new NodeImpl();
				n.setDisabled(false);
				n.setHealthy(true);
				String[] u2 = s.split(":", 2);
				n.setHost(u2[0]);
				n.setPort(Integer.parseInt(u2[1]));
				nodes.add(n);
			}
		}
		return nodes;
	}
	
	public static class FindListener implements NodeChangeListener {
		private Service group;
		
		public FindListener(Service group) {
			super();
			this.group = group;
		}

		@Override
		public void listChanged(List<String> list) {
			group.setServiceList(addressToShards(list));
		}
		
	}

}
