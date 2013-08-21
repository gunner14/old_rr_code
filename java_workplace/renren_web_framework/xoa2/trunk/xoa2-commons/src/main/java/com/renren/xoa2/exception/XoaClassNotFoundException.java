package com.renren.xoa2.exception;

/**
 * runtime异常，用于代替{@link ClassNotFoundException}
 * 
 * @author yanghe.liang
 * @since 2011-12-28 下午2:33:56
 */
public class XoaClassNotFoundException extends XoaRuntimeException {

    private static final long serialVersionUID = 1L;

    public XoaClassNotFoundException(Throwable cause) {
        super(cause);
    }

}
