package com.xiaonei.platform.core.opt.base.wrapper;

import java.sql.SQLException;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.opt.dao.NativeIpDAO;
import com.xiaonei.platform.core.schedule.ScheduleService;

/**
 * 内网IP白名单
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-7 下午02:38:31
 */
public class NativeIpWhiteList {

	private static final String NATIVE_IPS = "127.0.0.*,60.28.212.*,60.29.242.*,123.125.40.*,123.125.44.*,123.125.45.*,123.125.46.*,123.125.47.*,123.125.36.*,123.125.37.*,123.125.38.*,220.181.92.*,10.22.200.*,10.22.206.*,10.3.16.*,10.3.17.*,10.3.18.*,10.3.19.*,10.3.20.*,10.3.21.*,10.3.22.*,10.7.16.*,111.13.4.*,10.9.16.*,119.254.22.*";
	
	private Log logger = LogFactory.getLog(this.getClass());

    private ScheduledExecutorService timer = ScheduleService.getService();
    
    private static final NativeIpWhiteList instance = new NativeIpWhiteList();
    
    public static NativeIpWhiteList getInstance() {
    	return instance;
    }
    
    /**
     * 内部IP的set
     */
    private Set<String> nativeIpSet = new HashSet<String>();
	
    private NativeIpWhiteList() {
    	initNativeIps(NATIVE_IPS);
    }
    
    public boolean isNativeIp(String ipPattern) {
    	
    	if (ipPattern.startsWith("10.")) {	//10.*.*.*都是内网IP
    		return true;
    	}
    	return nativeIpSet.contains(ipPattern);
    }
    
    /**
     * 初始化IP列表
     * @param ips
     */
    private  void initNativeIps(String ips) {
    	
    	//先从本地初始化，防止从DB初始化时造成的延迟以及初始化失败时产生的错误
		String[] ss = NATIVE_IPS.split(",");
		for (String ip : ss) {
			nativeIpSet.add(ip);
		}
		
		//启动reload的定时任务，启动后会立即reload一次
		startReloader();
	}

    private void startReloader() {
    	
    	if (logger.isInfoEnabled()) {
    		logger.info("starting ip reloader...");
    	}
    	
    	timer.scheduleWithFixedDelay(new Runnable() {
			@Override
			public void run() {
				try {
					if (logger.isInfoEnabled()) {
			    		logger.info("ip reload task running");
			    	}
					loadFromDB();
				} catch (Throwable e) {
					e.printStackTrace();
				}
			}
		}, 0, 60 * 30, TimeUnit.SECONDS);
    }
    
    private void loadFromDB() {
    	List<String> ips;
		try {
			ips = NativeIpDAO.getInstance().getIps();
			if (ips != null) {
				if (logger.isInfoEnabled()) {
		    		logger.info(ips.size() + "ips reloaded from DB.");
		    	}
				
				Set<String> tmp = new HashSet<String>();
				for (String ip : ips) {
					tmp.add(ip);
				}
				nativeIpSet = tmp;
	    	}
		} catch (SQLException e) {
			e.printStackTrace();
		}
    }
    
    public static void main(String[] args) {
    	boolean ret = NativeIpWhiteList.getInstance().isNativeIp("10.3.16.8");
    	System.out.println(ret);
	}
    
}
