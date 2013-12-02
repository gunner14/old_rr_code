package com.renren.xcs;

/**
 * 节点内容发生变化事件的监听器
 * 
 * @author Xun Dai <xun.dai@renren-inc.com>
 *
 */
public interface DataChangeListener {
	/**
	 * 节点内容发生变化
	 * @param data 变化后的节点内容
	 */
	public void dataChanged(byte[] data);
}
