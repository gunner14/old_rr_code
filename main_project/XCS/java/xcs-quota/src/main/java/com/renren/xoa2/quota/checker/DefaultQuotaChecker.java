package com.renren.xoa2.quota.checker;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.datacenter.find.bean.QuotaInfo2;
import com.renren.ad.datacenter.find.bean.QuotaInfo2.QuotaLevel;
import com.renren.ad.datacenter.find.core.KDataParser;
import com.renren.xcs.Configuration;
import com.renren.xcs.DataChangeListener;
import com.renren.xcs.ServiceRegistryAccessor;
import com.renren.xcs.ZookeeperException;
import com.renren.xoa2.quota.cache.IQuotaCache;
import com.renren.xoa2.quota.cache.QuotaCacheConstants;
import com.renren.xoa2.quota.counter.DefaultQuotaCounter;
import com.renren.xoa2.quota.counter.QuotaCounter;
import com.renren.xoa2.quota.exception.QuotaCheckerException;
import com.renren.xoa2.quota.processor.ExceedProcessor;

/**
 * QuotaChecker的默认实现，load quota失败时会抛出QuotaCheckerException的RunTime异常
 * @author Xue Wenxin Aug 14, 2012 5:13:15 PM
 */
public class DefaultQuotaChecker implements QuotaChecker{
	
	private Log logger = LogFactory.getLog(DefaultQuotaChecker.class);

	private QuotaInfo2 quotaInfo;
	
	private String ip;
	
	private IQuotaCache cache;
	
	private ServiceRegistryAccessor accessor;
	
	private Configuration config;
	
	private ExceedProcessor processor;
	
	/**用于本地计数，防止频繁读写cache，这里只记录当前机器，也就是单台机器的某个level的调用次数*/
	private QuotaCounter counter;
	
	/** 用于刷ip的定时器*/
	private ScheduledThreadPoolExecutor timerExecutor = new ScheduledThreadPoolExecutor(2);
	
	public DefaultQuotaChecker(ServiceRegistryAccessor accessor, Configuration config, ExceedProcessor processor, IQuotaCache cache){
		this.accessor = accessor;
		this.config = config;
		this.processor = processor;
		this.cache = cache;
		loadQuotaInfo();
		counter = new DefaultQuotaCounter(cache);
		initIpCache();
		runTimer();
	}

	/**
	 * 用于向通用Cache中写入ip信息，以service+version+clientid为key<br>
	 * 设置过期时间为30分钟，有Timer每十分种重新写入，保证当该ip机器失效后，cache中信息的有效性<br>
	 * 所有异常信息将被cache，保证cache的任何异常不影响checker
	 */
	private void initIpCache(){
		ip = getLocalAddress();
		String ipKey = getIpKey(accessor.getService(),accessor.getVersion(), config.getClientUsername());
		try {
            cache.putIp(ipKey, ip, 30*QuotaCacheConstants.EXPIRE_MINUTE);
        } catch (Exception e) {
			logger.error("Register Checker ip failure!", e);
		}
	}
	
	/**
	 * 从zk cluster中version节点读取quota数据 
	 * @throws QuotaCheckerException 
	 */
	private void loadQuotaInfo(){
		
		QuotaListener listener = new QuotaListener();
		byte[] metaData =null;
		try {
			metaData = accessor.getVersionAndListenChange(listener);
			if(logger.isInfoEnabled()){
				logger.info("Accessor get Version data:"+metaData);
			}
		} catch (ZookeeperException e) {
			logger.error("Fail to Load Quota Info for service["+accessor.getService()+"],version["+accessor.getVersion()+"]");
			throw new QuotaCheckerException("Fail to Load Quota Info for service["+accessor.getService()+"],version["+accessor.getVersion()+"]", e);
		}
		parseQuota(metaData);
	}
	
	/**
	 * 解析从quota数据到本地缓存，也用于监听version节点数据变化后重新load quota数据
	 * @param metaData
	 */
	private void parseQuota(byte[] metaData){
		KDataParser parser = new KDataParser(metaData);
		String clientId = config.getClientUsername();
		quotaInfo = parser.getQuota2(clientId);
		if(logger.isInfoEnabled()){
			logger.info("quota info of this checker:"+quotaInfo);
		}
		
	}
	
