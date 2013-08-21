package com.renren.xcache.impl.memcachedclient;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.danga.MemCached.MemCachedClient;
import com.danga.MemCached.SockIOPool;
import com.renren.xcache.namespace.Namespace;
import com.renren.xcache.namespace.NamespaceFactory;
import com.renren.xcache.namespace.ZooKeeperBasedNamespaceFactory;
import com.renren.xcache.pool.CachePool;
import com.renren.xcache.pool.CachePoolFactory;
import com.renren.xcache.pool.Node;
import com.renren.xcache.pool.ZooKeeperBasedPoolFactory;
import com.renren.xcache.pool.CachePoolFactory.PoolConfigModifiedCallback;

/**
 * 负责{@link MemCachedClient}的创建
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-17 下午07:16:49
 */
public class MemcachedClientFactory {

    /**
     * 是否使用二进制协议，默认为true
     */
    private static final boolean USE_BIN_PROTOCOL = false;

    protected final Log logger = LogFactory.getLog(this.getClass());

    /**
     * {@link CachePoolFactory}
     */
    private CachePoolFactory poolFactory;

    /**
     * {@link NamespaceFactory}
     */
    private NamespaceFactory namespaceFactory;

    /**
     * 存储{@link MemCachedClient}实例，key为{@link CachePool}的name
     */
    private Map<String, MemCachedClient> clients = new ConcurrentHashMap<String, MemCachedClient>();

    /**
     * 存储{@link MemCachedClient}实例的版本号，每当{@link CachePool}的配置变更，这个版本号都要自增1。
     * 版本号的价值在于，当{@link CachePool}的配置变更时，需要重新构造一个{@link SockIOPool}，需要 在
     * {@link SockIOPool}的name中加入版本号以区分，同时通过旧的版本号获取老的{@link SockIOPool}实例，
     * 并销毁之。
     */
    private Map<String, AtomicInteger> clientVersions = new ConcurrentHashMap<String, AtomicInteger>();

    public MemcachedClientFactory() {
        init();
    }

    /**
     * 初始化
     */
    public void init() {
        namespaceFactory = ZooKeeperBasedNamespaceFactory.getInstance();
        poolFactory = ZooKeeperBasedPoolFactory.getInstance();
        poolFactory.addCallback(poolConfigModifiedCallback); //注册回调
    }

    /**
     * 当{@link CachePool}的配置变更时的回调逻辑
     */
    private PoolConfigModifiedCallback poolConfigModifiedCallback = new PoolConfigModifiedCallback() {

        @Override
        public void onPoolModified(CachePool newPool) {
            String poolName = newPool.getName();
            MemCachedClient oldClient = clients.get(poolName); //get old client
            if (oldClient != null) { //old client exists

                Object[] ret = initClient(newPool); //init a new client using the new CachePool config
                MemCachedClient newClient = (MemCachedClient) ret[0];
                if (logger.isInfoEnabled()) {
                    logger.info("Initialize a new client: " + newClient);
                }
                clients.put(poolName, newClient); //put to map

                String oldSockIOPoolName = (String) ret[1]; //get old SockIOPool name 
                if (logger.isInfoEnabled()) {
                    logger.info("Old client exists, shut it down. SockIOPool name: "
                            + oldSockIOPoolName + ", client: " + oldClient);
                }
                SockIOPool oldSockIOPool = SockIOPool.getInstance(oldSockIOPoolName); //get old SockIOPool
                if (oldSockIOPool != null) {
                    oldSockIOPool.shutDown(); //destroy old SockIOPool
                } else {
                    logger.error("SockIOPool not found: " + oldSockIOPoolName);
                }
            } else {
                if (logger.isDebugEnabled()) {
                    logger
                            .debug("client for pool '" + poolName
                                    + "' not exists, hence do nothing.");
                }
            }
        }
    };

    /**
     * 按照{@link Namespace}的名字来查找与之对应的{@link MemCachedClient}。
     * 
     * 对应关系是{@link Namespace}对应唯一的{@link CachePool}， 而
     * {@link MemCachedClient}和{@link CachePool}是一一对应关系。 所以代码逻辑是先根据
     * {@link Namespace}的名字来查找与之对应的{@link CachePool}， 在根据{@link CachePool}
     * 的名字来查找与之对应的{@link MemCachedClient}， 当前不存在的话就初始化一个。
     * 
     * @param namespace
     * @return
     */
    public MemCachedClient getClientByNamespace(String namespace) {

        CachePool pool = findPool(namespace); //find CachePool by namespace
        String poolName = pool.getName();
        MemCachedClient client = clients.get(poolName);
        if (client == null) { //client not exists for the pool
            synchronized (poolName.intern()) {
                client = clients.get(poolName);
                if (client == null) { //init a client
                    Object[] ret = initClient(pool);
                    client = (MemCachedClient) ret[0];
                    clients.put(poolName, client);
                }
            }
        }
        return client;
    }

