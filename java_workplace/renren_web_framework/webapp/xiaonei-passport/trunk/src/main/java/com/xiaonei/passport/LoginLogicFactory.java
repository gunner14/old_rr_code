/**
 * 
 */
package com.xiaonei.passport;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import org.springframework.beans.factory.BeanFactoryUtils;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.task.service.AppBonusLogic;
import com.xiaonei.passport.task.service.PassportUserScore;
import com.xiaonei.passport.task.service.impl.AppBonusLogicImpl;
import com.xiaonei.passport.task.service.impl.BabordIPsManager;
import com.xiaonei.passport.task.service.impl.PassportUserScoreImpl;
import com.xiaonei.passport.task.service.impl.RRFarmLogicImpl;




/**
 * LoginLogicFactory <br>
 * 
 * @author tai.wang@opi-corp.com Mar 2, 2010 - 10:22:22 AM
 */
@Service
public class LoginLogicFactory {
    
    private final static Map<Class<?>,Object> map = new ConcurrentHashMap<Class<?>, Object>();

    private static ApplicationContext applicationContext;
    
    static {
    	applicationContext=new ClassPathXmlApplicationContext("applicationContext.xml");
    }
    
    public static BabordIPsManager getBabordIPsManager() {
        return getT(BabordIPsManager.class);
    }
    public static PassportUserScore getPassportUserScore() {
        return getT(PassportUserScoreImpl.class);
    }
    
    /**
     * getAppBonusLogic<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Mar 11, 2010 - 5:06:39 PM
     */
    public static AppBonusLogic getAppBonusLogic() {
        return AppBonusLogicImpl.getInstance();
    }
    
    public static RRFarmLogicImpl getRRFarmLogicImpl() {
        return getT(RRFarmLogicImpl.class);
    }
    
    @SuppressWarnings("unchecked")
    private static <T> T getT(Class<T> clazz) {
        if(map.get(clazz) == null){
            map.put(clazz, (T) BeanFactoryUtils.beanOfType(applicationContext, clazz));
        }
        return (T) map.get(clazz);
    }
    
    public static Object getBean(String name){
    	if(applicationContext==null){
    		return null;
    	}
    	return applicationContext.getBean(name);
    }
}
