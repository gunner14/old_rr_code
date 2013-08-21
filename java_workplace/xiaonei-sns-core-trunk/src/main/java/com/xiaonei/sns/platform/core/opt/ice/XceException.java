/**
 * $Id: XceException.java 17104 2010-02-26 01:31:04Z xingtao.shi@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.control.BlockException;


/** The Exeption from ICE/XCE
 * @author xylz(xylz@live.cn)
 * @since 2010-1-12
 */
public class XceException extends BlockException {

    /**
	 * 
	 */
	private static final long serialVersionUID = 8246780120063107783L;

	public XceException() {
    }

    public XceException(String message) {
        super(message);
    }

    public XceException(Throwable cause) {
        super(cause);
    }

    public XceException(String message, Throwable cause) {
        super(message, cause);
    }

}
