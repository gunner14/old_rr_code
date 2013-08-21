package com.renren.xoa;

import com.renren.xoa.commons.exception.XoaException;

/**
 * 
 * XOA调用返回的{@link XoaResponse}的status code不是200(OK)时抛出此异常
 * 
 * @author Li Weibo (weibo.li@opi-corp.com) //I believe spring-brother
 * @since 2010-3-29 下午04:56:05
 */
public class StatusNotOkException extends XoaException {

	private static final long serialVersionUID = 1L;

	/**
	 * 这个异常所包装的XoaResponse对象
	 */
	private XoaResponse response;
	
	public StatusNotOkException() {
		super();
	}
	
	public StatusNotOkException(String msg) {
		super(msg);
	}
	
	public StatusNotOkException(Throwable cause) {
		super(cause);
	}
	
	public StatusNotOkException(String msg, Throwable cause) {
		super(msg, cause);
	}

	public XoaResponse getResponse() {
		return response;
	}

	public StatusNotOkException setResponse(XoaResponse response) {
		this.response = response;
		return this;
	}
}
