package com.renren.xoa.proxy.client;

import java.util.concurrent.Executors;

import org.jboss.netty.channel.ChannelFactory;
import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;

/**
 * 
 * 这个类存在的意义在于封装唯一的NioClientSocketChannelFactory实例，
 * 保证不创建多余的IO线程
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-6-4 下午03:14:11
 */
public class ClientChannelFactoryManager {

	private static ClientChannelFactoryManager instance = new ClientChannelFactoryManager();
	
	public static ClientChannelFactoryManager getInstance() {
		return instance;
	}
	
	private NioClientSocketChannelFactory nioClientSocketChannelFactory;
	
	private ClientChannelFactoryManager() {
		try {
			init();
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
	
	private void init() {
		nioClientSocketChannelFactory = new NioClientSocketChannelFactory(
	            Executors.newCachedThreadPool(), Executors.newCachedThreadPool());
	}
	
	public ChannelFactory getClientChannelFactory() {
		return nioClientSocketChannelFactory;
	}
}
