package com.renren.tripod.impl;

import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.tripod.TripodProducer;


/**
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2012-1-6 上午11:30:12
 */
public abstract class AbstractTripodProducer implements TripodProducer {

    protected final Log logger = LogFactory.getLog(this.getClass());
    
    private Map<String, Method> writeMethodMap = new ConcurrentHashMap<String, Method>();
    
    /* (non-Javadoc)
     * @see com.renren.tripod.TripodProducer#create(java.lang.String[], boolean)
     */
    @Override
    public Map<String, byte[]> create(String[] keys, boolean isMaster) {
        
        Map<String, Object> objectMap = createObjects(keys, isMaster);
        Map<String, byte[]> bytesMap = new HashMap<String, byte[]>(objectMap.size());
        
        Method writeToMethod = null;
        ByteArrayOutputStream baos = new ByteArrayOutputStream(512);
        for (Entry<String, Object> entry : objectMap.entrySet()) {
            if (writeToMethod == null) {
                writeToMethod = getWriteToMethod(entry.getValue().getClass());
            }
            try {
                writeToMethod.invoke(entry.getValue(), baos);
            } catch (Exception e) {
                logger.error("Error while serializing Object: " + entry.getKey(), e);
            }
            bytesMap.put(entry.getKey(), baos.toByteArray());
            baos.reset();
        }
        return bytesMap;
    }
    
    private Method getWriteToMethod(Class<?> klass) {
        
        String className = klass.getName();
        Method method = writeMethodMap.get(className);
        if (method == null) {
            synchronized (className.intern()) {
                method = writeMethodMap.get(className);
                if (method == null) {
                    try {
                        method = klass.getMethod("writeTo", OutputStream.class);
                        writeMethodMap.put(className, method);
                    } catch (Exception e) {
                        logger.error("", e);
                    }
                }
            }
        }
        return method;
    }
    
    public abstract Map<String, Object> createObjects(String[] keys, boolean isMaster);
    
}
