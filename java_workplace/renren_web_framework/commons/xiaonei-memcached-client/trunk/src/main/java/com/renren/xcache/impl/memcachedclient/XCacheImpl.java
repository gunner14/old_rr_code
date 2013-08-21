package com.renren.xcache.impl.memcachedclient;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import com.danga.MemCached.MemCachedClient;
import com.renren.xcache.XCache;

/**
 * {@link XCache}的一个实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @author Wang Zhiliang
 */
public class XCacheImpl<T> extends XCache<T> {

    /**
     * namespace和type之间的分隔符
     */
    public static final char NT_SEPARATOR = '.';

    /**
     * type和id之间的分隔符，比如要使用':'，因为memcached支持基于':'分隔的prefix统计
     */
    public static final char TI_SEPARATOR = ':';

    /**
     * 最长过期时间，30天
     */
    public static final int MAX_EXPIRE = XCache.EXPIRE_DAY * 30;

    /**
     * 命名空间，相当于业务类型
     */
    private final String namespace;

    /**
     * keyPrefix = namespace + NT_SEPARATOR + type
     */
    private final String keyPrefix;

    private final XCacheFactoryImpl cacheFactory;

    private final Class<T> valueClass;
    
    private boolean isCounter;
    
    private ValueCaster valueCaster = new ValueCaster();
    
    /**
     * 构造一个基于人人网基础设施的XCache实例。
     * <p>
     * 
     * 参数prefix表示该cache的使用策略，由两部分组成namespace和type，并通过点号连接，通过这个prefix，
     * XCache将从配置服务器中获取相关的使用策略。
     * <p>
     * 合法的prefix，非空且只能是由数字、字母、下划线、连接线以及点号组成，如：home.username、app.type<br>
     * 非法的prefix，将抛出 {@link IllegalArgumentException}异常，并在异常信息中进行说明
     * <p>
     * 
     * 您在使用XCacheImpl前，必须确认prefix已经在登记在人人网中，如果还未登录，请参考wiki:
     * http://doc.d.xiaonei.com/[wiki地址]
     * 
     * @throws {@link NullPointerException}
     * @throws {@link IllegalArgumentException}
     * @param prefix key prefix
     * @param cacheFactory xcache factory
     * @param valueClass value的类型
     * @param isCounter 当前XCache实例是否用作Counter
     */
    public XCacheImpl(String prefix, XCacheFactoryImpl cacheFactory, Class<T> valueClass, boolean isCounter) {
        // not null
        if (prefix == null) {
            throw new NullPointerException("prefix");
        }
        prefix = prefix.trim();
        // ':' not allowed
        if (prefix.indexOf(TI_SEPARATOR) != -1) {
            throw new IllegalArgumentException("wrong prefix '" + prefix + "': char '"
                    + TI_SEPARATOR + "' is not allowed ");
        }
        // '.' 在其中
        int index = prefix.indexOf(NT_SEPARATOR);
        if (index <= 0 || index == prefix.length() - 1) {
            throw new IllegalArgumentException("wrong prefix '" + prefix + "': char '"
                    + NT_SEPARATOR + "' is needed");
        }
        // not duplicated
        if (prefix.indexOf(NT_SEPARATOR, index + 1) != -1) {
            throw new IllegalArgumentException("wrong prefix '" + prefix + "': char '"
                    + NT_SEPARATOR + "' duplicated");
        }
        // legal chars
        for (int i = 0; i < prefix.length(); i++) {
            char ch = prefix.charAt(i);
            if (ch >= '0' && ch <= '9') {
                continue;
            }
            if (ch >= 'a' && ch <= 'z') {
                continue;
            }
            if (ch == NT_SEPARATOR || ch == '-' || ch == '_') {
                continue;
            }
			if (ch >= 'A' && ch <= 'Z') {
				throw new IllegalArgumentException("wrong prefix '" + prefix
						+ "', only lower-case letter is allowed");
			}
            
            throw new IllegalArgumentException("wrong prefix '" + prefix + "': char '" + ch
                    + "' is not allowed");
        }
        // ok
        this.namespace = prefix.substring(0, index); // namespace is the first part of prefix
        this.keyPrefix = prefix;
        this.cacheFactory = cacheFactory;
        this.valueClass = valueClass;
        this.isCounter = isCounter;
        
        //类型判断
		if (isCounter && !isCounterType(valueClass)) {
			throw new IllegalArgumentException(
					"For a counter, the value type must be Long or Integer, but yours is: "
							+ valueClass);
		}
    }

