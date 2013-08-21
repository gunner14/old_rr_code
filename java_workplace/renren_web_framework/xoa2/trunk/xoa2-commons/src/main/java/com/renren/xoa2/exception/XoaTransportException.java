package com.renren.xoa2.exception;


/**
 * 获得transport为null或者不可用抛出这个异常
 * 
 * @author yanghe.liang
 * @since 2011-12-28 下午3:38:47
 */
public class XoaTransportException extends XoaRuntimeException {

    private static final long serialVersionUID = 1L;
    
    public XoaTransportException(String msg) {
        super(msg);
    }

}
