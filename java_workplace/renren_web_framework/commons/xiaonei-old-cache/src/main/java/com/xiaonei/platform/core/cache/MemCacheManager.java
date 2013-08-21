package com.xiaonei.platform.core.cache;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicInteger;

import net.spy.memcached.CASValue;
import net.spy.memcached.ConnectionFactory;
import net.spy.memcached.KetamaConnectionFactory;
import net.spy.memcached.MemcachedClient;
import net.spy.memcached.MemcachedNode;
import net.spy.memcached.NodeLocator;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;

public class MemCacheManager{
	private static String loadConfigSql = "select * from memconfig2 where pool_name=?";
	private AtomicInteger nextClient = new AtomicInteger(0);
	private ArrayList<MemcachedClient> clientPool; 
	private ArrayList<MemcachedClient> lastPool;
	private int clientSize = 4;
	private volatile boolean isInited = false;
	private volatile boolean isUpdating = false;
	private ConnectionFactory connFactory;
	private String poolName;
	private int version;
	private List<InetSocketAddress> servers;
	private Future<Boolean> nullBoolFuture = new NullFuture<Boolean>(Boolean.FALSE);
	private Future<Object> nullObjectFuture =new NullFuture<Object>(null);
	private Future<Map<String,Object>> nullMapFuture = new NullFuture<Map<String,Object>>(new HashMap<String,Object>());
	
	public MemCacheManager(String poolName){
		clientPool = new ArrayList<MemcachedClient>();
		lastPool = clientPool;
		connFactory = new KetamaConnectionFactory();
		this.poolName = poolName;
	}
	
	public String getPoolVersion(){
		return poolName+"-"+version;
	}
	
	public Object get( String key ) {
		MemcachedClient client = getValidClient();
		if(client==null)return null;
		return client.get(key);
	}
	
	public CASValue<Object> gets(String key) {
		MemcachedClient client = getValidClient();
		if(client==null)return null;
		return client.gets(key);
	}
	
	private ArrayList<MemcachedClient> getCurrentPool(){
		return isUpdating ? lastPool : clientPool; 
	}
	
	private MemcachedClient getValidClient(){
		ArrayList<MemcachedClient> pool = getCurrentPool();
		
		if(pool.size()==0)return null;
		int rr = nextClient.getAndIncrement()%pool.size();
		return pool.get(rr);
	}
	
	public int getValidClientCount(){
		return getCurrentPool().size();
	}
	
	public int getCurrentNext(){
		return nextClient.get();
	}
	/**
	 * 异步获取
	 * @param key
	 * @return
	 */
	public Future<Object> asyncGet(String key){
		MemcachedClient client = getValidClient();
		if(client==null)return nullObjectFuture;
		return client.asyncGet(key);
	}
	/**
	 * 批量获取，同步获取，1秒超时
	 * get key1 key2 key3 ....
	 * @param keys
	 * @return
	 */
	public Map<String, Object> getBulk(Collection<String> keys) {
		MemcachedClient client = getValidClient();
		if(client==null)return null;
		return client.getBulk(keys);
	}
	
	public Map<String, Object> getBulk(String... keys) {
		MemcachedClient client = getValidClient();
		if(client==null)return null;
		return client.getBulk(keys);
	}
	/**
	 * 批量获取，异步获取
	 * @param keys
	 * @return
	 */
	public Future<Map<String, Object>> asyncGetBulk(String... keys) {
		MemcachedClient client = getValidClient();
		if(client==null)return (Future<Map<String,Object>>)nullMapFuture;
		return client.asyncGetBulk(keys);
	}
	
	public Future<Map<String, Object>> asyncGetBulk(Collection<String> keys) {
		MemcachedClient client = getValidClient();
		if(client==null)return (Future<Map<String,Object>>)nullMapFuture;
		return client.asyncGetBulk(keys);
	}
	
	/**
	 * 更新配置信息
	 * @param atTime
	 */
	public void update(String atTime){
		List<InetSocketAddress> nServers = loadConfig(poolName);
		if(nServers==null||nServers.size()==0)return;
		Object[] arrNew = nServers.toArray();
		Object[] arrOld = servers.toArray();
		
		if(!Arrays.equals(arrNew,arrOld)){
			System.out.println("Start Update "+poolName+" Pool");
			isUpdating = true;
			version++;
			clientPool = new ArrayList<MemcachedClient>();
			initPool(nServers,poolName);
			isUpdating = false;
			shutdown();
			lastPool = clientPool;
			System.out.println("End Update "+poolName+" Pool@"+atTime);
		}else{
			System.out.println("End Update "+poolName+" Pool,But No Change");
		}
	}
	/**
	 * 检查pool是否完成初始化
	 */
	public void checkPoolInit(){
		if(!isInited)return;
		System.out.println("check pool "+poolName+" pool version="+getPoolVersion());
		
		if(getCurrentPool().size()>0){
			System.out.println("MemCached Pool "+poolName+" Is Already Initialized");
			return;
		}
		
		initPool(poolName,false);
		System.out.println("MemCached Pool "+poolName+" Is ReInit");
	}
	/**
	 * 初始化pool
	 * @param poolName
	 * @param incVersion
	 */
	public void initPool(String poolName,boolean incVersion){
		List<InetSocketAddress> allServers = loadConfig(poolName);
		if(incVersion)this.version++;
		initPool(allServers,poolName);
	}
	
