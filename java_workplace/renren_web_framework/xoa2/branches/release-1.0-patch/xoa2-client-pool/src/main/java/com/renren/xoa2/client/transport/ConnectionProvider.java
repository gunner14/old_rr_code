/**   
 * @ ConnectionProvider.java Create on 2011-9-15 上午11:19:58   
 *     
 */   
package com.renren.xoa2.client.transport;   
import org.apache.thrift.transport.TTransport;

import com.renren.xoa2.registry.Node;
  
/**   
 * TODO: 接口处于不一致情况, 尽快修复<br>
 * 		获取的是 TTransport, 返回的却是 XoaTransport
 * 
 * @author gang.pan 
 * @mail gang.pan@renren-inc.com 
 * @version 1.0   
 */
public interface ConnectionProvider
{
    /**
     * 获取一个链接
     * 
     * @param node 连接描述信息 {@link Node}
     * 
     * @return
     */
    public TTransport getConnection(Node node) throws Exception;

    /**
     * 返回链接<br>
     * 
     * 	如果链接不是由 getConnection 返回的，则会抛出异常
     * 
     * @param xoaTransport 
     */
    public void returnConnection(XoaTransport xoaTransport) throws Exception;
    

    public void invalidateConnection(XoaTransport xoaTransport);
    
    public void clearConnections(Node node);
  
}
  