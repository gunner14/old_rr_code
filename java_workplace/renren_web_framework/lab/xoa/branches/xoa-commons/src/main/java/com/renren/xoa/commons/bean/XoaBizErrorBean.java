package com.renren.xoa.commons.bean;

/**
 * 用来封装业务中的错误信息，以返回给客户端
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-7-28 下午02:53:38
 */
public class XoaBizErrorBean {

	public static final int STATUS_CODE = 510;
	
	private int errorCode;
	
	private String message;
	
	public XoaBizErrorBean() {
		
	}

	public XoaBizErrorBean(int errorCode, String message) {
		this.errorCode = errorCode;
		this.message = message;
	}
	
	public XoaBizErrorBean(String message) {
		this.message = message;
	}
	
	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public int getErrorCode() {
		return errorCode;
	}

	public void setErrorCode(int errorCode) {
		this.errorCode = errorCode;
	}
}
