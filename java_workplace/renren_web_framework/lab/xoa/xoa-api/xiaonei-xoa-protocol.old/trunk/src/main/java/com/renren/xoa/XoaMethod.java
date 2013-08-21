package com.renren.xoa;

import com.renren.xoa.method.XoaMethodName;

/**
 * 用来表示要调用的XOA方法。
 * 
 * 方法的定位采用了REST的设计思想。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 上午11:47:52
 */
public interface XoaMethod {

	/**
     * 获取当前方法的名字，如GET,POST,PUT,DELETE
     * 
     * @return the name of this method
     */
    public XoaMethodName getName();
	
	/**
	 * 此XOA方法所属的domaim，包含了端口信息，如：blog.renren.com:8888
	 * @param domain
	 */
	public void setDomain(String domain);
	
	/**
	 * @return 此XOA方法所属的domain
	 */
	public String getDomain();
	
	/**
	 * 此方法所在路径，如：/blog/user/1234，同时这个路径中是可以带query string的，
	 * 如：/blog/user/1234?type=1。
	 * 
	 * domain和path一起就构成了此方法的URL
	 * 
	 * @param path
	 */
	public void setPath(String path);
	
	/**
	 * @return 此方法所在路径
	 */
	public String getPath();

	/**
	 * 增加一个参数，同一个参数名可以有多个参数值
	 * @param name 参数名
	 * @param value 参数值
	 */
	public void setParam(String name, String value);
	
}
