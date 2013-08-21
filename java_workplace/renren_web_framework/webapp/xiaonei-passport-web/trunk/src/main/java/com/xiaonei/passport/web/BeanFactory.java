/**
 * 
 */
package com.xiaonei.passport.web;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanFactoryUtils;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Service;






/**
 * LoginLogicFactory <br>
 * 
 * @author tai.wang@opi-corp.com Mar 2, 2010 - 10:22:22 AM
 */
@Service
public class BeanFactory implements ApplicationContextAware {
    
    private final static Map<Class<?>,Object> map = new ConcurrentHashMap<Class<?>, Object>();

    private static ApplicationContext applicationContext;
   
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

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        BeanFactory.applicationContext = applicationContext;
    }
}
