package com.renren.xoa2.client;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import java.util.Collection;
import java.util.Enumeration;
import java.util.List;
import java.util.Date;

import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.client.transport.ConnectionProvider;
import com.renren.xoa2.client.transport.TTransportConnectionProvider;
import com.renren.xoa2.client.transport.XoaTransport;
import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.XoaRegistry;
import com.renren.xoa2.registry.XoaRegistryFactory;
import com.renren.xoa2.sms.LanTaoSmsService;

import com.renren.xoa2.exception.XoaTransportException;
import com.renren.xoa2.exception.XoaClientPoolException;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since Dec 1, 2011 7:01:12 PM
 */
public class CommonServiceRouter implements ServiceRouter {
    private Logger logger = LoggerFactory.getLogger(CommonServiceRouter.class);

    private static CommonServiceRouter instance = new CommonServiceRouter();

    private ConnectionProvider connectionProvider = new TTransportConnectionProvider();

    private LoadBalancer loadBalancer = new RoundRobinLoadBalancer();
    ScheduledThreadPoolExecutor timerExecutor = new ScheduledThreadPoolExecutor(
            4);

    private XoaRegistry xoaRegistry = XoaRegistryFactory.getInstance()
            .getDefaultRegistry();

    private final static int DISABLE_THRESHOLD = 1000;
    private final static int MAX_GET_RETRY = 1;

    private static String localIp = getLocalAddr();

