package com.renren.xoa.registry;

/**
 * XoaServiceDescriptor的基础实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
public class XoaServiceDescriptorBase implements XoaServiceDescriptor{

	private String ipAddress;
	
	private String serviceId;
	
	private boolean disabled;
	
	private int port;

	public XoaServiceDescriptorBase setServiceId(String serviceId) {
		this.serviceId = serviceId;
		return this;
	}

	public XoaServiceDescriptorBase setIpAddress(String ipAddress) {
		this.ipAddress = ipAddress;
		return this;
	}

	public XoaServiceDescriptorBase setPort(int port) {
		this.port = port;
		return this;
	}

	@Override
	public String getServiceId() {
		return serviceId;
	}

	@Override
	public int getPort() {
		return port;
	}

	@Override
	public String getIpAddress() {
		return ipAddress;
	}

	@Override
	public boolean isDisabled() {
		return this.disabled;
	}

	public XoaServiceDescriptorBase setDisabled(boolean disabled) {
		this.disabled = disabled;
		return this;
	}
	
	@Override
	public String toString() {
		return serviceId + "," + ipAddress + ":" + port + ","
				+ (disabled ? STATUS_DISABLED : STATUS_ENABLED);
	}
	
	
}
