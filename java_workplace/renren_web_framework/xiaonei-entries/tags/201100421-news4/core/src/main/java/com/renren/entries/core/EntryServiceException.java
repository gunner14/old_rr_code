package com.renren.entries.core;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-6-15 下午08:34:34
 */
public class EntryServiceException extends RuntimeException {

    private static final long serialVersionUID = 1L;

    public EntryServiceException(String msg) {
        super(msg);
    }
    
    public EntryServiceException(String msg, Throwable cause) {
        super(msg, cause);
    }
    
}
