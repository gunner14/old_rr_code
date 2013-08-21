package com.renren.xoa2.scheduler.configure;

/**
 * Xoa 服务的调度配置文件 
 * 
 * @author Yuan Liu (yuan.liu1@renren-inc.com)
 * @date   2012-3-31 
 */
public class XoaJobConfigure {
    private int taskNumber;
    private int serviceTime;
    private int serviceIdleTime;
    private String serviceConfPath;
    
    public boolean isValid() {
        return true;
    }
    
    public void setTaskNumber(int number) {
        taskNumber = number;
    }
    
    public int getTaskNumber() {
        return taskNumber;        
    }
    
    public void setServiceTime(int time) {
        serviceTime = time;
    }
    
    public int getServiceTime() {
        return serviceTime;
    }
    
    public void setServiceIdleTime(int time) {
        serviceIdleTime = time;
    }
    
    public int getServiceIdleTime() {
        return serviceIdleTime;
    }
    
    public void setServiceConf(String conf) {
        serviceConfPath = conf;
    }
    
    public String getServiceConf() {
        return serviceConfPath;
    }
}
