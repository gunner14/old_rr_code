package com.xiaonei.passport.configure.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;


import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.InitializingBean;

import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.ILoginConfigureDao;
import com.xiaonei.passport.configure.LoginConfigureFactory;
import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.model.LoginRegistration;

/**
 * 从缓存中获取需要的配置值,定时加载对应的数据
 * @author Administrator
 *
 */
public class LoginConfigureImpl implements ILoginConfigure, InitializingBean{
	private static final ScheduledExecutorService scheduled = Executors.newScheduledThreadPool(1);
	private static Map<Integer, LoginRegistration> loginRegistrations=new HashMap<Integer, LoginRegistration>();
	private static Map<String, List<String>> loginConfigures=new HashMap<String, List<String>>();//key=id+"&"+key
	private volatile static boolean schduledRunning = false;
	private final static ReentrantLock lock = new ReentrantLock();
	private static ILoginConfigureDao loginConfigureDao=LoginConfigureFactory.getLoginConfigureDaoInstance();
	private static ILoginConfigure _instance=new LoginConfigureImpl();
	private static Log logger = LogFactory.getLog(LoginConfigureImpl.class);
	
	private LoginConfigureImpl(){
		
	}
	public static ILoginConfigure getInstance(){
		return _instance;
	}

	@Override
	public LoginRegistration getLoginRegistrationById(int id) {
		scheduleCreateOnlyOneTime();
		if(loginRegistrations.containsKey(Integer.valueOf(id))){
			return loginRegistrations.get(id);
		}
		
		return null;
	}

	@Override
	public List<String> getLoginConfigure(int id, String key) {
		scheduleCreateOnlyOneTime();
		List<String> result=new ArrayList<String>();
		String parentKey=PARENT_LOGIN_REGISRATION_ID+SPLIT_SIGN+key;
		if(loginConfigures.containsKey(parentKey)){
			List<String> parent=loginConfigures.get(parentKey);
			for(String value: parent){
				result.add(value);
			}
		}
		List<String> values=loginConfigures.get(id+SPLIT_SIGN+key);
		if(values!=null&&id>0){
			for(String value: values){
				result.add(value);
			}
		}
		return result;
	}

	@Override
	public void afterPropertiesSet() throws Exception {
		scheduleCreateOnlyOneTime();
	}
	private void initCache(){
		try{
			List<LoginRegistration> loginRegistrationList=loginConfigureDao.findLoginRegistration();
			Map<String, List<String>> configureStrategys=loginConfigureDao.findLoginConfigure();	
			Map<Integer, LoginRegistration> loginRegistrationMap=new HashMap<Integer, LoginRegistration>();
			for(LoginRegistration lr: loginRegistrationList){
				loginRegistrationMap.put(lr.getId(), lr);
			}
			synchronized (loginRegistrations) {
				loginRegistrations=loginRegistrationMap;
			}
			synchronized (loginConfigures) {
				loginConfigures=configureStrategys;
			}
			if(logger.isDebugEnabled()){
				logger.debug("schedule task get loginConfigure list ..");
				String mapString = "";
				for(Entry<String, List<String>> item : configureStrategys.entrySet()){
					mapString += " key: "+ item.getKey() + ", value: " + item.getValue() + "; ";
				}
				logger.debug(mapString);
			}
		}catch(Exception e){
			e.printStackTrace();
		}
		
	}
	/**
	 * 定时加载缓存数据
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
                        e.printStackTrace();
                    }

                }
            }, 0, 2, TimeUnit.MINUTES);
            schduledRunning = true;
            try{
             	Thread.sleep(1000);       	
            }catch(Exception e){
            	e.printStackTrace();
            }
        } finally {
            lock.unlock();
        }
    }
	@Override
	public String getUniqLoginConfigure(int id, String key) {
		if(StringUtils.isEmpty(key)){
			return null;
		}
		List<String> list=this.getLoginConfigure(id, key);
		if(list!=null&&list.size()==1){
			return list.get(0);
		}
		logger.error("getUniqLoginConfigure get not only one value , the size is "+list.size()+". this id is "+id+"; the key is "+key);
		return null;
	}
	@Override
	public Integer getUniqLoginConfigureForInt(int id, String key) {
		String value=getUniqLoginConfigure(id, key);
		try{
			if(value!=null){
				return Integer.valueOf(value);
			}
			logger.error("getUniqLoginConfigureForInt can not convert to int , the value is "+value+". this id is "+id+"; the key is "+key);
		}catch(Exception e){
			logger.error("getUniqLoginConfigureForInt can not convert to int , the value is "+value+". this id is "+id+"; the key is "+key, e);
			return null;
		}
		return null;
	}
	@Override
	public List<String> getLoginConfigure(int loginRegistration,
			LoginConfigureKey key) {
		return getLoginConfigure(loginRegistration, key.getKey());
	}
	@Override
	public String getUniqLoginConfigure(int loginRegistration,
			LoginConfigureKey key) {
		return getUniqLoginConfigure(loginRegistration, key.getKey());
	}
	@Override
	public Integer getUniqLoginConfigureForInt(int loginRegistration,
			LoginConfigureKey key) {
		return getUniqLoginConfigureForInt(loginRegistration, key.getKey());
	}
	@Override
	public String getUniqLoginConfigure(int loginRegistration,
			LoginConfigureKey key, String defaultValue) {
		String value= getUniqLoginConfigure(loginRegistration, key.getKey());
		if(StringUtils.isEmpty(value)){
			value=defaultValue;
		}
		return value;
	}
	@Override
	public Integer getUniqLoginConfigureForInt(int loginRegistration,
			LoginConfigureKey key, Integer defaultValue) {
		Integer value=getUniqLoginConfigureForInt(loginRegistration, key.getKey());
		if(value==null){
			value=defaultValue;
		}
		return value;
	}
	@Override
	public Map<String, List<String>> findLoginConfigure() {
		return loginConfigureDao.findLoginConfigure();
	}
	public static void main(String[] args) {
		try{
			Integer id=Integer.valueOf(args[0]);
			String key =args[1];
			ILoginConfigure config=LoginConfigureFactory.getLoginConfigureInstance();
			List<String> result=config.getLoginConfigure(id, key);
			System.out.println("key:="+key);
			for(String value:result){
				System.out.println(value+" ");
			}
			System.out.println( );
		}catch(Exception e){
			e.printStackTrace();
		}
		
	}
}
