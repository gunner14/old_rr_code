package com.renren.xoa2.backdoor;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.AbstractMap;
import java.util.Enumeration;

import org.apache.thrift.TException;
import org.apache.thrift.server.TServer;
import org.apache.zookeeper.ZooKeeper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.registry.impl.zookeeper.PathUtil;
import com.renren.xoa2.registry.impl.zookeeper.ServiceWatcher;
import com.renren.xoa2.server.IServiceBuilder;
import com.renren.xoa2.server.conf.Configuration;
import com.renren.xoa2.server.impl.ServiceBuilderImpl;
import com.renren.xoa2.server.impl.XmqServiceBuilder;
import com.renren.xoa2.server.impl.XoaService;
import com.renren.xoa2.server.impl.XoaServiceConfig;

/**
 * 后门服务基类
 * 提供了一些后门的基础服务
 * @author zhe.long
 * @email  zhe.long@renren-inc.com
 * @date   2012-09-16
 */
public abstract class AbstractBackdoorService implements RenrenService.Iface {
	private String name_;
	private long aliveTime_;
	private renren_status serviceStatus_;
	private String version_;
	private TServer server_;
	private XoaService service_;
	private int port;
	private String serviceId;
	private boolean isStartingMilitiaService;
	
	private byte[] serviceStatusLock = new byte[0];
	private byte[] startingMilitiaServiceLock = new byte[0];
	
	private static Logger logger = LoggerFactory.getLogger(AbstractBackdoorService.class);
	    
    private ServiceWatcher service ;
    
    private ZooKeeper zk ;//监控service zk
	  
    protected AbstractBackdoorService() {
        setStatus(renren_status.STARTING);
        aliveTime_ = System.currentTimeMillis();
        version_ = "1.0";
        isStartingMilitiaService = false;
        setStatus(renren_status.ALIVE);
    }
    
    protected AbstractBackdoorService(String name) {
        setStatus(renren_status.STARTING);
        name_ = name;
        aliveTime_ = System.currentTimeMillis();
        version_ = "1.0";
        isStartingMilitiaService = false;
        setStatus(renren_status.ALIVE);
    }
    
	protected AbstractBackdoorService(String name, String version) {
	    setStatus(renren_status.STARTING);
	    name_ = name;
        aliveTime_ = System.currentTimeMillis();
        version_ = version;
        isStartingMilitiaService = false;
        setStatus(renren_status.ALIVE);
    }
	  
	@Override
    public String getName() {
	    name_ = service_.getConfig().getServiceId();
	    return name_;
	}
	
	@Override
	public String getVersion() {
	    return version_;
	}
    
	//用于返回服务启动的毫秒数
	@Override
    public long aliveSince() {
    	return System.currentTimeMillis() - aliveTime_;
    }

	@Override
    public  renren_status getStatus() {
	    synchronized(serviceStatusLock) {
	        if(logger.isDebugEnabled()) {
	            logger.debug(getStatusDetails());
	        }
	        return serviceStatus_;
	    }
    }
	
	public void setStatus(renren_status status) {
	    synchronized(serviceStatusLock) {
    		serviceStatus_ = status;
	    }
	}
    
	@Override
	public String getStatusDetails() {
	    String statusDetails;
	    switch(serviceStatus_) {
	    case STOPPING:
	        statusDetails = "status is stopping";
	        break;
	    case ALIVE:
            statusDetails = "status is alive";
            break;
	    case DEAD:
            statusDetails = "status is dead";
            break;
	    case STOPPED:
            statusDetails = "status is stopped";
            break;
	    case WARNING:
            statusDetails = "status is warning";
            break;
	    case STARTING:
            statusDetails = "status is starting";
            break;
        default:
            statusDetails = "status is error!";
            break;    
	    }
	    return statusDetails;
	}

	@Override
	public long getCounter(String key) {
	    return service_.getCounter(key);
	}

	@Override
	public  AbstractMap<String,Long> getCounters() {
	    return service_.getCounters();
	}
	
	@Override
	public String getMean(String methodName) {
	    StringBuilder sbMean = new StringBuilder();
	    sbMean.append(getCallTimes(methodName)).append("\r\n").append(getMeanTime(methodName));
	    return sbMean.toString();
	}
	
