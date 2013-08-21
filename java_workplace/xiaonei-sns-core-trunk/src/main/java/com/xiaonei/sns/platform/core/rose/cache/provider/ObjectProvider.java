package com.xiaonei.sns.platform.core.rose.cache.provider;

import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * a provider for any object
 * 
 * @author xylz(xylz@live.cn)
 * @since 20091106
 */
public class ObjectProvider extends AbstractProvider<Object> {

	private static ObjectProvider instance = new ObjectProvider();

	private ObjectProvider() {
	}

	/**
	 * get the instance for the Object
	 */
	public static ObjectProvider getInstance() {
		return instance;
	}

	/**
	 * get one Object from Cache eache time<br />
	 * 
	 * @param params
	 *            the paramters must be only one and others will be ignored
	 * @return return the value for the first key or null while nothing is found
	 * @throws NullPointerException
	 *             the cm or params is null
	 * @throws IllegalArgumentException
	 *             the params is empty or the first object is null
	 */
	public Object get(CacheManager cm, Object... params) {
		if (cm == null || params == null)
			throw new NullPointerException(
					"the CacheManager or the params must not be null");
		if (params.length == 0 || params[0] == null)
			throw new IllegalArgumentException(
					"the params must be only one and not null");
		return cm.get(params[0]);
	}

	/**
	 * put an Object into the CacheManager <br />
	 * It will clear the key if the value is null.
	 * 
	 * @param cm
	 *            the CacheManager Object
	 * @param key
	 *            the key
	 * @param value
	 *            the value
	 * @throws NullPointerException
	 *             the key or value is null
	 */
	public void set(CacheManager cm, Object key, Object value) {
		if (cm == null || key == null)
			throw new NullPointerException(
					"the CacheManager or the Key must not be null");
		if (value == null) {
			cm.remove(key);
		} else {
			cm.put(key, value);
		}
	}

}
