package com.renren.xcache;

import java.util.Map;

/**
 * {@link XCache} is a encapsulation memcached client. It provides easy and powerful
 * API of memcached operations. Enjoy it.
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @author Wang Zhiliang
 */
public abstract class XCache<T> extends XCacheBase {

	/**
	 * Expiration, a second
	 */
	public static final int EXPIRE_SECOND = 1;
	
	/**
	 * Expiration, a minute
	 */
	public static final int EXPIRE_MINUTE = 60;
	
	/**
	 * Expiration, a hour
	 */
	public static final int EXPIRE_HOUR = 60 * EXPIRE_MINUTE;
	
	/**
	 * Expiration, a day
	 */
	public static final int EXPIRE_DAY = 24 * EXPIRE_HOUR;
	
	/**
	 * Specify whether this XCache instance is used as a counter.
	 * Counter is a date type that you can modify with incr/decr command.
	 * In memcached, counter is store as string, so special operation is needed
	 * when doing set and get.
	 * 
	 * @return true if this XCache instance is used as a counter.
	 */
	public abstract boolean isCounter();
	
	/**
	 * Adds data to the server; the id, value, and an expiration time are
	 * specified.
	 * 
	 * @param id
	 *            id to store data under
	 * @param value
	 *            value to store
	 * @param expiry
	 *            the expiration of the data
	 * @return true, if the data was successfully stored
	 */
    public abstract boolean add(String id, T value, int expireInSecond);

    /**
	 * Stores data on the server; only the id and the value are specified.
	 * 
	 * @param id
	 *            id to store data under
	 * @param value
	 *            value to store
	 * @param expiry
	 *            the expiration of the data
	 * @return true, if the data was successfully stored
	 */
    public abstract boolean set(String id, T value, int expireInSecond);

    /**
	 * Retrieve a id from the server.
	 * 
	 * @param id
	 *            id where data is stored
	 * @return the object that was previously stored, or null if it was not
	 *         previously stored
	 */
    public abstract T get(String id);

    /**
	 * Deletes an object from cache given data id.
	 * 
	 * @param id
	 *            the id to be removed
	 * @return <code>true</code>, if the data was deleted successfully
	 */
    public abstract boolean delete(String id);

    /**
	 * Increment the value at the specified id by passed in val.
	 * 
	 * @param id
	 *            id where the data is stored
	 * @param inc
	 *            how much to increment by
	 * @return -1, if the id is not found, the value after incrementing
	 *         otherwise
	 */
    public abstract long incr(String id, long inc);

    /**
	 * Decrement the value at the specified id by passed in value, and then
	 * return it.
	 * 
	 * @param id
	 *            id where the data is stored
	 * @param inc
	 *            how much to increment by
	 * @return -1, if the id is not found, the value after incrementing
	 *         otherwise
	 */
    public abstract long decr(String id, long inc);

    /**
	 * Retrieve multiple objects from the memcache.
	 * 
	 * This is recommended over repeated calls to {@link #get(String) get()},
	 * since it<br/>
	 * is more efficient.<br/>
	 * 
	 * @param ids
	 *            String array of ids to retrieve
	 * @return a hashmap with entries for each id is found by the server, ids
	 *         that are not found are not entered into the hashmap, but
	 *         attempting to retrieve them from the hashmap gives you null.
	 */
    public abstract Map<String, T> getMulti(String[] ids);
    
}
