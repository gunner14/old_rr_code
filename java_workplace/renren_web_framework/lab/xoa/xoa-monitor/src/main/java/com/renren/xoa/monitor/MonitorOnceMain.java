package com.renren.xoa.monitor;

import java.util.List;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-23 下午05:58:27
 */
public class MonitorOnceMain {

    
    public static void main(String[] args) {
        
        MonitorTask task = new MonitorTaskImpl();
        List<MonitorResult> resList = task.monitorAll();
        for (MonitorResult res : resList) {
            System.out.println(res);
        }
        task.destroy();
        System.exit(0);
    }
}