	/**
	 * 定时任务，十分钟执行一次，重新将ip写入cache，目的，让ip继续在cache
	 */
	private void runTimer(){
		timerExecutor.scheduleAtFixedRate(new Runnable() {
			
			public void run() {
				try {
					initIpCache();
				} catch (Throwable e) {
				    //TODO WARN
				}
			}
		}, 1, 600, TimeUnit.SECONDS);
		
	}
	
	@Override
	public boolean hasQuota(){
		boolean mResult = checkQuota(QuotaLevel.MIN);
		boolean hResult = checkQuota(QuotaLevel.HOUR);
		if(!(mResult && hResult)){
			processor.process();//&&heckQuota(QuotaLevel.MIN)&&heckQuota(QuotaLevel.SEC);
		}
		return true;
	}
	
	/**
	 * 检查是否超过level级别的配额(包括single和total)
	 * @param level
	 * @return true 有配额，false 没有配额
	 */
	private boolean checkQuota(QuotaLevel level){
		//TODO：对zk中没有配置quota信息的情况，先不做限制，待具体运维配置规范完整后再做调整
		if(quotaInfo == null){
			logger.warn("Quota of Clinet["+config.getClientUsername()+"] for Service["+accessor.getService()+","+accessor.getVersion()+"] is not set!");
			return true;
		}
		
		String totalKey = getTotalKey(accessor.getService(),accessor.getVersion(), config.getClientUsername());
		String singleKey = totalKey + "_" + ip;

		boolean totalResult = true;
		boolean singleResult = true;
		
		Integer totalQuota = quotaInfo.getTotalMap().get(level);
		Integer singleQuota = quotaInfo.getSingleMap().get(level);

		int singleCurrent = (int) counter.increase(level, singleKey);
		int totalCurrent = (int) counter.increase(level, totalKey);
		
		totalResult = totalQuota == null ? true : totalCurrent < totalQuota;
		singleResult = singleQuota == null ? true : singleCurrent < singleQuota;
		
		logger.info("total used:" + totalCurrent+",single used:"+ singleCurrent);
		return totalResult && singleResult;
	}
	
	/**
	 * quota信息相关的zk监听器，当quota信息变化时，由该监听器处理
	 * @author Xue Wenxin
	 */
	public class QuotaListener implements DataChangeListener{

		public void dataChanged(byte[] data) {
			parseQuota(data);
			logger.info("Quota Data Changed:"+quotaInfo);
		}
		
	}
	
	/** 生成获取ip信息的key */
	private String getIpKey(String service, int version, String clientId){
		return service+"_"+version+"_"+clientId;
	}
	/** 生成获取total key */
	private String getTotalKey(String service, int version, String clientId){
		return service+"_"+version+"_"+clientId;
	}
	
	public QuotaInfo2 getQuotaInfo() {
		return quotaInfo;
	}


	/** 获取本地ip*/
	private final String getLocalAddress(){
		try{
			for(Enumeration<NetworkInterface> ni =NetworkInterface.getNetworkInterfaces();ni.hasMoreElements();){
				NetworkInterface eth=ni.nextElement();
				for(Enumeration<InetAddress> add = eth.getInetAddresses();add.hasMoreElements();){
					InetAddress i=add.nextElement();
					if(i instanceof Inet4Address){
						if(i.isSiteLocalAddress()){
							return i.getHostAddress();
						}
					}
				}
			}
		}catch(SocketException e){
			logger.warn("get local ip address failed:"+e.getMessage());
		}
		//防止获取失败，用checker的hashcode代替，会过期
		return this.hashCode()+"";
	}

	@Override
	public String toString() {
		return "DefaultQuotaChecker [quotaInfo=" + quotaInfo + ", ip=" + ip + "]";
	}

	public String getCurrent(QuotaLevel level){
		String totalKey = getTotalKey(accessor.getService(),accessor.getVersion(), quotaInfo.getClientId());
		String singleKey = totalKey + "_" + ip;
		return "total used:" + counter.getCurrent(level, totalKey)+",single used:"+counter.getCurrent(level, singleKey);
	}



}