	@Override
	public String getMax(String methodName, int second) {
        StringBuilder sbMax = new StringBuilder();
        if(System.getProperty("test.phase") != null && System.getProperty("test.phase").equals("true")) {
            sbMax.append(getMaxConcurrentTimes(methodName, second)).append("\r\n").append(getMaxCallTime(methodName, second));
        } else {
            sbMax.append("no test phase! we have no these max data! if you want these data, please -Dtest.phase=true");
        }
        
        return sbMax.toString();
    }
	//传入时间，得到最近一段时间内的调用次数
	
	private String getCallTimes(String methodName) {
	    return service_.getCallTimes(methodName);
	    
	}
	
	//得到一段时间内某方法的平均响应时间
	private String getMeanTime(String methodName) {
	    return service_.getMeanTime(methodName);
	}
	
	//得到一段时间内某方法最大并发数
	private String getMaxConcurrentTimes(String methodName, int second) {
        return service_.getMaxConcurrentTimes(methodName, second);
	    
	}
	
	//得到一段时间内某个方法最长响应时间
	private String getMaxCallTime(String methodName, int second) {
        return service_.getMaxCallTime(methodName, second);
	}
    
	@Override
	public void setOption(String key, String value) {
	    service_.setOption(key, value);
	}

	@Override
	public String getOption(String key) {
	    return service_.getOption(key);
	}

	@Override
	public AbstractMap<String,String> getOptions() {
	    return service_.getOptions();
	}
	
	@Override
	public String getCpuProfile(int durationTime) {
	    return "cpu profile";
	}

    @Override
    public void reinitialize() {
    	setStatus(renren_status.STOPPING);
    	try {
            disabledNodes();
        } catch (TException e) {
            if(logger.isErrorEnabled()) {
                logger.error("reinitialize disable nodes exception");
            }
            return ;
        }
        scheduleAtReinit();
    }
       
    @Override
    public void shutdown() {
    	setStatus(renren_status.STOPPING);
    	try {
    	    disabledNodes();
    	} catch (TException e) {
    	    if(logger.isErrorEnabled()) {
                logger.error("shutdown disable nodes exception");
            }
    	    return ;
    	}
    	scheduleAtShutdown(false);
    }
    
    
    //启动民兵服务，根据服务id来启动服务
    @Override
    public void startMilitiaService(String serviceId) {
        synchronized(startingMilitiaServiceLock) {
            if(isStartingMilitiaService) {
                if(logger.isWarnEnabled()) {
                    logger.warn("state is starting militia service, can't start militia service duplicate");
                }
                return ;
            }
            Configuration config = Configuration.ConfigurationFactory.newConfiguration(service_.getConfPath());
            for (XoaServiceConfig serviceConfig : config.listServiceConfig()) {
                if(serviceConfig.getServiceId().equals(serviceId) && serviceConfig.getMilitiaFlag() == true) {
                    isStartingMilitiaService = true;
                    IServiceBuilder builder = getServiceBuilder(serviceConfig);
                    XoaService service = builder.build(serviceConfig,true);
                    //启动民兵服务过程中出错,则直接返回
                    if(service == null) {
                        if(logger.isWarnEnabled()) {
                            logger.warn("Build militia service fail! Service Id is " + serviceId + " Please try again!");
                        }
                        isStartingMilitiaService = false;
                        return ;
                    }
                    Thread curThread = new Thread(service, serviceConfig.getServiceId());
                    curThread.start();
                    
                    //让民兵服务所启动的服务也加入以前的服务队列
                    service_.getThreadList().add(curThread);
                    service_.getServerList().add(service);
                    shutdownParentService();
                    return ;
                }
            }
            if(logger.isWarnEnabled()) {
                logger.warn("Start militia service fail! No find correct service. Service Id is " + serviceId + " Please check service Id!");
            }
            isStartingMilitiaService = false;
            return ;
        }
    }
    
    public void setServer(TServer server) {
        server_ = server;
    }
    
    public void setService(XoaService service) {
        service_ = service;
    }
    
    
    public void setPort(int port) {
        this.port = port;
    }
    
    public void setServiceId(String serviceId) {
        this.serviceId = serviceId;
    }
    
    public void setZk(ZooKeeper zk) {
        this.zk = zk;
    }
    
    public void setServiceWatcher(ServiceWatcher service) {
        this.service = service;
    }
    
