/**
 * 
 */
package com.xiaonei.reg.common;

/**
 * for String,Integer,Long,Double,Boolean
 * 
 * @author wangtai
 * 
 */
public class ObjectHolder<T> {

	private volatile T property;

	public void set(T p) {
		property = p;
	}

	public T get() {
		return property;
	}

}
