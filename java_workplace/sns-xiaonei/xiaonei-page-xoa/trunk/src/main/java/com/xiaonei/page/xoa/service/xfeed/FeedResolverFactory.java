package com.xiaonei.page.xoa.service.xfeed;

import java.util.HashMap;
import java.util.Map;

import org.apache.log4j.Logger;
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Service;

import com.xiaonei.page.xoa.model.feed.PageFeed;

/**
 * @author yi.li@renren-inc.com since 2011-9-30
 * 
 */
@SuppressWarnings({"unchecked","rawtypes"})
@Service
public class FeedResolverFactory implements ApplicationContextAware {

    private static final Map<Class<?>, FeedResolver> resolverMap = new HashMap<Class<?>, FeedResolver>();
    private static final Map<String, OldFeedResolver> oldResolverMap = new HashMap<String, OldFeedResolver>();
    
    private final static Logger logger = Logger.getLogger("PAGEXOA");

    /**
     * 获取对应model的解析器
     * 
     * @param <T>
     * @param clazz
     * @return 找到解析器返回{@link FeedResolver}，反之返回null
     */
    public <T extends PageFeed> FeedResolver<T> getFeedResolver(Class<T> clazz) {
        FeedResolver<T> resolver = null;
        try{
            resolver = (FeedResolver<T>) resolverMap.get(clazz);
        }catch(Exception e){
            logger.error("get feed resovler error, exception:" + e);
            return null;
        }
        
        if (null != resolver) {
            return resolver;
        }
        return null;
    }
    
    /**
     * @param feedType  新鲜事类型
     * @return
     */
    public OldFeedResolver getOldFeedResolver(long feedType){
        OldFeedResolver resolver = null;
        try{
            resolver = oldResolverMap.get(feedType + "");
        }catch(Exception e){
            logger.error("get feed resovler error, exception:" + e);
            return null;
        }
        
        if(null != resolver){
            return resolver;
        }
        return null;
    }

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        wrapNewResovler(applicationContext);
        wrapOldResolver(applicationContext);
    }
    
    private void wrapNewResovler(ApplicationContext applicationContext){
        try{
            Map<String, FeedResolver> beanMaps = applicationContext.getBeansOfType(FeedResolver.class);
            if (null == beanMaps) {
                return;
            }
            for (FeedResolver item : beanMaps.values()) {
                resolverMap.put(item.getSupportClass(), item);
            }
            logger.info("Resovler Maps: " + resolverMap);
        }catch(Exception e){
            logger.error("wrap new feed resolver error", e);
        }
    }
    
    private void wrapOldResolver(ApplicationContext applicationContext){
        try{
            Map<String, OldFeedResolver> beanMaps = applicationContext.getBeansOfType(OldFeedResolver.class);
            if (null == beanMaps) {
                return;
            }
            for (OldFeedResolver item : beanMaps.values()) {
                oldResolverMap.put(item.getSupportedFeedType() + "", item);
            }
            logger.info("Old Resovler Maps: " + oldResolverMap);
        }catch(Exception e){
            logger.error("wrap old feed resolver error", e);
        }
    }
}