    /**
     * 判断类型是否可以用作counter，只支持Integer和Long
     * @param type
     * @return
     */
    private boolean isCounterType(Class<?> type) {
    	return type.equals(Long.class) || type.equals(Integer.class); 
    }
    
    /* (non-Javadoc)
     * @see com.renren.xcache.XCache#add(java.lang.String, java.lang.Object, int)
     */
    public boolean add(String id, T value, int expire) {
    	checkValueType(value);
        String key = generateKey(id);
        return getClient().add(key, value, calcExpireDate(expire));
    }

    /* (non-Javadoc)
     * @see com.renren.xcache.XCache#set(java.lang.String, java.lang.Object, int)
     */
    public boolean set(String id, T value, int expire) {
    	checkValueType(value);
        String key = generateKey(id);
        Object finalValue = value;
        if (this.isCounter()) {
        	//因为在构造函数里已经判断值的类型为Integer或Long了，所以这里可以放心地转为String
        	finalValue = "" + value;
        }
        return getClient().set(key, finalValue, calcExpireDate(expire));
    }

    private void checkValueType(Object value) {
		if (!valueClass.isAssignableFrom(value.getClass())) {
			throw new IllegalArgumentException("Value type must be '"
					+ valueClass.getName() + "' or its subclass, but it's '"
					+ value.getClass().getName() + "'");
		}
    }
    
    /* (non-Javadoc)
     * @see com.renren.xcache.XCache#get(java.lang.String)
     */
	public T get(String id) {
        String key = generateKey(id);
        Object obj = getClient().get(key);
        return valueCaster.cast(valueClass, obj);
    }

    /* (non-Javadoc)
     * @see com.renren.xcache.XCache#delete(java.lang.String)
     */
    public boolean delete(String id) {
        String key = generateKey(id);
        return getClient().delete(key);
    }

    /* (non-Javadoc)
     * @see com.renren.xcache.XCache#incr(java.lang.String, long)
     */
    public long incr(String id, long inc) {
        String key = generateKey(id);
        return getClient().incr(key, inc);
    }

    /* (non-Javadoc)
     * @see com.renren.xcache.XCache#decr(java.lang.String, long)
     */
    public long decr(String id, long inc) {
        String key = generateKey(id);
        return getClient().decr(key, inc);
    }

    /* (non-Javadoc)
     * @see com.renren.xcache.XCache#getMulti(java.lang.String[])
     */
	public Map<String, T> getMulti(String[] ids) {
    	String[] keys = new String[ids.length];
    	for (int i = 0; i < ids.length; i++) {
			keys[i] = generateKey(ids[i]);
		}
    	
    	Map<String, Object> originalRet = getClient().getMulti(keys);
    	Map<String, T> map = new HashMap<String, T>(originalRet.size());
    	
    	for (Entry<String, Object> entry : originalRet.entrySet()) {
    		String id = resolveIdFromKey(entry.getKey());
    		map.put(id, valueCaster.cast(valueClass, entry.getValue()));
    	}
    	return map;
    }
    
    /**
     * 由于MemcachedClient用Date类型作过期时间，而在我们封装的API中，使用int，所以这里做一个转换。
     * 使用int会相对方便一些，而且memcached协议本身的过期时间也是使用int来表示的。
     * 
     * @param expire
     * @return
     */
    private Date calcExpireDate(int expire) {
        if (expire <= 0 || expire > MAX_EXPIRE) {
            throw new IllegalArgumentException("Expiration should be in (0, " + MAX_EXPIRE
                    + "]. But yours is " + expire);
        }
        return new Date(expire * 1000);
    }

    private MemCachedClient getClient() {
        return cacheFactory.getMemcachedClient(namespace);
    }

    /**
     * 给据指定的id拼出一个用于memcached存储的key来
     * @param id
     * @return
     */
    private String generateKey(String id) {
        return keyPrefix + TI_SEPARATOR + id;
    }
    
    /**
     * 从key中解析出id
     * @param key
     * @return
     */
    private String resolveIdFromKey(String key) {
    	return key.substring(keyPrefix.length() + 1);	//+1是因为还有个TI_SEPARATOR
    }

	@Override
	public boolean isCounter() {
		return this.isCounter;
	}
}
