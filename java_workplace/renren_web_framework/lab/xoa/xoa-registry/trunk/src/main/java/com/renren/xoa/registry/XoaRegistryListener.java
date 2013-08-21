package com.renren.xoa.registry;


/**
 * Registry中的配置信息发生变更的时候会触发{@link XoaRegistryListener}
 * 中的相关事件处理方法
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-4-26 上午11:01:43
 */
public interface XoaRegistryListener {
    
    /**
     * 有节点被置为disabled状态的时候触发
     * @param node
     */
    public void onNodeDisabled(XoaServiceDescriptor node);
    
    /**
     * 有节点被删除的时候触发
     * @param node
     */
    public void onNodeDeleted(XoaServiceDescriptor node);
}
