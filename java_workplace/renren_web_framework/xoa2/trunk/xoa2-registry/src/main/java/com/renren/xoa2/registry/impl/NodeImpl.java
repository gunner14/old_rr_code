package com.renren.xoa2.registry.impl;

import com.renren.xoa2.registry.Node;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 29, 2011 6:35:10 PM
 */
public class NodeImpl implements Node {

    private String host;
    
    private int port;
    
    private boolean disabled;
    
    public void setHost(String host) {
        this.host = host;
    }
    
    public void setPort(int port) {
        this.port = port;
    }

    @Override
    public String getHost() {
        return host;
    }

    @Override
    public int getPort() {
        return port;
    }

    @Override
    public boolean isDisabled() {
        return disabled;
    }
    
    public void setDisabled(boolean disabled) {
        this.disabled = disabled;
    }

    @Override
    public int compareTo(Node o) {
        
        int cmp = this.host.compareTo(o.getHost());
        if (cmp != 0) {
            return cmp;
        } else {
            
            if (this.port < o.getPort()) {
                return -1;
            } else if (this.port > o.getPort()) {
                return 1;
            }
        }
        return 0;
    }

}
