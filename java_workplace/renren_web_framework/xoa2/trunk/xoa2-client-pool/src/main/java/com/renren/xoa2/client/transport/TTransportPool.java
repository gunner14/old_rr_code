package com.renren.xoa2.client.transport;

import org.apache.thrift.transport.TTransport;

/**
 * {@link TTransport}的缓存池，相当于Thrift的连接池
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 29, 2011 5:57:23 PM
 */
public interface TTransportPool {

    /**
     * 获取到指定IP和端口的{@link TTransport}
     * 
     * @param ip
     * @param port
     * @return
     */
    public TTransport get(String ip, int port);

}
