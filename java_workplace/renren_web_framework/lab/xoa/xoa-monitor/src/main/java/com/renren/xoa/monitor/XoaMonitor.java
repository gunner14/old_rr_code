package com.renren.xoa.monitor;

import java.util.List;

import com.renren.xoa.Method;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.registry.XoaRegistry;
import com.renren.xoa.registry.XoaRegistryFactory;
import com.renren.xoa.registry.XoaService;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-8-25 下午03:21:10
 */
public class XoaMonitor {

    private XoaRegistry registry = XoaRegistryFactory.getInstance().getDefaultRegistry();
    
    public void test() {
        
        List<XoaService> services = registry.getServices();
        for (XoaService service : services) {
            checkService(service);
        }
    }

    private void checkService(XoaService service) {
        for (XoaServiceDescriptor node : service.getNodes()) {
            pingNode(node);
        }
    }

    private void pingNode(XoaServiceDescriptor node) {
        
        if (node.isDisabled()) {
            return;
        }
        SimpleXoaRegistry registry = new SimpleXoaRegistry();
        registry.register(node.getServiceId(), node.getIpAddress(), node.getPort());
        XoaClient client = new XoaClient(registry);
        Method method = Method.get(getHeathyInfoUrl(node.getServiceId()));
        
        try {
            XoaResponse response = client.execute(method, 5000);
            System.out.println(node + "-" + response.getContentAsString());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    private String getHeathyInfoUrl(String serviceId) {
        return "xoa://" + serviceId + "/xoa-info/healthy";
    }
    
    public static void main(String[] args) {
        new XoaMonitor().test();
    }
}
