package com.renren.xcs;

import java.util.List;

/**
 * 节点的子节点发生变化事件的监听器
 * 
 * @author Xun Dai <xun.dai@renren-inc.com>
 *
 */
public interface ChildrenChangeListener {
	
	/**
	 * 子节点发生变化
	 * @param children 发生变化后的所有节点
	 */
	public void childrenChanged(List<String> children);
}
