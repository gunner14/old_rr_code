package com.renren.xoa2.client;

import org.apache.thrift.transport.TSocket;

import com.renren.xoa2.client.transport.XoaTransport;

/**
 * 服务路由器
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Dec 1, 2011 6:56:00 PM
 */
public interface ServiceRouter {

    /**
     * 获取到指定service的路由
     * 
     * @param serviceId
     * @return
     */
    public XoaTransport routeService(String serviceId, String version, String shardBy) throws Exception;    

    /**
     * 将transport连接归还到连接池
     * 
     * @param key
     *            对应的IP@端口
     **/
    public void returnConn(XoaTransport xoaTransport) throws Exception;

    /**
     * 客户端调用出现异常情况
     * 
     * @param Exception
     * @param 异常service
     *            name，ip
     **/
    public void serviceException(String serviceId, String version, Throwable e, XoaTransport xoaTransport);
    
    /**
     * 路由Service，建立连接和传输数据时。
     * 
     * TODO: 其实不应该这样搞，只是为了不改变routeService接口参数。
     * routeService 应该 关心timeout，因为路由服务、建立连接的时候才有超时这一说。 by Xun Dai
     * 
     * @param timeout 超时时间millisecond.
     * @see TSocket#setTimeout(int)
     * 
     */
    public void setTimeout(long timeout);
}
