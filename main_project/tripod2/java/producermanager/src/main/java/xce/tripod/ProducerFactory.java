package xce.tripod;

import java.io.File;
import java.text.ParseException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import org.json.JSONArray;
import org.json.JSONObject;

import xce.tripod.util.TextFileUtil;

/**
 * ProducerFactory
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class ProducerFactory {

    private static ProducerFactory instance = null;

    private ConcurrentHashMap<String, Producer> producerMap = new ConcurrentHashMap<String, Producer>();

    private ReentrantReadWriteLock rwLock = new ReentrantReadWriteLock();

    private static final int LOAD_INTERVAL = 30; //seconds

    class LoadTask implements Runnable {

        @Override
        public void run() {
            loadProdecerMap();
        }
    }

    private ProducerFactory() {
        loadProdecerMap();
        ScheduledExecutorService service = Executors.newScheduledThreadPool(1);
        service.scheduleAtFixedRate(new LoadTask(), LOAD_INTERVAL, LOAD_INTERVAL, TimeUnit.SECONDS);
    }

    public static ProducerFactory getInstance() {
        if (instance == null) {
            synchronized (ProducerFactory.class) {
                if (instance == null) {
                    instance = new ProducerFactory();
                }
            }
        }
        return instance;
    }

    public Producer get(String biz) {
        return producerMap.get(biz);
    }

    public boolean add(String biz, Producer producer) {
        if (producer == null) {
            return false;
        }
        producerMap.put(biz, producer);
        return true;
    }

    public void loadProdecerMap() {
        WriteLock wLock = rwLock.writeLock();
        try {
            wLock.lock();
            ConcurrentHashMap<String, Producer> newProducerMap = new ConcurrentHashMap<String, Producer>();
            File jsonFile = new File("../etc/producer.json");
            if (!jsonFile.exists()) {
                System.err.println("../etc/producer.json doesn't exist!!!");
                return;
            }
            String jsonString = TextFileUtil.read(jsonFile);
            try {
                JSONArray jsonArray = new JSONArray(jsonString);
                for (int i = 0; i < jsonArray.length(); ++i) {
                    JSONObject jsonObj = (JSONObject) jsonArray.get(i);
                    String name = jsonObj.getString("name");
                    String classString = jsonObj.getString("class");
                    System.out.println("name:" + name + "\tclass:" + classString);
                    try {
                        Class<?> c = Class.forName(classString);
                        Producer producer = (Producer) c.newInstance();
                        newProducerMap.put(name, producer);
                    } catch (ClassNotFoundException e) {
                        e.printStackTrace();
                    } catch (InstantiationException e) {
                        e.printStackTrace();
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    }
                }
            } catch (ParseException e) {
                e.printStackTrace();
            }
            if (!newProducerMap.isEmpty()) {
                producerMap.clear();
                producerMap.putAll(newProducerMap);
                System.out.println("Loaded Producer List :" + producerMap);
            }
        } finally {
            wLock.unlock();
        }
    }
}
