package com.renren.xoa;

import org.json.JSONObject;

/**
 * 封装由JSONObject到程序所需要的类型T的映射逻辑
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-4 下午12:07:44
 * @param <T> 由JSONObject转化为的类型
 */
public interface JOMapper<T> {

	/**
	 * 将一个JSONObject映射为一个T类型的对象
	 * 
	 * @param json 待映射的JSONObject
	 * @return 映射后的对象
	 */
	public T map(JSONObject json);
	
}