    private static String getLocalAddr() {
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

    private CommonServiceRouter() {
        timerExecutor.scheduleAtFixedRate(new Runnable() {
            public void run() {
                try {
                    Collection<NodeErrorInfo> errs = errorMap.values();
                    for (NodeErrorInfo err : errs) {
                        Node n = err.getNode();
                        if (n == null) {
                            continue;
                        }
                        String nodeKey = getNodeKey(n);
                        if (!n.isDisabled() && (err.getCount() >= DISABLE_THRESHOLD
                                || !n.isHealthy())) {
                            // 只有连接成功，才拿实际业务来重试
                            try {
                                Socket sock = new Socket();
                                SocketAddress endpoint = new InetSocketAddress(
                                        n.getHost(), n.getPort());
                                sock.connect(endpoint, 500);
                                sock.close();
                            } catch (Exception e) {
                                smsNotify(err.getServiceId(), nodeKey,
                                        "connect fail");
                                logger.error("Disabled xoa2 node "
                                        + nodeKey
                                        + " connect fail before retry");
                                continue;
                            }
                            n.setHealthy(true);
                            err.setCount(0);
                            logger.error("Retry disabled xoa2 node " + nodeKey);
                        } else {
                            if (logger.isDebugEnabled()) {
                                logger.debug("Check " + nodeKey 
                                        + " healthy=" + n.isHealthy() 
                                        + " disabled=" + n.isDisabled() 
                                        + " err=" + err.getCount() + "/" + DISABLE_THRESHOLD);
                            }
                        }
                    }
                    // } catch (Exception e) {
                } catch (Throwable e) {
                }
            }
        }, 3, 3, TimeUnit.SECONDS);
    }

    public static CommonServiceRouter getInstance() {
        return instance;
    }
    
    public boolean isValidServiceId(String id) {
        if (id == null || id.isEmpty()) {
            logger.error("null service id!");
            return false;
        }
        
        if (!id.endsWith(".xoa.renren.com")) {
            return false;
        }
        
        if (id.split("\\.").length != 5) {
            return false;
        }
        return true;
    }
    
    @Override
    public XoaTransport routeService(String serviceId, String shardBy)
            throws Exception {
        if (!isValidServiceId(serviceId)) {
            logger.error("bad service id : " + serviceId);
            throw new XoaClientPoolException("bad service id : " + serviceId);
        }
        Node node = null;
        int retry = 0;
        TTransport transport = null;
        XoaTransport xoaTransport = null;

        while (true) {
            // 没有shardBy参数，调用随即负载均衡
            if (shardBy == null || shardBy.equals("")) {
                node = loadBalancer.getNode(serviceId);
            } else {
                node = loadBalancer.getNode(serviceId,
                        Integer.parseInt(shardBy));
            }

            // 没有可用的node，这是严重的错误
            if (node == null) {
                logger.error("No endpoint available : " + serviceId);
                return null;
            }
            // 出错时，也创建xoaTransport, 以保存node信息
            xoaTransport = new XoaTransport();
            xoaTransport.setNode(node);

            try {
                transport = (TTransport)connectionProvider.getConnection(node);
                break;
            } catch (Exception e) {
                disableNode(serviceId, node, DISABLE_THRESHOLD / 200);
                logger.error("Get service error : " + getNodeKey(node)
                        + ' ' + serviceId);

                if (++retry >= MAX_GET_RETRY) {
                    throw new XoaClientPoolException("service error : " + getNodeKey(node)
                            + ' ' + serviceId);
                }
            }
        }
        if (logger.isDebugEnabled()) {
           logger.debug("get service ok : " + serviceId);
        }
        xoaTransport.setTransport(transport);
        return xoaTransport;
    }

    private static String getNodeKey(Node node) {
        if (node == null) {
            return "null-endpoint";
        }
        return node.getHost() + ':' + node.getPort();
    }

    @Override
    public void returnConn(XoaTransport xoaTransport) throws Exception {
        String signature = getNodeKey(xoaTransport.getNode());
        NodeErrorInfo err = errorMap.get(signature);
        if (err != null) {
            err.setCount(0);
        }

        connectionProvider.returnConnection(xoaTransport);
    }

    /**
     * 设定获取到连接的超时时间
     * 
     * @param connTimeout
     */
    public void setTimeout(long connTimeout) {
        TTransportConnectionProvider.setTimeout((int) connTimeout);
    }

    /**
     * 设定loadBalancer，用于测试
     * 
     * @param loadBalancer
     */
    void setLoadBalancer(LoadBalancer loadBalancer) {
        this.loadBalancer = loadBalancer;
    }

    private ConcurrentMap<String, NodeErrorInfo> errorMap = new ConcurrentHashMap<String, NodeErrorInfo>();

    class NodeErrorInfo {
        private String serviceId;
        private int count;
        private Node node;

        NodeErrorInfo(String serviceId, Node node, int count) {
            this.serviceId = serviceId;
            this.count = count;
            this.node = node;
        }

        void addCount(int delta) {
            count += delta;
        }

        String getServiceId() {
            return serviceId;
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

    private ConcurrentMap<String, Long> smsTimestampMap = new ConcurrentHashMap<String, Long>();

    private void smsNotify(String serviceId, String node, String desc) {
        Long lastSentTime = smsTimestampMap.get(serviceId);
        Long currentTime = new Date().getTime();

        if (lastSentTime != null && (currentTime - lastSentTime < 180000)) {
            return;
        }
        smsTimestampMap.put(serviceId, currentTime);

        LanTaoSmsService svc = new LanTaoSmsService();
        List<String> servicePhones = xoaRegistry.queryAlarmPhones(serviceId);
        if (servicePhones != null) {
            String msg = "xoa2.0-" + serviceId + "-client=" + localIp
                    + "-service=" + node + "-msg=" + desc;
            for (String phone : servicePhones) {
                svc.sendSms(phone, node, serviceId, "down", msg);
            }
        }
    }

    void disableNode(String serviceId, Node node, int delta) {
        if (node == null) {
            return;
        }
        String signature = getNodeKey(node);
        NodeErrorInfo err = errorMap.get(signature);

        connectionProvider.clearConnections(node);

        if (err == null) {
            err = new NodeErrorInfo(serviceId, node, delta);
            errorMap.put(signature, err);
        } else {
            err.addCount(delta);
        }

        logger.warn(signature + " disabled " + err.getCount() + '/'
                + DISABLE_THRESHOLD);
        if (node != null && err.getCount() >= DISABLE_THRESHOLD) {
            node.setHealthy(false);
            smsNotify(serviceId, signature, "service disabled");
        }
    }

    @Override
    public void serviceException(String serviceId, Throwable e,
            XoaTransport xoaTransport) {
        String signature = "null-service";
        Node node = null;
        if (xoaTransport != null) {
            node = xoaTransport.getNode();
            signature = getNodeKey(node);
            if (logger.isDebugEnabled()) {
                logger.debug("invalidate addr=" + signature + ",prov="
                    + connectionProvider + ",xoaTransport=" + xoaTransport);
            }
            connectionProvider.invalidateConnection(xoaTransport);
        }

        int delta = 1;

        if (e instanceof TTransportException) {
            Throwable cause = e.getCause();
            if (cause == null) {
                int type = ((TTransportException) e).getType();
                switch (type) {
                case TTransportException.END_OF_FILE:
                    logger.error("xoa2 service=" + serviceId  + " addr=" + signature 
                        + " ex=" + "RPC TTransportException END_OF_FILE");
                    delta = DISABLE_THRESHOLD / 200;
                    break;
                default:
                    logger.error("xoa2 service=" + serviceId  + " addr=" + signature 
                        + " ex=" + "RPC TTransportException default");
                    delta = DISABLE_THRESHOLD / 200;
                    break;
                }
            } else {
                if (cause instanceof java.net.SocketTimeoutException) {
                    delta = DISABLE_THRESHOLD / 500;
                    logger.error("xoa2 service=" + serviceId  + " addr=" + signature 
                        + " ex=" + "RPC TTransportException SocketTimeoutException");
                } else {
                    delta = DISABLE_THRESHOLD / 200;
                    logger.error("xoa2 service=" + serviceId  + " addr=" + signature 
                        + " ex=" + "RPC TTransportException " + cause.getMessage());
                }
            }
        } else if (e instanceof XoaTransportException) {
            delta = DISABLE_THRESHOLD / 200;
            logger.error("xoa2 service=" + serviceId  + " addr=" + signature 
                + " ex=" + "XoaTransportException " + e.getMessage());
        } else if (e instanceof XoaClientPoolException) {
            logger.error("xoa2 service=" + serviceId  + " addr=" + signature 
                + " ex=" + "XoaClientPoolException");
            delta = 1;
        }

        if (delta <= 0) {
            delta = 1;
        }

        disableNode(serviceId, node, delta);
    }
}
