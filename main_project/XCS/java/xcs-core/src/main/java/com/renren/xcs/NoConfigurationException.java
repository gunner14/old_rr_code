package com.renren.xcs;
/**
 * 没有配置xcs.conf系统属性时，抛出的异常
 * @author Xue Wenxin Nov 23, 2012 11:43:44 AM
 */
public class NoConfigurationException extends XcsException {

	private static final long serialVersionUID = -3024599833665495284L;

	public NoConfigurationException(String message, Exception e) {
		super(message, e);
	}

	public NoConfigurationException(String message) {
		super(message);
	}
}
