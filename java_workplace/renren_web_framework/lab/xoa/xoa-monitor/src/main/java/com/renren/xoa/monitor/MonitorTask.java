package com.renren.xoa.monitor;

import java.util.List;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-23 下午05:48:27
 */
public interface MonitorTask {

    public List<MonitorResult> monitor(String ip, int port);
    
    public List<MonitorResult> monitorAll();
    
    public void destroy();
}
