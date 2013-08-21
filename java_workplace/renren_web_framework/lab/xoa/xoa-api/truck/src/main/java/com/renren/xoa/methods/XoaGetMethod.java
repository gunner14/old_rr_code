package com.renren.xoa.methods;


/**
 * GET方法
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午04:36:57
 */
public class XoaGetMethod extends XoaBodilessMethod {

	/*public XoaGetMethod(String domain, String path) {
		super(domain, path);
	}*/
	
	@Override
	public XoaMethodName getName() {
		return XoaMethodName.GET;
	}
	
}
