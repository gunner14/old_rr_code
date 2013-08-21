package com.xiaonei.platform.component.space.home;

import com.xiaonei.platform.core.opt.permission.Message;


/**
 * @deprecated
 * space的权限返回值对象
 */
public class SpacePermission {
	
	/**甲和乙之间的关系值*/
	private int path;
	
	/**是否有权限浏览*/
	private boolean right;
	
	/**无权限看的理由Message对象*/
	private Message promptMessage;
	
	/**
	 * 如果有权限看，建议使用此构造函数
	 * @param right
	 * @param path
	 */
	public SpacePermission(boolean right , int path){
		this.right = right;
		this.path = path;
	}
	/**
	 * 如果无权限看，建议使用此构造函数
	 * @param right
	 * @param path
	 * @param promptMessage
	 */
	public SpacePermission(boolean right , int path, Message promptMessage){
		this.right = right;
		this.path = path;
		this.promptMessage = promptMessage;
	}
	/**
	 * 如果无权限看，返回无权限看的理由Message对象
	 * @return
	 */
	public Message getPromptMessage(){
		return promptMessage;
	}
	/**
	 * 是否有权限看
	 * @return true 有权限看；false 无权限看主页
	 */
	public boolean hasRight(){
		return this.right ;
	}
	/**
	 * 得到甲和乙之间的关系值
	 * @return
	 */
	public int getPath(){
		return this.path;
	}
}