package com.renren.xoa;

import org.json.JSONObject;

/**
 * 表示一次Xoa调用的返回值
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午04:34:09
 */
public interface XoaResponse {

	/**
	 * @return 此次调用的状态码
	 */
	public int getStatusCode();
	
	/**
	 * 返回指定应答头部
	 * @param headerName
	 * @return
	 */
	public String getHeader(String headerName);
	
	/**
	 * 将返回值封装成klass类型的对象并返回
	 * 
	 * @param klass
	 * @return klass类型的对象
	 */
	public Object getContent(Class<?> klass);
	
	/**
	 * @param <T>
	 * @param mapper
	 * @param obj
	 * @return
	 */
	public <T> T getContent(JOMapper<T> mapper);
	
	/**
	 * 以JSONObject的形式返回内容
	 * 
	 * @return
	 */
	public JSONObject getContentAsJson();
	
	/**
	 * 以String的形式返回内容
	 * @return
	 */
	public String getContentAsString();
}
