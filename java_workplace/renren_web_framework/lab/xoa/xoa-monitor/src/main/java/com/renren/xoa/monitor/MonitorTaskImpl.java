package com.renren.xoa.monitor;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xoa.Method;
import com.renren.xoa.StatusNotOkException;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.client.service.SpdyClientManager;
import com.renren.xoa.registry.XoaRegistry;
import com.renren.xoa.registry.XoaRegistryFactory;
import com.renren.xoa.registry.XoaService;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-23 下午05:50:08
 */
public class MonitorTaskImpl implements MonitorTask {

    private Log logger = LogFactory.getLog(this.getClass());
    
    private XoaRegistry registry = XoaRegistryFactory.getInstance().getDefaultRegistry();

    private Set<String> connectedHosts = new HashSet<String>();
    
    private List<MonitorResult> checkService(XoaService service) {
        List<MonitorResult> resList = new ArrayList<MonitorResult>(); 
        for (XoaServiceDescriptor node : service.getNodes()) {
            MonitorResult res = new MonitorResult();
            res.setServiceId(service.getServiceId());
            res.setIp(node.getIpAddress());
            res.setPort(node.getPort());
            res.setDisabled(node.isDisabled());
            
            res.setStatus(pingNode(node));
            
            connectedHosts.add(node.getIpAddress() + ":" + node.getPort());
            
            resList.add(res);
        }
        return resList;
    }

    private String pingNode(XoaServiceDescriptor node) {
        if (node.isDisabled()) {
            return "None";
        }
        SimpleXoaRegistry registry = new SimpleXoaRegistry();
        registry.register(node.getServiceId(), node.getIpAddress(), node.getPort());
        XoaClient client = new XoaClient(registry);
        Method method = Method.get(getHeathyInfoUrl(node.getServiceId()));
        
        //先调用一次，建立起连接，防止第一次建立连接是耗时过多
        /*try {
            client.execute(method, 5000);
        } catch (Exception e) {
            logger.error("error while trying to connect remote server", e);
        }*/
        
        String status;
        try {
            Future<XoaResponse> future = client.submit(method);
            try {
                XoaResponse response = future.get(50, TimeUnit.MILLISECONDS);
                if ("OK".equals(response.getContentAsString())) {
                    status = "OK";
                } else {
                    status = "UNEXPECTED_RETURN";
                }
            } catch (StatusNotOkException e) {
                XoaResponse response = e.getResponse();
                status = "" + response.getStatusCode();
            } catch (Exception e) {
                logger.error("", e);
                status = "TIMEOUT";
            }
        } catch (Exception e) {
            status = "DOWN";
        }
        
       /* try {
            XoaResponse response = client.execute(method, 5000);
            //System.out.println(node + "-" + response.getContentAsString());
        } catch (Exception e) {
            //e.printStackTrace();
        }*/
        
        return status;
    }
    
    private String getHeathyInfoUrl(String serviceId) {
        return "xoa://" + serviceId + "/xoa-info/healthy";
    }
    
    @Override
    public List<MonitorResult> monitorAll() {
        List<MonitorResult> resList = new ArrayList<MonitorResult>(); 
        List<XoaService> services = registry.getServices();
        for (XoaService service : services) {
            resList.addAll(checkService(service));
        }
        return resList;
    }

    @Override
    public void destroy() {
        for (String hostport : connectedHosts ) {
            SpdyClientManager.getInstance().destroyClient(hostport);
        }
    }

    private void initServices() {
        List<XoaService> services = registry.getServices();
        for(XoaService svc : services) {
            registry.queryService(svc.getServiceId());
        }
    }
    
    @Override
    public List<MonitorResult> monitor(String ip, int port) {
        
        //空调用一次，是为了获取配置，需要优化
        initServices();
        
        List<String> serviceIds = registry.lookup(ip, port);
        List<MonitorResult> res = new ArrayList<MonitorResult>();
        for (String serviceId : serviceIds) {
            XoaService service = new XoaService();
            service.setServiceId(serviceId);
            ArrayList<XoaServiceDescriptor> nodes = new ArrayList<XoaServiceDescriptor>(1);
            XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
            desc.setServiceId(serviceId);
            desc.setIpAddress(ip);
            desc.setPort(port);
            nodes.add(desc);
            service.setNodes(nodes);
            res.addAll(checkService(service));
        }
        return res;
    }
    
}
