package com.renren.xcs;

import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

/**
 * 用于获取ExtendedConsumer和ExtendedProducer的工具类，保证一样的Configuration对象
 * 获取到同一个ExtendedConsumer或ExtendedProducer。
 * 
 * 说白了就是同样的配置获取同一个ZooKeeper链接。
 * 
 * @author Xun Dai <xun.dai@renren-inc.com>
 * 
 */
public class FindFactorys {

    private static ConcurrentMap<Configuration, ExtendedConsumer> consumerCache = new ConcurrentHashMap<Configuration, ExtendedConsumer>();

    private static ConcurrentMap<Configuration, ExtendedProducer> producerCache = new ConcurrentHashMap<Configuration, ExtendedProducer>();
    
    /**
     * Producer 超时时间设定<br>
     * 
     * 由于service的注销操作需要快速得到反馈，因此设定时间较短，仍需要仔细关注和调节
     * 
     * @author liuyuan
     */
    private static final int PRODUCER_TIMEOUT = 5000;
    
    /**
     * Consumer 超时时间设定
     *
     * @author liuyuan
     */
    private static final int CONSUMER_TIMEOUT = 20 * 1000;

    /**
     * 获取ExtendedConsumer
     * 
     * @param configuration
     * @return
     * @throws IOException
     */
    public static ExtendedConsumer getConsumer(Configuration configuration)
            throws IOException {
        ExtendedConsumer consumer = consumerCache.get(configuration);
        if (consumer == null) {
            synchronized (configuration) {
                consumer = consumerCache.get(configuration);
                if (consumer == null) {
                    consumer = new ExtendedConsumer(configuration.getCluster(), CONSUMER_TIMEOUT);
                    consumer.addAuthInfo(configuration.getClientUsername(), configuration.getClientPassword());
                    consumer.setRoot(configuration.getRoot());
                    consumerCache.put(configuration, consumer);
                }
            }
        }
        return consumer;
    }

    /**
     * 获取ExtendedProducer
     * @param configuration
     * @return
     * @throws IOException
     */
    public static ExtendedProducer getProducer(Configuration configuration)
            throws IOException {

        ExtendedProducer producer = producerCache.get(configuration);
        if (producer == null) {
            synchronized (configuration) {
                producer = producerCache.get(configuration);
                if (producer == null) {
                    producer = new ExtendedProducer(configuration.getCluster(), PRODUCER_TIMEOUT);
                    producer.addAuthInfo(configuration.getServerUsername(), configuration.getServerPassword());
                    producer.setRoot(configuration.getRoot());
                    producerCache.put(configuration, producer);
                }
            }
        }
        return producer;
    }
}
