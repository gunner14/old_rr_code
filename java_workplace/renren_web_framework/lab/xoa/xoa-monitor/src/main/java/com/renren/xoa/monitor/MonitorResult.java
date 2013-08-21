package com.renren.xoa.monitor;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-23 下午05:49:06
 */
public class MonitorResult {

    private String serviceId;
    
    private String ip;
    
    private int port;
    
    private boolean disabled = false;
    
    private String status;

    
    public String getServiceId() {
        return serviceId;
    }

    
    public void setServiceId(String serviceId) {
        this.serviceId = serviceId;
    }

    
    public String getIp() {
        return ip;
    }

    
    public void setIp(String ip) {
        this.ip = ip;
    }

    
    public int getPort() {
        return port;
    }

    
    public void setPort(int port) {
        this.port = port;
    }

    
    public boolean isDisabled() {
        return disabled;
    }

    
    public void setDisabled(boolean disabled) {
        this.disabled = disabled;
    }

    
    public String getStatus() {
        return status;
    }

    
    public void setStatus(String status) {
        this.status = status;
    }
    
    public String toString() {
        
        StringBuilder sb = new StringBuilder();
        sb.append(serviceId);
        sb.append(" ");
        sb.append(ip);
        sb.append(" ");
        sb.append(port);
        sb.append(" ");
        sb.append((isDisabled() ? "Disabled" : "Enabled"));
        sb.append(" ");
        sb.append(status);
        return sb.toString();
    }
}
