package com.xiaonei.commons.interceptors.performance;

import org.perf4j.StopWatch;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-8-24 下午02:19:15
 */
public interface StopWatchable {

    public void setStopWatch(StopWatch stopWatch);
    
}
