/**
 * @ GenericConnectionProvider.java Create on 2011-9-15 上午11:12:31
 */
package com.renren.xoa2.client.transport;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.apache.commons.pool.ObjectPool;
import org.apache.commons.pool.impl.GenericObjectPool;
import org.apache.thrift.transport.TTransport;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

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
    private Logger logger = LoggerFactory
            .getLogger(TTransportConnectionProvider.class);

    /** 连接超时配置 */
    private static int connTimeout = 250;

    // 配置数据 从registry获取
    /** 可以从缓存池中分配对象的最大数量 */
    private static int maxActive = 200;

    /** 缓存池中最大空闲对象数量 */
    private static int maxIdle = 40;

    /** 缓存池中最小空闲对象数量 */
    private static int minIdle = 10;

    /** 最多等待多少毫秒 */
    private static long maxWait = 10;

    /** 从缓存池中分配对象，是否执行PoolableObjectFactory.validateObject方法 */
    private boolean testOnBorrow = false;

    private boolean testOnReturn = false;

    private boolean testWhileIdle = false;

    private static HashMap<String, ObjectPool> servicePoolMap = new HashMap<String, ObjectPool>();

    public TTransportConnectionProvider() {
    }

    public static void setPoolParam(int maxActive, int maxIdle, int minIdle,
            int maxWait) {
        TTransportConnectionProvider.maxActive = maxActive;
        TTransportConnectionProvider.maxIdle = maxIdle;
        TTransportConnectionProvider.minIdle = minIdle;
        TTransportConnectionProvider.maxWait = maxWait;
    }

    /**
     * 设置连接的超时时间，如果需要针对独立的 IP+Port 设定超时时间，则需要调整 servicePoolMap 的数据结构
     * 
     * @param connTimeout
     */
    public static void setTimeout(int connTimeout) {
        TTransportConnectionProvider.connTimeout = connTimeout;
    }

    ObjectPool createPool(Node node) {
        GenericObjectPool objectPool = new GenericObjectPool();

        objectPool.setMaxActive(maxActive);
        objectPool.setMaxIdle(maxIdle);
        objectPool.setMinIdle(minIdle);
        objectPool.setMaxWait(maxWait);
        objectPool.setTestOnBorrow(testOnBorrow);
        objectPool.setTestOnReturn(testOnReturn);
        objectPool.setTestWhileIdle(testWhileIdle);
        // 连接池耗尽，borrowObject方法锁等待
        objectPool
                .setWhenExhaustedAction(GenericObjectPool.WHEN_EXHAUSTED_FAIL);
        // 设置factory
        ThriftPoolableObjectFactory thriftPoolableObjectFactory = new ThriftPoolableObjectFactory(
                node.getHost(), node.getPort(), connTimeout);
        objectPool.setFactory(thriftPoolableObjectFactory);

        return objectPool;
    }

    public static String getConnStatus() {
        Iterator iter = servicePoolMap.entrySet().iterator();

        StringBuffer message = new StringBuffer();
        while (iter.hasNext()) {
            Map.Entry entry = (Map.Entry) iter.next();
            String keyString = (String) entry.getKey();
            ObjectPool pool = (ObjectPool) entry.getValue();

            message.append("Status of connection [" + keyString + "] is:"
                    + "\n pool using size: " + pool.getNumActive()
                    + "\n pool idle size:" + pool.getNumIdle() + '\n');
        }

        return message.toString();
    }

    private static String getNodeKey(Node node) {
        return node.getHost() + ":" + node.getPort();
    }

    @Override
    public TTransport getConnection(Node node) throws Exception {
        TTransport transport = null;
        String key = getNodeKey(node);
        ObjectPool pool = null;
        
        try {
            pool = servicePoolMap.get(key);
            if (pool == null) {
                pool = createPool(node);
                logger.info("pool-stat: pool construction " + key);

                synchronized (this) {
                    if (!servicePoolMap.containsKey(key)) {
                        servicePoolMap.put(key, pool);
                    } else {
                        pool.close();
                        pool = servicePoolMap.get(key);
                    }
                }
            }
            transport = (TTransport) pool.borrowObject();
            
        } catch (java.util.NoSuchElementException e) {
            pool = servicePoolMap.get(key);
            logger.warn("client pool NoSuchElementException exception : " + key
                    + ",active=" + pool.getNumActive() + ",idle="
                    + pool.getNumIdle());
            throw new RuntimeException(e);
        } catch (Exception e) {
            logger.warn("client pool other exception : " + key + "ex="
                    + e.getMessage());
            throw e;
        }

        if (logger.isDebugEnabled()) {
            logger.debug("pool-stat: alloc " + transport + ",active="
                + pool.getNumActive() + ",idle=" + pool.getNumIdle());
        }
        return transport;
    }

    @Override
    public void returnConnection(XoaTransport xoaTransport) throws Exception {
        String key = getNodeKey(xoaTransport.getNode());
        ObjectPool pool = null;
        try {
            pool = servicePoolMap.get(key);
            if (pool != null) {
                pool.returnObject(xoaTransport.getTransport());
                if (logger.isDebugEnabled()) {
                    logger.debug("pool-stat: dealloc "
                        + xoaTransport.getTransport() + ",active="
                        + pool.getNumActive() + ",idle=" + pool.getNumIdle());
                  }
            } else {
                if (logger.isDebugEnabled()) {
                    logger.debug("pool-stat: dealloc "
                        + xoaTransport.getTransport() + ", pool not exist.");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public void invalidateConnection(XoaTransport xoaTransport) {

        String key = getNodeKey(xoaTransport.getNode());
        TTransport trans = xoaTransport.getTransport();
        ObjectPool pool = null;
        try {
            pool = servicePoolMap.get(key);
            if (pool != null) {
                pool.invalidateObject(trans);
                if (logger.isDebugEnabled()) {
                    logger.debug("pool-stat: invalidate " + trans + ",active="
                        + pool.getNumActive() + ",idle=" + pool.getNumIdle());
                 }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void clearConnections(Node node) {
        ObjectPool pool = null;
        String key = getNodeKey(node);
        try {
            pool = servicePoolMap.get(key);
            if (pool != null) {
                pool.clear();
                // servicePoolMap.remove(key);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        logger.info("pool-stat: pool destruction " + key);
    }

    public int getConnTimeout() {
        return connTimeout;
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

}
