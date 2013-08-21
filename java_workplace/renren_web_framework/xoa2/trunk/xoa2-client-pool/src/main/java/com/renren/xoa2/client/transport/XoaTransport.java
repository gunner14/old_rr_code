/**   
 * @ XoaTransport.java Create on 2011-12-22 下午4:18:17   
 *     
 */   
package com.renren.xoa2.client.transport;   

import org.apache.thrift.transport.TTransport;

import com.renren.xoa2.registry.Node;

  
/**   
 * @author gang.pan 
 * @mail gang.pan@renren-inc.com 
 * @version 1.0
 *
 * TODO: 剥离对于registry的依赖，比如这里仅仅管理 Socket 对象而不是 TTransport
 */

public class XoaTransport {

    private TTransport transport;
    private Node node;
    private boolean isDisabled = false;
    public TTransport getTransport() {
        return transport;
    }
    public void setTransport(TTransport transport) {
        this.transport = transport;
    }
    public Node getNode() {
        return node;
    }
    public void setNode(Node node) {
        this.node = node;
    }
    public boolean isDisabled() {
        return isDisabled;
    }
    public void setDisabled(boolean isDisabled) {
        this.isDisabled = isDisabled;
    }
}
  