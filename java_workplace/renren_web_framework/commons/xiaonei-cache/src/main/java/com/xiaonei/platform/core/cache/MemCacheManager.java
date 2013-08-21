package com.xiaonei.platform.core.cache;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.Collection;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicInteger;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import net.spy.memcached.CASValue;
import net.spy.memcached.ConnectionFactory;
import net.spy.memcached.MemcachedClient;
import net.spy.memcached.MemcachedNode;
import net.spy.memcached.NodeLocator;
import net.spy.memcached.OperationTimeoutException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.xce.log.TimeCost;

public class MemCacheManager{
	
	public static final int MULTI_CLIENTS_COUNT = 20;
	
	private static Log logger = LogFactory.getLog(MemCacheManager.class);
	
	private static String loadConfigSql = "select * from memconfig2 where pool_name=?";
	private MemcachedClient[] clients;
	private ConnectionFactory connFactory;
	private String poolName;
	private int version;
	private long timeout;
	private List<InetSocketAddress> servers;
	
	private AtomicInteger count = new AtomicInteger();
	
	public MemCacheManager(String poolName){
		connFactory = new MemCacheConnectionFactory();
		this.poolName = poolName;
		this.timeout = connFactory.getOperationTimeout();
	}
	
	public String getPoolVersion(){
		return poolName+"-"+version;
	}
	
    private void initCheck() {
        if (clients == null) throw new RuntimeException("client not init");
    }
	
	private MemcachedClient getClient() {
		if (count.get() > Integer.MAX_VALUE - 9000000) {
			count.set(0);
		}
		return clients[count.incrementAndGet() % clients.length];
	}
	
