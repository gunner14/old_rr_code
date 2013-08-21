/**
 * 
 */
package com.xiaonei.reg.thread.timelimit.exception;

/**
 * 没有返回数据
 * 
 * @author wangtai
 *
 */
public class TimeLimitNoReturnException extends TimeLimitException{

	public TimeLimitNoReturnException(String e) {
		super(e);
	}

	private static final long serialVersionUID = -3666879537875426083L;

	@Override
	public String getMessage() {
		return super.getMessage();
	}

}
