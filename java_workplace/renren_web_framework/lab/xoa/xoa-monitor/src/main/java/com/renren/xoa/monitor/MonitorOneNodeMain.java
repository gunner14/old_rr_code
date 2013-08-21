package com.renren.xoa.monitor;

import java.util.List;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-23 下午05:58:27
 */
public class MonitorOneNodeMain {

    public static void main(String[] args) {
       
        if(args.length != 2) {
            System.err.println("Usage: java " + MonitorOneNodeMain.class.getName() + " ip port");
            System.exit(1);
        }
        
        String ip = args[0];
        int port = Integer.parseInt(args[1]);
        
        MonitorTask task = new MonitorTaskImpl();
        List<MonitorResult> resList = task.monitor(ip, port);
        for (MonitorResult res : resList) {
            System.out.println(res);
        }
        task.destroy();
        System.exit(0);
       
    }
}
