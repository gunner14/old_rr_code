package com.renren.xoa2.registry;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 28, 2011 4:41:16 PM
 */
public interface Node extends Comparable<Node>{
    public String getHost();    
    public int getPort();

    public boolean isDisabled();    
    public void setDisabled(boolean disabled);    

    public boolean isHealthy();
    public void setHealthy(boolean healthy);
}
