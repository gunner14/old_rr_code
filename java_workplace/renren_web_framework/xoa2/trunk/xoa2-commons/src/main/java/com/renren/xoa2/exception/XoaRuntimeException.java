package com.renren.xoa2.exception;


/**
 * xoa的根异常
 * 
 * @author yanghe.liang
 * @since 2011-12-28 下午2:40:41
 */
public class XoaRuntimeException extends RuntimeException {

    private static final long serialVersionUID = 1L;
    
    public XoaRuntimeException(String msg) {
        super(msg);
    }
    
    public XoaRuntimeException(Throwable cause) {
        super(cause);
    }
    
    public XoaRuntimeException(String msg, Throwable cause) {
        super(msg, cause);
    }

}