	public void initPool(List<InetSocketAddress> servers,String poolName){
		this.servers = servers;
		if(servers==null || servers.size()==0){
			System.out.println("config error,get null config list from "+poolName+" pool");
			return;
		}
		
		for(int i=0;i < clientSize;i++){
			try{
				MemcachedClient c =  new MemcachedClient(connFactory,servers);
				clientPool.add(c);
			}catch(IOException ex){
				ex.printStackTrace();
			}
		}
		
		isInited = true;
	}
	
	public void shutdown(){
		System.out.println("shutdown pool "+this.poolName);
		for(int i=0;i<lastPool.size();i++){
			MemcachedClient client = lastPool.get(i);
			if(client!=null)client.shutdown();
		}
		lastPool = null;
	}
	/**
	 * 从数据库中加载缓存的配置
	 * @param poolName
	 * @return
	 */
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
		MemcachedClient client = getValidClient();
		NodeLocator locator = client.getNodeLocator();
		MemcachedNode primary = locator.getPrimary(key);
		if(primary!=null)
			return primary.toString();
		return null;
	}
	
	@Deprecated
	public boolean set( String key, Object value, Date expiry ) {
		MemcachedClient client = getValidClient();
		if(client==null)return false;
		if(expiry==null)expiry = new Date(0);
		int expiryTime = (int)expiry.getTime();
		
		Future<Boolean> f = client.set(key,expiryTime,value);
		Boolean ok = false;
		try {
			ok = f.get();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		return ok;
	}
	
	public Future<Boolean> asyncSet(String key,Object value,int exp){
		MemcachedClient client = getValidClient();
		if(client==null)return nullBoolFuture;
		return client.set(key, exp, value);
	}
	
	public boolean set(String key,Object value,int expiry){
		MemcachedClient client = getValidClient();
		if(client==null)return false;
		Future<Boolean> f = client.set(key, expiry, value);
		Boolean ok = false;
		try {
			ok = f.get();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		return ok;
	}
	
	public boolean replace(String key,Object value){
		MemcachedClient client = getValidClient();
		if(client==null)return false;
		Future<Boolean> f = client.replace(key,0, value);
		Boolean ok = false;
		try {
			ok = f.get();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		return ok;
	}
	
	public Future<Boolean> asyncReplace(String key,Object value){
		MemcachedClient client = getValidClient();
		if(client==null)return nullBoolFuture;
		return client.replace(key, 0, value);
	}
	
	public boolean set( String key, Object value ) {
		MemcachedClient client = getValidClient();
		if(client==null)return false;
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
		MemcachedClient client = getValidClient();
		if(client==null)return false;
		Boolean ok = false;
		int exp = (int)date.getTime();
		Future<Boolean> f = client.add(key, exp, value);
		try {
			ok = f.get();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		return ok;
	}	
	/**
	 * 异步添加
	 * @param key 键值
	 * @param o	存储的对象
	 * @param exp 过期时间，相对时间
	 * @return
	 */
	public Future<Boolean> asyncAdd(String key, Object o,int exp ) {
		MemcachedClient client = getValidClient();
		if(client==null)return nullBoolFuture;
		return client.add(key, exp, o);
	}
	/**
	 * 相对asyncAdd,一个同步实现
	 * @param key
	 * @param o
	 * @param exp
	 * @return
	 */
	public boolean add(String key,Object o,int exp){
		MemcachedClient client = getValidClient();
		if(client==null)return false;
		Future<Boolean> f = asyncAdd(key,o,exp);
		try{
			return f.get();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		return false;
	}
	/**
	 * 同步删除
	 * @param key
	 * @return
	 */
	public boolean delete( String key ) {
		MemcachedClient client = getValidClient();
		if(client==null)return false;
		Future<Boolean> f= client.delete(key);
		Boolean ok = false;
		try {
			ok = f.get();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		return ok;
	}
	/**
	 * 异步删除
	 * @param key
	 * @return
	 */
	public Future<Boolean> asyncDelete(String key){
		MemcachedClient client = getValidClient();
		if(client==null)return nullBoolFuture;
		return client.delete(key);
	}
	/**
	 * 自增，通过CAS实现，同步
	 * @param key
	 * @param inc 自增量
	 * @return
	 */
	public long incr(String key ,long inc){
		MemcachedClient client = getValidClient();
		return client.incr(key, (int)inc);
	}
	
	public long incr(String key){
		MemcachedClient client = getValidClient();
		return client.incr(key, 1);
	}
	
	public long decr(String key){
		MemcachedClient client = getValidClient();
		return client.decr(key, 1);
	}
	
	public long decr(String key ,long inc){
		MemcachedClient client = getValidClient();
		return client.decr(key, (int)inc);
	}
	
	public boolean keyExists(String key){
		if(get(key)!=null)return true;
		return false;
	}
	
	static class NullFuture<T> implements Future<T>{
		T nullObject;
		public NullFuture(T o){
			nullObject = o;
		}
		public boolean cancel(boolean mayInterruptIfRunning) {
			return true;
		}
		public T get() throws InterruptedException,
				ExecutionException {
			return nullObject;
		}
		public T get(long timeout, TimeUnit unit)
				throws InterruptedException, ExecutionException,
				TimeoutException {
			return nullObject;
		}
		public boolean isCancelled() {
			return true;
		}
		public boolean isDone() {
			return true;
		}
	}
}