package com.xiaonei.commons.jade.impl.cache;

import java.util.HashMap;

import net.paoding.rose.jade.jadeinterface.cache.Cache;
import net.paoding.rose.jade.jadeinterface.cache.CacheProvider;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

/**
 * 提供 memcache 缓存池的 {@link CacheProvider} 实现。
 * 
 * @author han.liao
 */
public class MemCacheProvider implements CacheProvider {

    // 在 MemCacheKeys 中定义的缓存池名称
    public static String pool_default = "default";

    public static String pool_icode = "pool_icode";

    public static String pool_expday = "pool_expday"; // = "default"

    public static String pool_photo_upload = "pool_photo_upload";

    public static String pool_user_space = "pool_user_space"; // = "default"

    public static String pool_guide = "pool_guide"; // = "default"

    public static String pool_tribe = "pool_tribe"; // = "default"

    public static String pool_rand_code = "pool_rand_code";

    public static String pool_profile = "pool_profile"; // = "default"

    public static String pool_score = "pool_score";

    public static String pool_xnml_ref = "pool_xnml_ref";

    public static String pool_tyrant = "tyrant";

    public static String pool_apps = "pool_apps";

    public static String pool_tyrant_read = "tyrant_read";

    public static String pool_user_recent_apps = "pool_user_recent_apps";

    public static String pool_status_count = "pool_status_count";

    public static String pool_status = "pool_status";

    public static String pool_antirobot = "pool_antirobot";

    public static String pool_wap = "pool_wap";

    public static String pool_app_user = "pool_app_user";

    public static String pool_vip = "pool_vip";

    public static String pool_page = "pool_page";

    public static String pool_request_center = "pool_request_center";

    public static String pool_test = "pool_test";

    public static String pool_photo_test = "pool_photo_test";

    // 静态映射缓存池名称到 MemCacheKeys
    private static HashMap<String, MemCacheKeys> memCacheKeys = new HashMap<String, MemCacheKeys>();

    static {
        memCacheKeys.put(pool_default, MemCacheKeys.pool_default);
        memCacheKeys.put(pool_icode, MemCacheKeys.pool_icode);
        memCacheKeys.put(pool_expday, MemCacheKeys.pool_expday);
        memCacheKeys.put(pool_photo_upload, MemCacheKeys.pool_photo_upload);
        memCacheKeys.put(pool_user_space, MemCacheKeys.pool_user_space);
        memCacheKeys.put(pool_guide, MemCacheKeys.pool_guide);
        memCacheKeys.put(pool_tribe, MemCacheKeys.pool_tribe);
        memCacheKeys.put(pool_rand_code, MemCacheKeys.pool_rand_code);
        memCacheKeys.put(pool_profile, MemCacheKeys.pool_profile);
        memCacheKeys.put(pool_score, MemCacheKeys.pool_score);
        memCacheKeys.put(pool_xnml_ref, MemCacheKeys.pool_xnml_ref);
        memCacheKeys.put(pool_tyrant, MemCacheKeys.pool_tyrant);
        memCacheKeys.put(pool_apps, MemCacheKeys.pool_apps);
        memCacheKeys.put(pool_tyrant_read, MemCacheKeys.pool_tyrant_read);
        memCacheKeys.put(pool_user_recent_apps, MemCacheKeys.pool_user_recent_apps);
        memCacheKeys.put(pool_status_count, MemCacheKeys.pool_status_count);
        memCacheKeys.put(pool_status, MemCacheKeys.pool_status);
        memCacheKeys.put(pool_antirobot, MemCacheKeys.pool_antirobot);
        memCacheKeys.put(pool_wap, MemCacheKeys.pool_wap);
        memCacheKeys.put(pool_app_user, MemCacheKeys.pool_app_user);
        memCacheKeys.put(pool_vip, MemCacheKeys.pool_vip);
        memCacheKeys.put(pool_page, MemCacheKeys.pool_page);
        memCacheKeys.put(pool_request_center, MemCacheKeys.pool_request_center);
        memCacheKeys.put(pool_test, MemCacheKeys.pool_test);
        memCacheKeys.put(pool_photo_test, MemCacheKeys.pool_photo_test);
    }

    @Override
    public Cache getCacheByPool(String poolName) {

        MemCacheKeys poolKey = memCacheKeys.get(poolName);

        if (poolKey == null) {
            throw new IllegalArgumentException("Cache pool \'" + poolName + "\' not found.");
        }

        MemCacheFactory memCacheFactory = MemCacheFactory.getInstance();
        MemCacheManager memCacheManager = memCacheFactory.getManagerByPoolName(poolKey);

        return new MemCache(poolName, memCacheManager);
    }
}
