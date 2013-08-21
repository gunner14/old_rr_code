package com.xiaonei.platform.framework.exception;


/**
 * 基础异常类，其他各类异常都继承此类<br>
 * 在构造异常类时，根据子类中传入code，查资源文件，找到对应的message 
 * @author lifs
 * 
 */
public class XnException extends Exception {
	
	private int code;

	/**
	 * @return the code
	 */
	public int getCode() {
		return code;
	}

	/**
	 * @param code the code to set
	 */
	public void setCode(int code) {
		this.code = code;
	}
	
	public XnException(int code) {
		super();
		this.code = code;
	}
	public XnException(int code, String message) {
		super(message);
		this.code = code;
	}

	public XnException(int code, Throwable cause) {
		super(cause);
		this.code = code;
	}

	public XnException(int code, String message, Throwable cause) {
		super(message,cause);
		this.code = code;
	}

}
