package com.renren.xcs;

/**
 * 读取配置出错时，抛出的异常。
 * @author xun.dai@renren-inc.com
 *
 */
public class InvalidConfigurationException extends XcsException {

	private static final long serialVersionUID = -3402536374224209460L;

	public InvalidConfigurationException(String message, Exception e) {
		super(message, e);
	}

	public InvalidConfigurationException(String message) {
		super(message);
	}

}
