package com.renren.xcache.namespace;

/**
 * 简单的{@link NamespaceFactory}实现，用于测试
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:40:54
 */
public class SimpleNamespaceFactory extends AbstractNamespaceFactory {

	private static final SimpleNamespaceFactory instance = new SimpleNamespaceFactory();
	
	public static SimpleNamespaceFactory getInstance(){
		return instance;
	} 
	
	private SimpleNamespaceFactory() {
		
	}
	
	@Override
	public Namespace loadConfig(String name) {
		NamespaceImpl namespace = new NamespaceImpl();
		namespace.setName(name);
		namespace.setAssociatedPoolName("default");
		return namespace;
	}
	
}
