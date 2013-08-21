package com.renren.sns.wiki.utils.cache;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xcache.XCache;

/**
 * 提供一个对{@link XCache}的封装
 * 
 * @author yi.li@renren-inc.com since 2012-3-22
 * 
 */
public class CacheDelegator {

    public static final String TN_SEPERATOR = ".";

    private static Log logger = LogFactory.getLog(CacheDelegator.class);

    private static String genXCachePrefix(CacheNameSpace namespace, CachePrefix prefix) {
        return namespace.getNamespace() + TN_SEPERATOR + prefix.getPrefix();
    }

    private static <T> XCache<T> getXCache(CacheNameSpace namespace, CachePrefix prefix,
            Class<T> clazz) {
        return getXCache(namespace, prefix, clazz, false);
    }

    private static <T> XCache<T> getXCache(CacheNameSpace namespace, CachePrefix prefix,
            Class<T> clazz, boolean isCounter) {
        String xcachePrefix = genXCachePrefix(namespace, prefix);
        XCache<T> xcache = XCache.getCache(xcachePrefix, clazz, isCounter);
        return xcache;
    }

    /**
     * 从cache中获取名字空间为namespace，业务前缀为prefix，对象id为{@code id}的对象
     * 
     * @param <T> Memcache中存的对象的类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param clazz Memcache中存的对象的class
     * @param id 资源id（如blogId）
     * @return Cache中存储的对象，如果cache中没有的话返回null
     */
    public static <T> T get(CacheNameSpace namespace, CachePrefix prefix, Class<T> clazz, Object id) {
        QueryAction<T> query = new QueryAction<T>(namespace, prefix, clazz, id) {

            @Override
            public T getFromOther() {
                return null;
            }
        };

        return query.queryCache();
    }

    /**
     * 从cache中获取名字空间为namespace，业务前缀为prefix，对象id为{@code id}的对象
     * 如果没有从cache中获取到对象将返回defaultValue
     * 
     * @param <T> Memcache中存的对象的类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param clazz Memcache中存的对象的class
     * @param id 资源id（如blogId）
     * @param defaultValue 默认值
     * @return Cache中存储的对象，如果cache中没有的话返回defaultValue
     */
    public static <T> T get(CacheNameSpace namespace, CachePrefix prefix, Class<T> clazz,
            Object id, T defaultValue) {
        T value = get(namespace, prefix, clazz, id);
        if (value == null) {
            return defaultValue;
        }
        return value;
    }

    /**
     * 根据查询操作{@link QueryAction}去从Cache中获取对象，在cache miss的情况下调用<br>
     * {@link QueryAction#getFromOther()}
     * 从其他数据源获取数据，如果从其他源获取数据则将数据set进memcache，<br>
     * 如果仍没有获取到数据那么将{@code defaultValue}返回给调用者
     * 
     * @param <T> 要获取的对象的类型
     * @param queryAction 查询操作
     * @param defaultValue 默认值
     * @return 类型为T的对象，如果获取的对象为null，则返回defaultValue
     */
    public static <T> T get(QueryAction<T> queryAction, T defaultValue) {
        T value = queryAction.queryCache();
        if (value == null) {
            return defaultValue;
        }
        return value;
    }

    /**
     * 从memcache中获取多个对象
     * 
     * @param <K> Key类型
     * @param <V> 对象类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param clazz Memcache中存的对象的class
     * @param ids id列表
     * @return 一个HashMap，如果其中有id不在memcache中，从返回的数据中获取这个id的对象会返回null
     */
    public static <K, V> Map<String, V> getMulti(CacheNameSpace namespace, CachePrefix prefix,
            Class<V> clazz, List<K> ids) {
        MultiQueryAction<K, V> query = new MultiQueryAction<K, V>(namespace, prefix, clazz, ids) {

            @Override
            public V getFromOther(K id) {
                return null;
            }
        };

        return query.queryCache();
    }

    /**
     * 从memcache中获取多个对象
     * 
     * @param <V> 对象类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param clazz Memcache中存的对象的class
     * @param ids id列表
     * @return 一个大小与ids大小相同的数组，如果位于list中第i位的对象没有获取到数据 那么返回的数组中存下的值是null
     */
    public static <K, V> V[] getMultiAsArray(CacheNameSpace namespace, CachePrefix prefix,
            Class<V> clazz, List<K> ids) {
        MultiQueryAction<K, V> query = new MultiQueryAction<K, V>(namespace, prefix, clazz, ids) {

            @Override
            public V getFromOther(K id) {
                return null;
            }
        };

        return query.queryCacheAsArray();
    }

    /**
     * 从memcache中获取多个对象
     * 
     * @param <V> 对象类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param clazz Memcache中存的对象的class
     * @param ids id列表
     * @return 一个大小与ids大小相同的List，如果位于list中第i位的对象没有获取到数据
     *         那么返回的List中相应的位置存下的值是null
     */
    public static <K, V> List<V> getMultiAsList(CacheNameSpace namespace, CachePrefix prefix,
            Class<V> clazz, List<K> ids) {
        V[] values = getMultiAsArray(namespace, prefix, clazz, ids);

        return Arrays.asList(values);
    }

    /**
     * 根据查询操作{@link MultiQueryAction}去从Cache中批量获取数据，在cache miss的情况下调用<br>
     * {@link MultiQueryAction#getFromOther()}
     * 从其他数据源获取数据，如果从其他源获取数据则将数据set进memcache，<br>
     * 
     * @param <K> Key的类型
     * @param <V> 值的类型
     * @param query {@link MultiQueryAction}
     * @return 返回查询到的数据列表，没有查到数据的Key不会在List中
     */
    public static <K, V> List<V> getMultiAsList(MultiQueryAction<K, V> query) {
        return Arrays.asList(query.queryCacheAsArray());
    }

