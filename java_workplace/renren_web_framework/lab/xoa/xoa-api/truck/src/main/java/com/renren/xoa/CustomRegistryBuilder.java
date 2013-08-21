package com.renren.xoa;

import com.renren.xoa.registry.XoaRegistry;
import com.renren.xoa.registry.XoaRegistryFactory;
import com.renren.xoa.registry.impl.XoaRegistryLogListener;


/**
 * 
 * 由于可能需要向Registry中添加一些Listener，所以这些逻辑中这个类来封装。
 * 
 * 同时这个类也要保证Registry是单例的。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-8 下午06:26:52
 */
public class CustomRegistryBuilder {

    private static CustomRegistryBuilder instance = new CustomRegistryBuilder();
    
    public static CustomRegistryBuilder getInstance() {
        return instance;
    }
    
    private XoaRegistry customRegistry;
    
    private CustomRegistryBuilder(){
        prepairRegistry();
    }
    
    private void prepairRegistry() {
        customRegistry = XoaRegistryFactory.getInstance().getDefaultRegistry();
        customRegistry.addListener(new XoaRegistryLogListener());
        customRegistry.addListener(new Disconnector(customRegistry));
    }
    
    public XoaRegistry get() {
        return customRegistry;
    }
    
}