    private void shutdownParentService() {
        setStatus(renren_status.STOPPING);
        try {
            disabledNodes();
        } catch (TException e) {
            if(logger.isErrorEnabled()) {
                logger.error("shutdown militia service disable nodes exception", e);
            }
        }
        scheduleAtShutdown(true);
    }
    
    private void scheduleAtReinit() {
        Thread stopThread = new Thread() {
            public void run() {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {   
                    if(logger.isWarnEnabled()) {
                        logger.warn("re init thread sleep exception!");
                    }
                }
                reInitializeService();
            }
        };
        stopThread.start();
    }
    
    
    //重新初始化某个服务
    private void reInitializeService() {
        setStatus(renren_status.STARTING);
        aliveTime_ = System.currentTimeMillis(); 
        service_.initService();
        try {
            enabledNodes();
        } catch (TException e) {
            if(logger.isErrorEnabled()) {
                logger.error("reInitialize enable nodes exception");
            }
        }
        setStatus(renren_status.ALIVE);
    }
    
    private void scheduleAtShutdown(final boolean isMilitia) {
        Thread stopThread = new Thread() {
            public void run() {
                try {
                    Thread.sleep(1000);
                    if(!isMilitia) {
                        zk.close();
                    } else {
                        synchronized(startingMilitiaServiceLock) {
                            isStartingMilitiaService = false;
                        }
                    }
                } catch (InterruptedException e) {   
                    if(logger.isWarnEnabled()) {
                        logger.warn("Close Zookeeper Exception!");
                    }
                }
                server_.stop();    
                //System.exit(0);
            }
        };
        stopThread.start();
    }
    
    //获得本机的ip地址
    private String getLocalAddr() {
        Enumeration<NetworkInterface> interfaces = null;
        try {
            interfaces = NetworkInterface.getNetworkInterfaces();
        } catch (SocketException e) {
            e.printStackTrace();
            return null;
        }

        while (interfaces.hasMoreElements()) {
            NetworkInterface ifc = interfaces.nextElement();
            Enumeration<InetAddress> addressesOfAnInterface = ifc
                    .getInetAddresses();

            while (addressesOfAnInterface.hasMoreElements()) {
                InetAddress address = addressesOfAnInterface.nextElement();
                if (address.isSiteLocalAddress()) {
                    return address.getHostAddress();
                }
            }
        }
        return null;
    }
    
    private void enabledNodes() throws TException {
        String processorIp = getLocalAddr();
        if(processorIp == null) {
            if(logger.isWarnEnabled()) {
                logger.warn("get processor ip error");
            }
            throw new TException("get processor ip exception");
        }
        String zkPath = getZkPath(serviceId, processorIp, port, service);
        if(PathUtil.setZodeData(zk, zkPath, "status=enabled".getBytes()) == false) {
            if(logger.isWarnEnabled()) {
                logger.warn("set Zookeeper enabled node data error");
            }
            throw new TException("set Zookeeper enabled node data exception");
        }
    }
    
    private void disabledNodes() throws TException {      
        String processorIp = getLocalAddr();
        if(processorIp == null) {
            if(logger.isWarnEnabled()) {
                logger.warn("get processor ip error");
            }
            throw new TException("get processor ip exception");
        }
        String zkPath = getZkPath(serviceId, processorIp, port, service);
        if(PathUtil.setZodeData(zk, zkPath, "status=disabled".getBytes()) == false) {
            if(logger.isWarnEnabled()) {
                logger.warn("set Zookeeper disabled node data error");
            }
            throw new TException("set Zookeeper disabled node data exception");
        }
    }
    
    private String getZkPath(String processorServiceId, String processorIp, int processorPort,ServiceWatcher service) {
        StringBuilder zkPath = new StringBuilder();
        int groupId = service.getServiceGroup().getId();
        int shardId = service.getServiceGroup().getShard(0).getId();
        zkPath.append(PathUtil.serviceIdToPath(processorServiceId))
              .append('/').append(".service_group_").append(groupId)
              .append('/').append("shard_").append(shardId)
              .append('/').append(processorIp).append(":").append(processorPort);
        return zkPath.toString();
    }
    
    private IServiceBuilder getServiceBuilder(XoaServiceConfig conf) {
        if (conf.isXmqService())
            return new XmqServiceBuilder();

        return new ServiceBuilderImpl();
    }
}