    /**
     * 存储一个对象至Memcache中，如果Memcache中已经存在相同Id的数据， <br>
     * 操作完成后memcache中的数据将会被新的至所替代
     * 
     * <p>
     * <Strong>注意：</Strong> XCache中还支持add操作（这里暂不支持），
     * add操作是如果memcache中已经存在了，新的数据将不会替换旧数据
     * 
     * @param <T> 对象类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param clazz Memcache中存的对象的class
     * @param id 要存的对象的id
     * @param value 对象的值
     * @return 存储成功的情况下返回true，否则返回false
     */
    public static <T> boolean set(CacheNameSpace namespace, CachePrefix prefix, Class<T> clazz,
            Object id, T value) {
        XCache<T> xcache = getXCache(namespace, prefix, clazz);
        boolean result = xcache.set("" + id, value, prefix.getExpireSeconds());
        return result;
    }

    /**
     * 删除memcache中的一个对象
     * 
     * @param <T> 对象类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param clazz Memcache中存的对象的class
     * @param id 要存的对象的id
     * @return 删除成功的情况下返回true， 否则返回false
     */
    public static <T> boolean delete(CacheNameSpace namespace, CachePrefix prefix, Class<T> clazz,
            Object id) {
        XCache<T> xcache = getXCache(namespace, prefix, clazz);
        boolean result = xcache.delete(id + "");
        return result;
    }

    /**
     * 将memcache中用来计数的key增加{@code inc}大小
     * 
     * @param <T> 对象类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param id 要存的对象的id
     * @param inc 增加的值
     * @return -1，如果该id在memache中不存在，否则返回增加后memcache中的数据值
     */
    public static long incr(CacheNameSpace namespace, CachePrefix prefix, Object id, long inc) {
        XCache<Long> xcache = getXCache(namespace, prefix, Long.class, true);
        long result = xcache.incr(id + "", inc);
        return result;
    }

    /**
     * 将memcache中用来计数的key减少{@code inc}大小
     * 
     * @param <T> 对象类型
     * @param namespace Cache的名字空间
     * @param prefix key的前缀
     * @param id 要存的对象的id
     * @param inc 减少的值
     * @return -1，如果该id在memache中不存在，否则返回减少后memcache中的数据值
     */
    public static long decr(CacheNameSpace namespace, CachePrefix prefix, Object id, long inc) {
        XCache<Long> xcache = getXCache(namespace, prefix, Long.class, true);
        long result = xcache.decr(id + "", inc);
        return result;
    }

    /**
     * memcache查询操作
     * <p>
     * Override {@link #getFromOther()}可以实现在cache miss的情况下从其他数据源读取数据
     * 
     * @author yi.li@renren-inc.com since 2012-3-22
     * 
     * @param <T> 在Memcache中存的对象的类型
     */
    public static abstract class QueryAction<T> {

        private CacheNameSpace namespace;

        private CachePrefix prefix;

        private Object id;

        private Class<T> clazz;

        public QueryAction(CacheNameSpace namespace, CachePrefix prefix, Class<T> clazz, Object id) {
            this.namespace = namespace;
            this.prefix = prefix;
            this.id = id;
            this.clazz = clazz;
        }

        public final T queryCache() {
            XCache<T> xcache = getXCache(namespace, prefix, clazz);
            T value = xcache.get(id + "");

            if (value == null) {
                value = getFromOther();
                if (null != value) {
                    try {
                        set(namespace, prefix, clazz, id, value);
                    } catch (Exception e) {
                        logger.error("Query action set error. ", e);
                    }
                }
            }

            return value;
        }

        public abstract T getFromOther();
    }

    /**
     * 批量cache查询操作 Override{@link #getFromOther()}在miss的情况下从其他数据源获取
     * 
     * @author yi.li@renren-inc.com since 2012-5-14
     * 
     * @param <K> Key的类型
     * @param <V> Value类型
     */
    public static abstract class MultiQueryAction<K, V> {

        private CacheNameSpace namespace;

        private CachePrefix prefix;

        private List<K> ids;

        private Class<V> clazz;

        public MultiQueryAction(CacheNameSpace namespace, CachePrefix prefix, Class<V> clazz,
                List<K> ids) {
            this.namespace = namespace;
            this.prefix = prefix;
            this.ids = ids;
            this.clazz = clazz;
        }

        public final Map<String, V> queryCache() {
            XCache<V> xcache = getXCache(namespace, prefix, clazz);

            String[] cacheIds = new String[ids.size()];
            int i = 0;
            for (K id : ids) {
                cacheIds[i++] = id.toString();
            }

            Map<String, V> result = xcache.getMulti(cacheIds);
            if (ids.size() == result.size()) {
                return result;
            }

            List<K> missIds = new ArrayList<K>();
            for (K queryId : ids) {
                if (!result.containsKey(queryId.toString())) {
                    missIds.add(queryId);
                }
            }

            if (missIds.size() > 0) {
                for (K missId : missIds) {
                    V value = getFromOther(missId);
                    if (value != null) {
                        result.put(missId.toString(), value);
                        set(namespace, prefix, clazz, missId, value);
                    }
                }
            }

            return result;
        }

        @SuppressWarnings("unchecked")
        public final V[] queryCacheAsArray() {
            Map<String, V> result = queryCache();

            V[] arr_result = (V[]) Array.newInstance(this.clazz, result.size());

            int t = 0;
            for (int i = 0; i < ids.size(); i++) {
                V value = result.get(ids.get(i).toString());
                if (value != null) {
                    arr_result[t++] = value;
                }
            }

            return arr_result;
        }

        public abstract V getFromOther(K id);
    }

}
