package com.renren.xoa;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-12 上午12:29:11
 */
public class XoaBizException extends XoaException {

	private static final long serialVersionUID = 1L;

	private int bizCode;
	
	/*public XoaBizException() {
		super();
	}*/
	
	public XoaBizException(int bizCode, String msg) {
		super(msg);
		this.bizCode = bizCode;
	}
	
	/*public XoaBizException(Throwable cause) {
		super(cause);
	}
	
	public XoaBizException(String msg, Throwable cause) {
		super(msg, cause);
	}*/
	
	public int getBizCode() {
		return bizCode;
	}

	public void setBizCode(int bizCode) {
		this.bizCode = bizCode;
	}
}
