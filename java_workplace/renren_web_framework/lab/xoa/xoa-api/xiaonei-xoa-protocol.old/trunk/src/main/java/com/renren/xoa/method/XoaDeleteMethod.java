package com.renren.xoa.method;

/**
 * DELETE方法
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午04:37:07
 */
public class XoaDeleteMethod extends XoaBodilessMethod {

	public XoaDeleteMethod(String domain, String path) {
		super(domain, path);
	}

	@Override
	public XoaMethodName getName() {
		return XoaMethodName.DELETE;
	}

}
