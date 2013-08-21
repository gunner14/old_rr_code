package com.renren.xcache;

import com.renren.xcache.helper.ClassUtils;


/**
 * {@link XCache}基类
 * 
 * @author Wang zhiliang
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
class XCacheBase {

    // factory methods
    public static String factoryClass = "com.renren.xcache.impl.memcachedclient.XCacheFactoryImpl";

    private static volatile XCacheFactory xfactory;	//volatile是为了DCL构造的时候没有线程安全问题

    private static Object initLock = new Object();
    
    /**
     * 获取一个 {@link XCache}实例，这个实例不可用作counter。
     * 
     * 具体请参考getCache(String prefix, Class<T> klass, boolean isCounter)方法。
     * 
     * @param <T>
     * @param prefix
     * @param klass
     * @return
     * 
     * @see XCacheFactory
     */
    public static <T> XCache<T> getCache(String prefix, Class<T> klass) {
    	return getCache(prefix, klass, false);
    }
    
    /**
     * 获取一个 {@link XCache}实例
     * <p>
     * 参数prefix表示该cache的使用策略，由两部分组成namespace和type，并通过点号连接，通过这个prefix，
     * XCache将从配置服务器中获取相关的使用策略。
     * <p>
     * 合法的prefix，非空且只能是由数字、字母、下划线、连接线以及点号组成，如：home.username、app.type<br>
     * 非法的prefix，将抛出 {@link IllegalArgumentException}异常，并在异常信息中进行说明
     * <p>
     * 
     * 您在使用 XCache 前，必须确认prefix已经在登记在人人网中，如果还未登录，请参考wiki:
     * http://doc.d.xiaonei.com/[wiki地址]
     * 
     × @param <T> value的类型
     * @param prefix
     * @param klass value的类型
     * @param isCounter 当 {@link XCache}实例是否用作counter，如果你要使用此实例来操作一个计数器，
     * 即使用incr和decr方法，则请务必传入参数true，否则将无法正常使用。
     * @return
     * @see XCacheFactory
     */
    public static <T> XCache<T> getCache(String prefix, Class<T> klass, boolean isCounter) {
    	 // 调用getCache前重新设置factoryClass有效，调用之后将变得无效 
    	if (xfactory == null) {
         	synchronized (initLock) {
 				if (xfactory == null) {
 					xfactory = (XCacheFactory) ClassUtils.newInstanceOfClass(factoryClass);
 				}
 			}
         }
         return xfactory.getCache(prefix, klass, isCounter);
    }
    
    
    /**
     * @param prefix
     * @return
     * 
     * @deprecated 不提倡使用此方法，请明确指定cache所应用的数据类型，即使用
     * getCache(String prefix, Class<T> klass)方法
     */
    public static XCache<Object> getCache(String prefix) {
    	return getCache(prefix, Object.class);
    }

    /**
     * 
     * @param factoryClassName
     * @throws IllegalStateException
     */
    public static void setFactoryClass(String factoryClassName) {
        if (XCacheBase.xfactory != null) {
            throw new IllegalStateException();
        }
        XCache.factoryClass = factoryClassName;
    }

    /**
     * 
     * @param xfactory
     * @throws NullPointerException
     */
    public static void setFactoryClass(XCacheFactory xfactory) {
        if (xfactory == null) {
            throw new NullPointerException();
        }
        XCacheBase.xfactory = xfactory;
    }
}
