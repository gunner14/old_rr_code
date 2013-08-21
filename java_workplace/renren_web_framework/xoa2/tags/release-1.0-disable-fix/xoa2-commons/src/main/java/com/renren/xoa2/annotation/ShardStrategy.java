package com.renren.xoa2.annotation;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 28, 2011 4:48:13 PM
 */
public @interface ShardStrategy {

    /**
     * @return the class name of the strategy
     */
    public String value();
    
}
