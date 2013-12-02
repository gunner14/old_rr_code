package com.renren.xcs;

import java.io.IOException;
import java.util.List;

import com.renren.ad.datacenter.find.client.NodeChangeListener;

/**
 * 基于find的ServiceRegistryAccessor实现
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public class FindServiceRegistryAccessor implements ServiceRegistryAccessor {

	private ExtendedConsumer consumer;

	private String service;
	
	private String versionReg;

	private int version = Integer.MIN_VALUE;//Integer.MIN_VALUE代表为初始化的version
	
	private Configuration config;

	@Override
	public String getService() {
		return service;
	}

	@Override
	public int getVersion() {
	    if(version == Integer.MIN_VALUE){
	        version = consumer.getCurrentVersion(this.service, this.versionReg);
	    }
		return version;
	}

	public FindServiceRegistryAccessor(String service, String version,
			Configuration config) {

		this.config = config;
		this.service = service;
		this.versionReg = version;
		try {
			consumer = FindFactorys.getConsumer(config);
		} catch (IOException e) {
			throw new RuntimeException(
					"Failed to connecting to zookeeper cluster '"
							+ this.config.getCluster() + "'.", e);
		}
	}

	@Override
	public byte[] getVersionAndListenChange(final DataChangeListener listener)
			throws ZookeeperException {
		try {
			return consumer
					.getMetaDataAndListenChange(
							this.service,
							this.versionReg,
							listener == null ? null
									: new com.renren.ad.datacenter.find.client.DataChangeListener() {

										@Override
										public void dataChanged(byte[] data) {
											listener.dataChanged(data);
										}
									});
		} catch (Exception e) {
			throw ZookeeperException.valueOf(e);
		}
	}

	@Override
	public List<String> listStatesAndListenChange(
			final ChildrenChangeListener listener) throws ZookeeperException {
		try {
			return consumer.getAllStatAndListenChange(this.service,
					this.versionReg, listener == null ? null
							: new NodeChangeListener() {

								@Override
								public void listChanged(List<String> nodeList) {
									listener.childrenChanged(nodeList);
								}
							});
		} catch (Exception e) {
			throw ZookeeperException.valueOf(e);
		}
	}

	@Override
	public byte[] getStateAndListenChange(String state,
			DataChangeListener listener) throws ZookeeperException {
		throw new IllegalStateException("not supported.");
	}

	@Override
	public List<String> listNodesAndListenChange(String state,
			final ChildrenChangeListener listener) throws ZookeeperException {
		try {
			return consumer.getAddressAndListenChange(this.service,
					this.versionReg, state, listener == null ? null
							: new NodeChangeListener() {

								@Override
								public void listChanged(List<String> nodeList) {
									listener.childrenChanged(nodeList);
								}
							});
		} catch (Exception e) {
			throw ZookeeperException.valueOf(e);
		}
	}

	@Override
	public byte[] getNodeAndListenChange(String state, String node,
			final DataChangeListener listener) throws ZookeeperException {
		try {
			return consumer
					.getAddressAndListenChange(
							this.service,
							this.versionReg,
							state,
							node,
							listener == null ? null
									: new com.renren.ad.datacenter.find.client.DataChangeListener() {

										@Override
										public void dataChanged(byte[] data) {
											listener.dataChanged(data);
										}
									});
		} catch (Exception e) {
			throw ZookeeperException.valueOf(e);
		}
	}

}
