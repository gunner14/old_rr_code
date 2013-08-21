package com.xiaonei.jade.cache;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import net.paoding.rose.jade.statement.StatementMetaData;
import net.paoding.rose.jade.statement.cached.CacheInterface;
import net.paoding.rose.jade.statement.cached.CacheProvider;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.util.Assert;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

/**
 * 提供 memcache 缓存池的 {@link CacheProvider} 实现。
 * 
 * @author han.liao
 */
public class MemCacheProvider implements CacheProvider {

    private Log logger = LogFactory.getLog(MemCacheProvider.class);

    // 静态映射缓存池名称到 MemCacheKeys
    private Map<String, MemCacheKeys> memCacheKeys = Collections.emptyMap();

    public MemCacheProvider() {
        freshConstaintsFromMemecheKeys();
    }

    @Override
    public CacheInterface getCacheByPool(StatementMetaData metaData,String poolName) {
        MemCacheKeys poolKey = memCacheKeys.get(poolName);
        if (poolKey == null) {
            throw new IllegalArgumentException("Cache pool \'" + poolName + "\' not found.");
        }

        MemCacheFactory memCacheFactory = MemCacheFactory.getInstance();
        MemCacheManager memCacheManager = memCacheFactory.getManagerByPoolName(poolKey);

        return new MemCache(poolName, memCacheManager);
    }

    protected void freshConstaintsFromMemecheKeys() {
        HashMap<String, MemCacheKeys> memCacheKeys = new HashMap<String, MemCacheKeys>();
        Field fields[] = MemCacheKeys.class.getDeclaredFields();
        for (Field field : fields) {
            int modifiers = field.getModifiers();
            if (!Modifier.isStatic(modifiers) || !Modifier.isPublic(modifiers)) {
                continue; // 忽略非static、public常量
            }
            if (field.getType() != MemCacheKeys.class) {
                continue; // 忽略非MemCacheKeys变量
            }

            try {
                if (field.isAccessible()) {
                    field.setAccessible(true);
                }

                MemCacheKeys key = (MemCacheKeys) field.get(null);
                memCacheKeys.put(key.getPoolName(), key);
                if (logger.isDebugEnabled()) {
                    logger.debug("found MemCacheKeys(by poolName): " + key.getPoolName() + "->"
                            + key.getPoolName());
                }
                if (!key.getPoolName().equals(field.getName())) {
                    memCacheKeys.put(field.getName(), key);
                    if (logger.isDebugEnabled()) {
                        logger.debug("found MemCacheKeys(by field.name):  " + field.getName()
                                + "->" + key.getPoolName());
                    }
                }
            } catch (SecurityException e) {
                logger.warn("", e);
            } catch (IllegalAccessException e) {
                logger.warn("", e);
            }
        }
        Assert.isTrue(memCacheKeys.size() > 0, "fail to found MemCacheKeys constaints"
                + " from class 'com.xiaonei.platform.core.cache.MemCacheKeys'");
        //
        this.memCacheKeys = memCacheKeys;
    }

}
