package com.renren.xoa;

import org.json.JSONObject;


/**
 * 封装由T类型对象到JSONObject对象的映射逻辑
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-4 下午01:53:31
 * @param <T>
 */
public interface OJMapper<T> {

	/**
	 * 将一个T类型的对象映射为一个JSONObject类型的对象
	 * 
	 * @param json 待映射的JSONObject
	 * @param object 要映射成的对象
	 * @return 映射后的对象
	 */
	public JSONObject map(T object, JSONObject json);
	
}
