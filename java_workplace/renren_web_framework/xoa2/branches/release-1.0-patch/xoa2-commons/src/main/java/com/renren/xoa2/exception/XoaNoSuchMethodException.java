package com.renren.xoa2.exception;


/**
 * runtime异常，用于代替{@link NoSuchMethodException}
 * 
 * @author yanghe.liang
 * @since 2011-12-28 下午3:06:47
 */
public class XoaNoSuchMethodException extends XoaRuntimeException {

    private static final long serialVersionUID = 1L;

    public XoaNoSuchMethodException(Throwable cause) {
        super(cause);
    }

}
