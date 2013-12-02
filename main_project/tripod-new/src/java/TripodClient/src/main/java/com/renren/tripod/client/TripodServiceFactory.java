package com.renren.tripod.client;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.renren.tripod.client.impl.TripodServiceImpl;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @date 2012-1-4 下午03:47:16
 */
public class TripodServiceFactory {
    
    private static final String ZK_ADRESS = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/Tripod";
    
    private static Map<String, TripodService<?>> serviceMap = new ConcurrentHashMap<String, TripodService<?>>();
    
    @SuppressWarnings({ "rawtypes", "unchecked" })
    public static <T> TripodService<T> getService(String namespace, String biz, Class<T> dataClass) {
        String key = namespace + biz + dataClass.getName();
        TripodService service = serviceMap.get(key);
        if (service == null) {
            synchronized (key.intern()) {
                service = serviceMap.get(key);
                if (service == null) {
                    TripodClient tripodClient = new TripodClient(ZK_ADRESS, namespace, biz);
                    TripodServiceImpl<T> serviceImpl = new TripodServiceImpl<T>();
                    serviceImpl.setTripodClient(tripodClient);
                    serviceImpl.setDataClass(dataClass);
                    serviceMap.put(key, serviceImpl);
                    
                    service = serviceImpl;
                }
            }
        }
        return service;
    }
    
}
