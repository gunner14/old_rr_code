package com.renren.xoa.methods;

/**
 * POST方法
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午04:36:45
 */
public class XoaPostMethod extends XoaEntityEnclosingMethod {
	
	/*public XoaPostMethod(String domain, String path) {
		super(domain, path);
	}*/

	@Override
	public XoaMethodName getName() {
		return XoaMethodName.POST;
	}

}
