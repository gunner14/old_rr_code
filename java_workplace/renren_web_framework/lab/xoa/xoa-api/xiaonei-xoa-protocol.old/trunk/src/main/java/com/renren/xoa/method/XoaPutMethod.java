package com.renren.xoa.method;

/**
 * PUT方法
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午04:36:32
 */
public class XoaPutMethod extends XoaEntityEnclosingMethod {

	public XoaPutMethod(String domain, String path) {
		super(domain, path);
	}

	@Override
	public XoaMethodName getName() {
		return XoaMethodName.PUT;
	}

}
