package com.xiaonei.login.logic;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;

/**
 * <p> loginConfMap是从db读取过来的，有一个定时任务5分钟自动加载至loginConfMap中,
 * <p> 此类是一个单列类
 * 
 * @author wen.he1
 *
 */

public class LoginConfigureImpl implements ILoginConfigure{
	
	private static final ScheduledExecutorService scheduled = Executors.newScheduledThreadPool(1);
	
	private static Map<String, String> loginConfigures = new HashMap<String, String>(8);
	
	private volatile static boolean schduledRunning = false;
	
	private final static ReentrantLock lock   = new ReentrantLock();
	
	private static ILoginConfigure _instance  = new LoginConfigureImpl();
	
	private static Log logger = LogFactory.getLog(LoginConfigureImpl.class);
	
	public static ILoginConfigure getInstance(){
		return _instance;
	}
	
	private LoginConfigureImpl() {
		scheduleCreateOnlyOneTime();
	}
	
	@Override
	public Map<String, String> getLoginConfigure() {
		return loginConfigures;
	}
	
	/**
	 * 每隔5分钟定时加载数据库数据,loadConfig
	 */
    private void scheduleCreateOnlyOneTime() {
        if (schduledRunning) {
            return;
        }
        if (lock.isLocked()) {
            return;
        }
        lock.lock();
        try {
            if (schduledRunning) {
                return;
            }
            scheduled.scheduleWithFixedDelay(new Runnable() {

                @Override
                public void run() {
                    try {
                        //增加加载数据的实现
                    	initCache();
                    } catch (final Throwable e) {
                    	logger.error("LoginConfigureImpl->scheduleCreateOnlyOneTime调用定时任务出错了！", e);
                    }

                }
            }, 0, 5, TimeUnit.MINUTES);
            schduledRunning = true;
        } finally {
            lock.unlock();
        }
    }
    
    @SuppressWarnings("unchecked")
	private void initCache(){
    	
		class Configure{
			String key;
			String value;
		}
		
		OpList  op = new OpList(LoginConfigureDao.selectLoginConfigureSQL, LoginConfigureDao.DATA_SOURCE){
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				
			}
			
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				Configure configure = new Configure();
				configure.key   = rs.getString("config_key");
				configure.value = rs.getString("config_value");
				return configure;
			}
		};
		
		Map<String,String> result = new HashMap<String,String>(8);
		try {
			List<Configure>    list   = (List<Configure>)DataAccessMgr.getInstance().queryList(op);
			for (Configure configure : list) {
				result.put(configure.key, configure.value);
			}
			
			synchronized (loginConfigures) {
				loginConfigures = result;
			}
			
		} catch (SQLException e) {
			logger.error("LoginConfigureImpl->initCache访问数据库出错！", e);
		}
		
		
	}
    
}