	public Object get( String key ) {
		TimeCost timeCost = TimeCost.begin("memcached-get");
		initCheck();
		Future<Object> f = getClient().asyncGet(key);
		timeCost.endStep("submit-asyncGet");
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted waiting for value, pool=" + poolName + ", key=" + key, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Exception waiting for value, pool=" + poolName + ", key=" + key, e);
		} catch (TimeoutException e) {
			throw new OperationTimeoutException("Timeout waiting for value, pool=" + poolName + ", key=" + key, e);
		} finally {
			timeCost.endFinally();
		}
	}
	
	public CASValue<Object> gets(String key) {
		initCheck();
		Future<CASValue<Object>> f = getClient().asyncGets(key);
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted waiting for value, pool=" + poolName + ", key=" + key, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Exception waiting for value, pool=" + poolName + ", key=" + key, e);
		} catch (TimeoutException e) {
			 
			throw new OperationTimeoutException("Timeout waiting for value, pool=" + poolName + ", key=" + key, e);
		}
	}
	/**
	 * 异步获取
	 * @param key
	 * @return
	 */
	public Future<Object> asyncGet(String key){
		initCheck();
		return getClient().asyncGet(key);
	}
	
	/**
	 * 批量获取，同步获取，1秒超时
	 * get key1 key2 key3 ....
	 * @param keys
	 * @return
	 */
	public Map<String, Object> getBulk(Collection<String> keys) {
		TimeCost timeCost = TimeCost.begin("memcached-getBulk");
		initCheck();
		Future<Map<String,Object>> f = getClient().asyncGetBulk(keys);
		timeCost.endStep("submit-asyncGetBulk");
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted getting bulk values, pool=" + poolName, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Failed getting bulk values, pool=" + poolName, e);
		} catch (TimeoutException e) {
			 
			throw new OperationTimeoutException("Timeout waiting for bulkvalues, pool=" + poolName, e);
		} finally {
			timeCost.endFinally();
		}
	}
	public Map<String, Object> getBulk(String... keys) {
		TimeCost timeCost = TimeCost.begin("memcached-getBulk2");
		initCheck();
		Future<Map<String,Object>> f = getClient().asyncGetBulk(keys);
		timeCost.endStep("submit-asyncGetBulk");
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted getting bulk values, pool=" + poolName, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Failed getting bulk values, pool=" + poolName, e);
		} catch (TimeoutException e) {
			 
			throw new OperationTimeoutException("Timeout waiting for bulkvalues, pool=" + poolName, e);
		} finally {
			timeCost.endFinally();
		}
	}
	/**
	 * 批量获取，异步获取
	 * @param keys
	 * @return
	 */
	public Future<Map<String, Object>> asyncGetBulk(String... keys) {
		initCheck();
		return getClient().asyncGetBulk(keys);
	}
	
	public Future<Map<String, Object>> asyncGetBulk(Collection<String> keys) {
		initCheck();
		return getClient().asyncGetBulk(keys);
	}
	
	/**
	 * 更新配置信息
	 * 
	 * @param atTime
	 */
	public void update(String atTime) {
		if (logger.isInfoEnabled()) {
			logger.info("Start Update " + poolName + " Pool");
		}

		List<InetSocketAddress> nServers = loadConfig(poolName);
		if (nServers == null || nServers.size() == 0)
			return;
		Object[] arrNew = nServers.toArray();
		Object[] arrOld = servers.toArray();

		if (!Arrays.equals(arrNew, arrOld)) {
			
		    MemcachedClient[] oldClients = this.clients;
		    
		    version++;
            initPool(nServers, poolName);
            if (logger.isInfoEnabled()) {
                logger.info("End Update " + poolName + " Pool@" + atTime);
            }
		    
		    for (int i = 0; i < oldClients.length; i++) {
				try {
				    oldClients[i].shutdown();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			
		} else {
			if (logger.isInfoEnabled()) {
				logger.info("End Update " + poolName + " Pool,But No Change");
			}
		}

	}

	/**
	 * 检查pool是否完成初始化
	 */
	public void checkPoolInit() {
		if (logger.isInfoEnabled()) {
			logger.info(poolName + " pool version=" + getPoolVersion());
		}
		if (clients != null) {
			if (logger.isInfoEnabled()) {
				logger.info("MemCached Pool " + poolName + " Is Initialized");
			}
		} else {
			initPool(poolName, false);
			logger.warn("MemCached Pool " + poolName + " Is ReInit");
		}
	}
	/**
	 * 初始化pool
	 * @param poolName
	 * @param incVersion
	 */
	public void initPool(String poolName,boolean incVersion){
		List<InetSocketAddress> allServers = loadConfig(poolName);
		if(incVersion)this.version++;
		initPool(allServers, poolName);
	}
	
	public void initPool(List<InetSocketAddress> servers, String poolName) {
		this.servers = servers;
		if (servers == null || servers.size() == 0) {
			throw new RuntimeException("Memcached " + poolName
					+ " pool init fail,config list is null");
		}
		MemcachedClient[] tmpClients = new MemcachedClient[MULTI_CLIENTS_COUNT];
		
    	for (int i = 0; i < tmpClients.length; i++) {
    	    try {
    	        tmpClients[i] = new MemcachedClient(connFactory, servers);
    	    } catch (IOException ex) {
                ex.printStackTrace();
            }
    	}
    	this.clients = tmpClients;
	}
	
	public void shutdown(){
		for (int i = 0; i < clients.length; i++) {
		    try {
		        clients[i].shutdown();
            } catch (Exception e) {
                e.printStackTrace();
            }
		}
	}
	/**
	 * 从数据库中加载缓存的配置
	 * @param poolName
	 * @return
	 */
	@SuppressWarnings("unchecked")
	private List<InetSocketAddress> loadConfig(final String poolName){
		OpList op = new OpList(loadConfigSql,DataAccessMgr.BIZ_STAT){
			public void setParam(PreparedStatement ps) throws SQLException{
				ps.setString(1, poolName);
			}
			public Object parse(ResultSet rs) throws SQLException{
				return new MemCacheConfig(rs.getString("pool_name"),rs.getString("server_name"),rs.getInt("server_port"),rs.getInt("connection_num"));
			}
		};
		
		List<MemCacheConfig> configs = null;
		try {
			configs = DataAccessMgr.getInstance().queryList(op);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		if(configs==null||configs.size()==0)return null;
		List<InetSocketAddress> addrs = new LinkedList<InetSocketAddress>();
		
		for(int i=0;i<configs.size();i++){
			MemCacheConfig config = configs.get(i);
			addrs.add(new InetSocketAddress(config.hostName,config.port));
		}
		
		return addrs;
	}
	
	/**
	 * 获取key散列到的机器名字(方便调试)
	 * @param key
	 * @return
	 */
	public String getHost(String key){
		NodeLocator locator = getClient().getNodeLocator();
		MemcachedNode primary = locator.getPrimary(key);
		if(primary!=null)
			return primary.toString();
		return null;
	}
	
	@Deprecated
	public boolean set( String key, Object value, Date expiry ) {
		initCheck();
		if(expiry==null)expiry = new Date(0);
		int expiryTime = (int)expiry.getTime();
		
		Future<Boolean> f = getClient().set(key,expiryTime,value);
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted set value, pool=" + poolName + ", key=" + key, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Failed set value, pool=" + poolName + ", key=" + key, e);
		} catch (TimeoutException e) {
			 
			throw new OperationTimeoutException("Timeout waiting for set value, pool=" + poolName + ", key=" + key, e);
		}
	}
	
	public Future<Boolean> asyncSet(String key,Object value,int exp){
		initCheck();
		return getClient().set(key, exp, value);
	}
	
	public boolean set(String key,Object value,int expiry){
		initCheck();
		Future<Boolean> f = getClient().set(key, expiry, value);
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted set value, pool=" + poolName + ", key=" + key, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Failed set value, pool=" + poolName + ", key=" + key, e);
		} catch (TimeoutException e) {
			 
			throw new OperationTimeoutException("Timeout waiting for set value, pool=" + poolName + ", key=" + key, e);
		}
	}
	
	public boolean replace(String key,Object value){
		initCheck();
		Future<Boolean> f = getClient().replace(key,0, value);
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted replace value, pool=" + poolName + ", key=" + key, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Failed replace value, pool=" + poolName + ", key=" + key, e);
		} catch (TimeoutException e) {
			 
			throw new OperationTimeoutException("Timeout waiting for replace value, pool=" + poolName + ", key=" + key, e);
		}
	}
	
	public Future<Boolean> asyncReplace(String key,Object value){
		initCheck();
		return getClient().replace(key, 0, value);
	}
	
	public boolean set( String key, Object value ) {
		return set(key,value,0);
	}
	
	/**
	 * 为兼容老接口，不推荐使用
	 * @param key
	 * @param value
	 * @param date
	 * @return
	 */
	@Deprecated
	public boolean add( String key, Object value, Date date ) {
		int exp = (int)date.getTime();
		return add(key,value,exp);
	}
	
	/**
	 * 异步添加
	 * @param key 键值
	 * @param o	存储的对象
	 * @param exp 过期时间，相对时间
	 * @return
	 */
	public Future<Boolean> asyncAdd(String key, Object o,int exp ) {
		initCheck();
		return getClient().add(key, exp, o);
	}
	
	/**
	 * 相对asyncAdd,一个同步实现
	 * @param key
	 * @param o
	 * @param exp
	 * @return
	 */
	public boolean add(String key,Object o,int exp){
		initCheck();
		Future<Boolean> f = asyncAdd(key,o,exp);
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted add value, pool=" + poolName + ", key=" + key, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Failed add value, pool=" + poolName + ", key=" + key, e);
		} catch (TimeoutException e) {
			 
			throw new OperationTimeoutException("Timeout waiting for add value, pool=" + poolName + ", key=" + key, e);
		}
	}
	
	/**
	 * 同步删除
	 * @param key
	 * @return
	 */
	public boolean delete( String key ) {
		initCheck();
		Future<Boolean> f= getClient().delete(key);
		try {
			return f.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException("Interrupted delete value, pool=" + poolName + ", key=" + key, e);
		} catch (ExecutionException e) {
			throw new RuntimeException("Failed delete value, pool=" + poolName + ", key=" + key, e);
		} catch (TimeoutException e) {
			 
			throw new OperationTimeoutException("Timeout waiting for delete value, pool=" + poolName + ", key=" + key, e);
		}
	}
	/**
	 * 异步删除
	 * @param key
	 * @return
	 */
	public Future<Boolean> asyncDelete(String key){
		initCheck();
		return getClient().delete(key);
	}
	/**
	 * 自增，通过CAS实现，同步
	 * @param key
	 * @param inc 自增量
	 * @return
	 */
	public long incr(String key ,long inc){
		return getClient().incr(key, (int)inc);
	}
	
	/**
	 * 自增
	 * @param key
	 * @param inc 自增量
	 * @param exp 过期时间
	 * @return
	 */
	public long incr(String key , long inc, int exp) {
		return getClient().incr(key, (int)inc, 0, exp);
	}
		
	public long incr(String key){
		return getClient().incr(key, 1);
	}
	
	public long decr(String key){
		return getClient().decr(key, 1);
	}
	
	public long decr(String key ,long inc){
		return getClient().decr(key, (int)inc);
	}
	
	/**
	 * @deprecated
	 * @param key
	 * @return
	 */
	public boolean keyExists(String key){
		if(get(key)!=null)return true;
		return false;
	}
	
	public MemcachedClient getMemcachedClient(){
		return getClient();
	}
}