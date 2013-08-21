package com.renren.xoa2.client;

import java.util.NoSuchElementException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import java.util.Collection;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.thrift.transport.TTransport;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.client.transport.ConnectionProvider;
import com.renren.xoa2.client.transport.TTransportConnectionProvider;
import com.renren.xoa2.client.transport.XoaTransport;
import com.renren.xoa2.registry.Node;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Dec 1, 2011 7:01:12 PM
 */
public class CommonServiceRouter implements ServiceRouter {
	private Logger logger = LoggerFactory.getLogger(CommonServiceRouter.class);

    private static CommonServiceRouter instance = new CommonServiceRouter();
    
    private ConnectionProvider connectionProvider = new TTransportConnectionProvider();
    
    private LoadBalancer loadBalancer = new RoundRobinLoadBalancer();
    private CommonServiceRouter(){
        Timer timer = new Timer();
        timer.schedule(new DisabledNodeChecker(), 100);
    }
    public static CommonServiceRouter getInstance(){
        return instance;
    }
    @Override
    public XoaTransport routeService(String serviceId,String shardBy){
        Node node = null;
        //没有shardBy参数，调用随即负载均衡
        if(shardBy==null||shardBy.equals(""))
            node = loadBalancer.getNode(serviceId);
        //有shardBy方式的情况,特殊处理
        else {
            node = loadBalancer.getNode(serviceId, Integer.parseInt(shardBy));
        }
        
        // 没有可用的node，这是严重的错误
        if (node == null) {
        	return null;
        }
        // 出错时，也创建xoaTransport, 以保存node信息
        XoaTransport xoaTransport = new XoaTransport();
        xoaTransport.setNode(node);
        try {
            TTransport transport= (TTransport) connectionProvider.getConnection(node);
            xoaTransport.setTransport(transport);
            if(transport == null)
                xoaTransport.setDisabled(true);
        } catch (NoSuchElementException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IllegalStateException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return xoaTransport;
    }
    @Override
    public XoaTransport reRouteService(String serviceId,String shardBy){
        Node node;
        if(shardBy==null||shardBy.equals(""))
            //没有shardBy参数，调用随即负载均衡
            node = loadBalancer.getNode(serviceId);
        else {
            //有shardBy方式的情况,特殊处理
            node = loadBalancer.getNode(serviceId, Integer.parseInt(shardBy));
        }
        try {
            TTransport transport= (TTransport) connectionProvider.getConnection(node);
            XoaTransport xoaTransport = new XoaTransport();
            xoaTransport.setTransport(transport);
            if(transport == null)
                xoaTransport.setDisabled(true);
            xoaTransport.setNode(node);
            return xoaTransport;
        } catch (NoSuchElementException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IllegalStateException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return null;
    }

    private static String getNodeKey(Node node) {
		return node.getHost() + ':'+ node.getPort(); 
    }

	@Override
	public void returnConn(XoaTransport xoaTransport) {
		errorMap.remove(getNodeKey(xoaTransport.getNode()));
		connectionProvider.returnCon(xoaTransport);
	}
	
	/**
	 * 设定获取到连接的超时时间
	 * 
	 * @param conTimeOut
	 */
	public void setTimeOut(long conTimeOut) {
		TTransportConnectionProvider.setTimeOut((int)conTimeOut);
	}

    /** 
     * 设定loadBalancer，用于测试
     * 
     * @param loadBalancer 
     */
	void setLoadBalancer(LoadBalancer loadBalancer) {
		this.loadBalancer = loadBalancer;
    }
    
	private final static int DISABLE_THRESHOLD = 10; 
    private ConcurrentMap<String, NodeErrorInfo> errorMap =  new ConcurrentHashMap<String, NodeErrorInfo>();
    
    class NodeErrorInfo {
    	private int count;
    	private Node node;
    	NodeErrorInfo(Node node, int count) {
    		this.count = count;
    		this.node = node;
    	}
    	void incCount() {
    		++count;
    	}
    	Node getNode() {
    		return node;
    	}
    	int getCount() {
    		return count;
    	}
    	void setCount(int count) {
    		this.count = count;
    	}
    }
    
    public final class DisabledNodeChecker extends TimerTask {
    	@Override
    	public void run(){
    		logger.info("retry disabled nodes");
    		try {
	    		Collection<NodeErrorInfo> errs = errorMap.values();
	    		for (NodeErrorInfo err : errs) {
	    			if (err.getCount() >= DISABLE_THRESHOLD 
	    					&& err.getNode() != null) {
	    				err.getNode().setDisabled(false);
	    				logger.warn("enable node " + getNodeKey(err.getNode()) + " to retry");
	    				err.setCount(DISABLE_THRESHOLD - 1);
	    			}
	    		}
    		} finally {
				Timer timer = new Timer();
				timer.schedule(new DisabledNodeChecker(), 20 * 1000);
    		}
    	}
    }
        
    @Override
    public void serviceException(Exception e, XoaTransport xoaTransport) {
    	String signature = "no-service-available";
    	Node node = null;
    	if (xoaTransport != null) {
        	node = xoaTransport.getNode();
    		signature = getNodeKey(node); 
    	}

		logger.error("RPC error : " + signature);

    	NodeErrorInfo err = errorMap.get(signature);    
    	
    	if (err == null) {
    		errorMap.put(signature, new NodeErrorInfo(node, 1));
    	} else {
    		err.incCount();    		
    	}
    	
    	if (node != null && err.getCount() >= DISABLE_THRESHOLD) {
    		node.setDisabled(true);
    	}    	
    }
}
