/**
 * @(#)CommandException.java, 2012-10-31. 
 * 
 * Copyright 2012 RenRen, Inc. All rights reserved.
 */
package com.renren.xcs.optool;

/**
 * @author Xun Dai <xun.dai@renren-inc.com>
 * 
 */
public class CommandException extends Exception {

    private static final long serialVersionUID = -4153337091917339819L;
    
    private int code;
    
    public int getCode() {
        return code;
    }

    public CommandException(int code, String message) {
        this(code, message, null);
    }

    public CommandException(int code, String message, Throwable cause) {
        super(message, cause);
        this.code = code;
    }
    
}
