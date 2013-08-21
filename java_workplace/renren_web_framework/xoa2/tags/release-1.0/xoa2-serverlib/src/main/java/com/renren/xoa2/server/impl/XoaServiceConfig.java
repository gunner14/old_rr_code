package com.renren.xoa2.server.impl;

/**
 * XoaServiceConfig对象，每一个对象实例与一个XoaService实例对应
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Nov 28, 2011 5:20:59 PM
 */
public class XoaServiceConfig {

    private String serviceId;

    private String serviceClass;
    
    private boolean isXmqService = false;

    private int port;

    private int threadPoolMaxSize = 0;
    
    private int threadPoolCoreSize = 0;

    private String initListenerClass;
    
    private boolean militiaFlag = false;

    public String getServiceId() {
        return serviceId;
    }

    public void setServiceId(String serviceId) {
        this.serviceId = serviceId;
    }

    public String getServiceClass() {
        return serviceClass;
    }

    public void setServiceClass(String serviceClass) {
        this.serviceClass = serviceClass;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public String getInitListenerClass() {
        return initListenerClass;
    }

    public void setInitListenerClass(String initListenerClass) {
        this.initListenerClass = initListenerClass;
    }
    
    public boolean getMilitiaFlag() {
        return militiaFlag;
    }
    
    public void setMilitiaFlag(boolean flag) {
        militiaFlag = flag;
    }

    @Override
    public String toString() {
        return new StringBuffer(" serviceId=").append(serviceId).append(", serviceClass=")
                .append(serviceClass).append(", port=").append(port).append(", initListenerClass=")
                .append(initListenerClass).append(", threadPoolMaxSize=").append(threadPoolMaxSize)
                .append(", threadPoolCoreSize=").append(threadPoolCoreSize).toString();
    }

	public void setThreadPoolMaxSize(int threadPoolMaxSize) {
		this.threadPoolMaxSize = threadPoolMaxSize;
	}

	public int getThreadPoolMaxSize() {
		return threadPoolMaxSize;
	}

	public void setThreadPoolCoreSize(int threadPoolCoreSize) {
		this.threadPoolCoreSize = threadPoolCoreSize;
	}

	public int getThreadPoolCoreSize() {
		return threadPoolCoreSize;
	}

    public void setXmqService(boolean isXmqService) {
        this.isXmqService = isXmqService;
    }

    public boolean isXmqService() {
        return isXmqService;
    }
}
