/**
 * @ GenericConnectionProvider.java Create on 2011-9-15 上午11:12:31
 */
package com.renren.xoa2.client.transport;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.apache.commons.pool.ObjectPool;
import org.apache.commons.pool.impl.GenericObjectPool;
import org.apache.log4j.Logger;
import org.apache.thrift.transport.TTransport;
import com.renren.xoa2.registry.Node;

/**
 * 对连接的管理类,有以下职责:<br>
 * 1. 连接池职责: 建立和唯一标识符(ip+port)对应的连接池信息<br>
 * 2. 继承自接口职责: 获取,归还连接
 *
 * @author gang.pan/yuan.liu1
 * @mail gang.pan@renren-inc.com/yuan.liu1@renren-inc.com
 * @version 1.0
 */
public class TTransportConnectionProvider implements ConnectionProvider {
    private static Logger logger = Logger
            .getLogger(TTransportConnectionProvider.class);

    /** 连接超时配置 */
    private static int conTimeOut = 250;

    // 配置数据 从registry获取
    /** 可以从缓存池中分配对象的最大数量 */
    private static int maxActive = 200;

    /** 缓存池中最大空闲对象数量 */
    private static int maxIdle = 50;

    /** 缓存池中最小空闲对象数量 */
    private static int minIdle = 30;

    /** 阻塞的最大数量 */
    private static long maxWait = 10;

    /** 从缓存池中分配对象，是否执行PoolableObjectFactory.validateObject方法 */
    private boolean testOnBorrow = false;

    private boolean testOnReturn = false;

    private boolean testWhileIdle = false;

    private static HashMap<String, ObjectPool> servicePoolMap = new HashMap<String, ObjectPool>();

    public TTransportConnectionProvider() {}

    public static void setPoolParam(int maxActive, int maxIdle, int minIdle, int maxWait) {
        TTransportConnectionProvider.maxActive = maxActive;
        TTransportConnectionProvider.maxIdle = maxIdle;
        TTransportConnectionProvider.minIdle = minIdle;
        TTransportConnectionProvider.maxWait = maxWait;
    }

    /**
     * 设置连接的超时时间，如果需要针对独立的 IP+Port 设定超时时间，则需要调整 servicePoolMap
     * 的数据结构
     * 
     * @param conTimeOut
     */
    public static void setTimeOut(int conTimeOut) {
        TTransportConnectionProvider.conTimeOut = conTimeOut;     	
    }
    
    synchronized void createPool(String ip, int port) {
        ObjectPool objectPool = new GenericObjectPool();
        //
        ((GenericObjectPool) objectPool).setMaxActive(maxActive);
        ((GenericObjectPool) objectPool).setMaxIdle(maxIdle);
        ((GenericObjectPool) objectPool).setMinIdle(minIdle);
        ((GenericObjectPool) objectPool).setMaxWait(maxWait);
        ((GenericObjectPool) objectPool).setTestOnBorrow(testOnBorrow);
        ((GenericObjectPool) objectPool).setTestOnReturn(testOnReturn);
        ((GenericObjectPool) objectPool).setTestWhileIdle(testWhileIdle);
        // 连接池耗尽，borrowObject方法锁等待
        ((GenericObjectPool) objectPool)
                .setWhenExhaustedAction(GenericObjectPool.WHEN_EXHAUSTED_BLOCK);
        // 设置factory
        ThriftPoolableObjectFactory thriftPoolableObjectFactory = new ThriftPoolableObjectFactory(
                ip, port, conTimeOut);
        objectPool.setFactory(thriftPoolableObjectFactory);
        getservicePoolMap().put(ip + ":" + port, objectPool);
    }

    /**
     * 销毁连接池
     **/
    public void destroy(Node node) {
        try {
            String key = node.getHost() + ":" + node.getPort();
            if (getservicePoolMap().containsKey(key)) {
                getservicePoolMap().get(key).close();
                getservicePoolMap().remove(key);
            }
        } catch (Exception e) {
            throw new RuntimeException("erorr destroy()", e);
        }
    }

    public static String getConnStatus(){
        Iterator iter = servicePoolMap.entrySet().iterator();
        
        StringBuffer message = new StringBuffer();
        while (iter.hasNext()) {
            Map.Entry entry = (Map.Entry) iter.next();
            String keyString = (String) entry.getKey();
            ObjectPool pool = (ObjectPool) entry.getValue();

            message.append("Status of connection [" + keyString + "] is:" + 
            		"\n pool using size: "+pool.getNumActive() +
            		"\n pool idle size:"+pool.getNumIdle() + '\n');
        }
        
        return message.toString();
    }

    @Override
    public TTransport getConnection(Node node) {
    	TTransport transport = null;
        try {
            String key = node.getHost() + ":" + node.getPort();
            if (getservicePoolMap().containsKey(key)) {
                transport = (TTransport) getservicePoolMap()
                        .get(key).borrowObject();
            } else {
            	createPool(node.getHost(), node.getPort());
            	transport = (TTransport) getservicePoolMap().get(key).borrowObject();
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw new RuntimeException(getConnStatus() + "error getConnection()", e);
        }
        return transport;
    }

    @Override
    public void returnCon(XoaTransport xoaTransport) {
        try {
            getservicePoolMap().get(
                    xoaTransport.getNode().getHost() + ":"
                            + xoaTransport.getNode().getPort()).returnObject(
                    xoaTransport.getTransport());
        } catch (Exception e) {
            throw new RuntimeException("error returnCon()", e);
        }
    }

    public int getConTimeOut() {
        return conTimeOut;
    }

    public boolean isTestOnBorrow() {
        return testOnBorrow;
    }

    public void setTestOnBorrow(boolean testOnBorrow) {
        this.testOnBorrow = testOnBorrow;
    }

    public boolean isTestOnReturn() {
        return testOnReturn;
    }

    public void setTestOnReturn(boolean testOnReturn) {
        this.testOnReturn = testOnReturn;
    }

    public boolean isTestWhileIdle() {
        return testWhileIdle;
    }

    public void setTestWhileIdle(boolean testWhileIdle) {
        this.testWhileIdle = testWhileIdle;
    }

    public HashMap<String, ObjectPool> getservicePoolMap() {
        return servicePoolMap;
    }
}