    /**
     * 按照{@link Namespace}的名字来查找与之对应的CachePool
     * 
     * @param namespace
     * @return
     */
    private CachePool findPool(String namespace) {
        Namespace ns = namespaceFactory.getNamespace(namespace);
        if (ns == null) {
            throw new IllegalArgumentException("Namespace not exists: " + namespace);
        }
        String poolName = ns.getAssociatedPoolName();
        CachePool pool = poolFactory.getPool(poolName);
		if (pool == null) {
			throw new IllegalArgumentException("Pool not exists:" + poolName
					+ ", for namespace:" + namespace);
		}
        return pool;
    }

    /**
     * 初始化一个与指定的{@link CachePool}对应的{@link MemCachedClient}并返回， 同时返回之前一个版本的
     * {@link MemCachedClient}所使用的{@link SockIOPool}的name。
     * 
     * @param pool
     * @return 一个长度为2的Object[]，第0个元素是初始化完成的{@link MemCachedClient}实例，
     *         第1个元素是之前一个版本的{@link MemCachedClient}所使用的{@link SockIOPool}
     *         的name。
     */
    private Object[] initClient(CachePool pool) {

        if (logger.isDebugEnabled()) {
            logger.debug("Initialize pool:" + pool.getName());
        }

        List<Node> nodes = pool.getNodes();
        String[] servers = new String[nodes.size()];
        Integer[] weights = new Integer[nodes.size()];

        int offset = 0;
        for (Node node : nodes) {
            servers[offset] = node.getHostname() + ":" + node.getPort();
            weights[offset] = node.getWeight();
            offset++;
        }

        int initialConnections = 10;
        int minSpareConnections = 5;
        int maxSpareConnections = 100;
        long maxIdleTime = 1000 * 60 * 30; // 30 minutes
        long maxBusyTime = 1000 * 60 * 5; // 5 minutes
        long maintThreadSleep = 1000 * 5; // 5 seconds
        int socketTimeOut = 300; // 3 seconds to block on reads
        int socketConnectTO = 1000 * 3; // 3 seconds to block on initial connections.  If 0, then will use blocking connect (default)

        String xcachePoolName = pool.getName();

        AtomicInteger version = clientVersions.get(xcachePoolName);
        if (version == null) {
            version = new AtomicInteger(-1);
            clientVersions.put(xcachePoolName, version);
        }

        //construct old SockIOPool name
        String oldSockIOPoolName = xcachePoolName + "-v" + version.get();

        //construct new SockIOPool name
        String sockIOPoolName = xcachePoolName + "-v" + version.incrementAndGet();

        if (logger.isDebugEnabled()) {
            logger.debug("SockIOPool name: " + sockIOPoolName);
        }

        //get a new SockIOPool
        SockIOPool sockPool = SockIOPool.getInstance(sockIOPoolName);

        sockPool.setServers(servers);
        sockPool.setWeights(weights);
        sockPool.setInitConn(initialConnections);
        sockPool.setMinConn(minSpareConnections);
        sockPool.setMaxConn(maxSpareConnections);
        sockPool.setMaxIdle(maxIdleTime);
        sockPool.setMaxBusyTime(maxBusyTime);
        sockPool.setMaintSleep(maintThreadSleep);
        sockPool.setSocketTO(socketTimeOut);
        sockPool.setSocketConnectTO(socketConnectTO);
        sockPool.setNagle(false); // turn off Nagle's algorithm on all sockets in pool	
        sockPool.setHashingAlg(SockIOPool.CONSISTENT_HASH);
        sockPool.setAliveCheck(false); // disable health check of socket on checkout
        sockPool.initialize();

        MemCachedClient client = new MemCachedClient(sockIOPoolName, USE_BIN_PROTOCOL);
        client.setSanitizeKeys(false); //不对key进行编码
        return new Object[] { client, oldSockIOPoolName };
    }

}
