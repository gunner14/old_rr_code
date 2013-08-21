package com.xiaonei.commons.interceptors.cache;

import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.util.Assert;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

/**
 * @author lookis (comic.liu@gmail.com)
 * 
 */
public class ResultCacheInterceptor extends ControllerInterceptorAdapter implements
        InitializingBean {

    @Autowired
    private HostHolder hostHolder;

    private int priority = -10;

    // 静态映射缓存池名称到 MemCacheKeys
    private Map<String, MemCacheKeys> memCacheKeys = Collections.emptyMap();

    @Override
    protected Class<? extends Annotation> getRequiredAnnotationClass() {
        return ResultCache.class;
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        freshConstaintsFromMemecheKeys();
    }

    @Override
    public int getPriority() {
        return priority;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        ResultCache resultCache = inv.getMethod().getAnnotation(ResultCache.class);
        if (resultCache.timeout() == 0) {
            //不缓存
            return super.before(inv);
        } else {
            inv.setRequest(new RequestForCacheWrapper(inv.getRequest()));//防止有人把数据放入Request对象的Attribute属性的log
            try {
                //查询是否有缓存
                MemCacheManager manager = MemCacheFactory.getInstance().getManagerByPoolName(
                        getPool(resultCache.cachePool()));
                Object result = manager.get(getKey(inv));
                if (result == null) {//走正常流程
                    logger.info("Cache Miss:" + getKey(inv));
                    return super.before(inv);
                } else {
                    logger.info("Cache Hit:" + getKey(inv));
                    CacheObject co = (CacheObject) result;
                    inv.getModel().merge(co.getModels());
                    return co.getInstruction();
                }
            } catch (Exception e) {
                logger.debug(e);
                return super.before(inv);
            }
        }

    }

    @Override
    public Object after(Invocation inv, Object instruction) throws Exception {
        ResultCache resultCache = inv.getMethod().getAnnotation(ResultCache.class);
        if (resultCache.timeout() == 0) {
            return super.after(inv, instruction);
        } else {
            //只缓存Model和instruction里的数据
            Map<String, Object> models = new HashMap<String, Object>(inv.getModel().getAttributes());
            CacheObject co = new CacheObject(instruction, models);
            try {
                MemCacheManager manager = MemCacheFactory.getInstance().getManagerByPoolName(
                        getPool(resultCache.cachePool()));
                manager.asyncSet(getKey(inv), co, (int) resultCache.timeUnit().toSeconds(
                        resultCache.timeout()));
                return instruction;
            } catch (Exception e) {
                logger.debug(e);
                return super.after(inv, instruction);
            }
        }
    }

    private String getKey(Invocation inv) {

        int hostId = hostHolder.getUser() == null ? 0 : hostHolder.getUser().getId();
        return ResultCacheInterceptor.class.getName() + "_" + inv.getMethod().getName() + "_"
                + hostId;
    }

    /**
     * 
     * 
     * @return MemCacheKeys in {@link MemCacheKeys}
     * 
     */
    private MemCacheKeys getPool(String poolName) {
        MemCacheKeys poolKey = memCacheKeys.get(poolName);

        if (poolKey == null) {
            throw new IllegalArgumentException("Cache pool \'" + poolName + "\' not found.");
        }
        return poolKey;
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
