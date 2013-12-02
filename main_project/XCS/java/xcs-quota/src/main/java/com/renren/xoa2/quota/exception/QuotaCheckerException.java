package com.renren.xoa2.quota.exception;
/**
 * QuotaChecker 的异常
 * @author Xue Wenxin Oct 10, 2012 4:05:16 PM
 */
public class QuotaCheckerException extends XoaQuotaException {
	
	private static final long serialVersionUID = 1L;
	
	public QuotaCheckerException(String msg) {
        super(msg);
    }
	
	public QuotaCheckerException(Throwable cause) {
		super(cause);
	}
	public QuotaCheckerException(String msg, Throwable cause) {
        super(msg, cause);
    }
}
