package com.renren.xcs;

/**
 * 服务注册接口，提供给Server角色，用于发布一个服务。
 * 
 * @author dai
 * 
 */
public interface ServiceRegistry {

    /**
     * 发布Service
     * 
     * @param serviceId 服务ID
     * @param version 版本
     * @param stat 状态
     * @param node 节点
     * @param isTemp 是否为临时节点
     * @throws XcsException 发布失败将抛出的异常
     */
    public void publishService(String serviceId, int version, String stat, Node node, boolean isTemp)
            throws XcsException;

    /**
     * 更新Node内容
     * 
     * @param serviceId 服务ID
     * @param version 版本
     * @param stat 状态
     * @param node 节点
     * @throws XcsException 更新失败将抛出的异常
     */
    public void updateNode(String serviceId, int version, String stat, Node node)
            throws XcsException;
    
    /**
     * 销毁ServiceRegistry
     */
    public void destroy();
}
