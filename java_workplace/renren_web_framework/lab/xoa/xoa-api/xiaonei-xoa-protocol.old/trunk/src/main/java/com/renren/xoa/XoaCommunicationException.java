package com.renren.xoa;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-12 上午12:29:11
 */
public class XoaCommunicationException extends XoaException {

	private static final long serialVersionUID = 1L;

	public XoaCommunicationException() {
		super();
	}
	
	public XoaCommunicationException(String msg) {
		super(msg);
	}
	
	public XoaCommunicationException(Throwable cause) {
		super(cause);
	}
	
	public XoaCommunicationException(String msg, Throwable cause) {
		super(msg, cause);
	}
	
}
