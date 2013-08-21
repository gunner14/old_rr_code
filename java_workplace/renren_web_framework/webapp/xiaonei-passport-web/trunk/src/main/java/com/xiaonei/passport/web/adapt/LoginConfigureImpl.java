package com.xiaonei.passport.web.adapt;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.passport.service.ILoginXoaConfigure;
import com.renren.xoa2.client.ServiceFactory;
import com.xiaonei.passport.constants.LoginConfigureKey;

public class LoginConfigureImpl implements com.renren.passport.service.ILoginConfigure{
	
	public static final String SPLIT_SIGN="&";
	public static final int PARENT_LOGIN_REGISRATION_ID=0;//对于任何一个key,如果所有的登陆都共享,可以直接配置id=0,key=key, value.则所有的登陆都会共享此value
	public static final int DEFAULT_LOGIN_REGISTRATION_ID=1;
	
	//这里用两个变量vmLoginConfigure 和 xoaLoginConfigure是为了线上能即时切换
	private com.renren.passport.service.ILoginConfigure xoaLoginConfigure=null;
	
	private static final ScheduledExecutorService scheduled = Executors.newScheduledThreadPool(1);
	private static Map<String, List<String>> loginConfigures=new HashMap<String, List<String>>();//key=id+"&"+key
	private volatile static boolean schduledRunning = false;
	private final static ReentrantLock lock = new ReentrantLock();
	private static com.renren.passport.service.ILoginConfigure _instance=new LoginConfigureImpl();
	private static Log logger = LogFactory.getLog(LoginConfigureImpl.class);
	
	private LoginConfigureImpl(){
		//scheduleCreateOnlyOneTime();
	}
	public static com.renren.passport.service.ILoginConfigure getInstance(){
		return _instance;
	}
	
	/**
	 * 此接口用来在XOA定义的接口外再做一层封装
	 * XOA只定义了最基本的服务Map<String, List<String>> findLoginConfigure();
	 * 后续可以考虑把这些比较具体的接口全部开放到XOA定义中
	 * @return
	 */
	public static LoginConfigureImpl getLocalInstance(){
		return (LoginConfigureImpl) getInstance();
	}

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


	private void initCache(){
		Map<String, List<String>> configureStrategys=this.findLoginConfigure();
		synchronized (loginConfigures) {
			loginConfigures=configureStrategys;
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
        } finally {
            lock.unlock();
        }
    }
	
	public String getUniqLoginConfigure(int id, String key) {
		if(StringUtils.isEmpty(key)){
			return null;
		}
		List<String> list=this.getLoginConfigure(id, key);
		if(list==null){
			return null;
		}
		if(list!=null&&list.size()==1){
			return list.get(0);
		}
		logger.error("getUniqLoginConfigure get not only one value , the size is "+list.size()+". this id is "+id+"; the key is "+key);
		return null;
	}
	
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
	
	public List<String> getLoginConfigure(int loginRegistration,
			LoginConfigureKey key) {
		return getLoginConfigure(loginRegistration, key.getKey());
	}
	
	public String getUniqLoginConfigure(int loginRegistration,
			LoginConfigureKey key) {
		return getUniqLoginConfigure(loginRegistration, key.getKey());
	}
	
	public Integer getUniqLoginConfigureForInt(int loginRegistration,
			LoginConfigureKey key) {
		return getUniqLoginConfigureForInt(loginRegistration, key.getKey());
	}
	
	public String getUniqLoginConfigure(int loginRegistration,
			LoginConfigureKey key, String defaultValue) {
		String value= getUniqLoginConfigure(loginRegistration, key.getKey());
		if(StringUtils.isEmpty(value)){
			value=defaultValue;
		}
		return value;
	}
	
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
		try{
			return getLoginConfigureService().findLoginConfigure();
		}catch(Exception e){
			//return this.vmLoginConfigure.findLoginConfigure();
			logger.error("findLoginConfigure failed...", e);
		}
		return new HashMap<String, List<String>>();
	}

	/**
	 * 获取xoa服务，由于xoa2.0也是测试阶段，所以做了一个开关，允许暂时使用jar包的方式调用登陆服务。
	 * 如果xoa服务初始化失败，也将使用jar包的方式保证服务可以使用。
	 * by shuguo: 最新精神--线上将只使用XOA，所以，。。。挂掉就挂掉了
	 * @return loginService服务
	 */
	private com.renren.passport.service.ILoginConfigure getLoginConfigureService() {
		//先判断当前需要何种形式，再去初始化，否则白做无用功，还会增加XOA端的连接数
		//String serviceMode=this.getUniqLoginConfigure(1, LoginConfigureKey.USE_VM_SERVICE);  这个值不能用这种方式调用。。会死循环滴！默认用jar包形式
		//String serviceMode = LoginConfigureFactory.getLoginConfigureInstance().getUniqLoginConfigure(1, LoginConfigureKey.USE_VM_SERVICE);
		//by shuguo: 按最新精神--先获得XOA服务，利用XOA服务去取数据库里的配置，这个配置值原则上应该就是写死的2（only-xoa），但为了。。。还是判断一下，做个分支吧
		if(xoaLoginConfigure==null){
			try {
				xoaLoginConfigure = ServiceFactory.getService(ILoginXoaConfigure.class);
			} catch (Exception e) {
				logger.error("init xoaLoginConfigure with ONLY_XOA_MODE failed...", e);
			}
		}
		
		return xoaLoginConfigure;
	}
}
