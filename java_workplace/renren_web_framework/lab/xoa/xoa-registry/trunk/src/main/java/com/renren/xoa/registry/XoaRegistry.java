package com.renren.xoa.registry;

import java.util.List;

/**
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-19 下午03:59:52
 */
public interface XoaRegistry {
	
	/**
	 * 返回一个服务节点
	 * @param serviceId
	 * @return
	 */
	public XoaServiceDescriptor queryService(String serviceId);
	
	/**
	 * 返回Registry注册的所有服务节点
	 * @param serviceId
	 * @return
	 */
	public List<XoaServiceDescriptor> queryServices(String serviceId);
	
	/**
	 * 获取指定的service配置
	 * @param serviceId
	 * @return
	 */
	public XoaService getService(String serviceId);
	
	/**
	 * 获取素有的service配置
	 * @return
	 */
	public List<XoaService> getServices();
	
	/**
	 * 注册事件监听器
	 * @param listener
	 */
	public void addListener(XoaRegistryListener listener);
	
	/**
	 * 查询指定的ip和port正在为那些serviceId提供服务
	 * 
	 * @param ip
	 * @param port
	 * @return
	 */
	public List<String> lookup(String ip, int port);
}
