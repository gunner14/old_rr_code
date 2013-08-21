package com.renren.xoa.server;

/**
 * 服务器组件，共享{@link ServerContext}上下文
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-2 下午12:01:49
 */
public interface XoaServerComponent {

	/**
	 * 完成初始化
	 * @param serverContext 服务器上下文变量
	 */
	public void init(ServerContext serverContext);
	
}
