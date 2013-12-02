package com.renren.xoa2.quota.processor;


/**
 * 超过配额处理器(不同的处理策略对应不同的实现)
 * @author Xue Wenxin
 *
 */
public interface ExceedProcessor {
	
	/***
	 * 处理超过配额到情况
	 */
	public void process();

}
