package com.renren.xcs;

import java.util.List;

/**
 * 服务注册点访问器
 * 
 * @author xun.dai@renren-inc.com
 * 
 */
public interface ServiceRegistryAccessor {

    /**
     * 当前访问器可以访问的目标服务ID
     * 
     * @return 服务ID
     */
    public String getService();

    /**
     * 当前访问器可以访问的服务的版本
     * 
     * @return
     */
    public int getVersion();

    /**
     * 当前访问器可以访问的服务的版本节点的内容，并坚挺内容的变化
     * 
     * @param listener 内容监听器
     * @return 版本内容
     */
    public byte[] getVersionAndListenChange(DataChangeListener listener) throws ZookeeperException;

    /**
     * 获取当前版本下的所有状态，并坚挺该状态下所有节点的变化
     * 
     * @param listener 子节点监听器
     * @return 状态列表
     */
    public List<String> listStatesAndListenChange(ChildrenChangeListener listener)  throws ZookeeperException;

    /**
     * 获取某个state节点的内容，并监听该状态内容的变化
     * 
     * @param state 状态
     * @param listener 内容监听器
     * @return 状态内容
     */
    public byte[] getStateAndListenChange(String state, DataChangeListener listener) throws ZookeeperException;

    /**
     * 获取当前Service和Version下某个状态下的所有节点
     * 
     * @param state 状态
     * @param listener 子节点监听器
     * @return 子节点名字列表
     */
    public List<String> listNodesAndListenChange(String state, ChildrenChangeListener listener) throws ZookeeperException;

    /**
     * 获取节点内容，并坚挺内容变化
     * 
     * @param state
     * @param node
     * @param listener
     * @return 节点内容
     */
    public byte[] getNodeAndListenChange(String state, String node, DataChangeListener listener) throws ZookeeperException;

}
