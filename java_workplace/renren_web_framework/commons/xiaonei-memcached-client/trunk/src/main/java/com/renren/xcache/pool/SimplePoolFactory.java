package com.renren.xcache.pool;




/**
 * 简单的{@link CachePoolFactory}实现，可用于测试
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:18:36
 */
public class SimplePoolFactory extends AbstractPoolFactory {

	private static SimplePoolFactory instance = new SimplePoolFactory();
	
	public static SimplePoolFactory getInstance() {
		return instance;
	}

	private SimplePoolFactory() {
		
	}

	@Override
	protected CachePool loadPoolConfig(String poolName) {
		CachePoolImpl pool = new CachePoolImpl();
		pool.setName(poolName);
		pool.addNode(new NodeImpl().setHostname("10.22.200.140").setPort(11211));
		//pool.addNode(new NodeImpl().setHostname("10.22.200.140").setPort(11212));
		//pool.addNode(new NodeImpl().setHostname("10.22.200.140").setPort(11213));
		return pool;
	}
	
	public static void main(String[] args) {
		SimplePoolFactory fact = SimplePoolFactory.getInstance();
		System.out.println(fact.getPool("haha"));
		
		CachePoolImpl pool = new CachePoolImpl();
		pool.setName("haha");
		pool.addNode(new NodeImpl().setHostname("10.22.200.140").setPort(11211));
		pool.addNode(new NodeImpl().setHostname("10.22.200.140").setPort(11212));
		pool.addNode(new NodeImpl().setHostname("10.22.200.140").setPort(11213));
		pool.addNode(new NodeImpl().setHostname("10.22.200.141").setPort(11213));
		
		fact.onPoolModified(pool);
		
		System.out.println(fact.getPool("haha"));
	}
	
}
