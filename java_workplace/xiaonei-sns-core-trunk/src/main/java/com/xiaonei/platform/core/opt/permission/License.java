package com.xiaonei.platform.core.opt.permission;



/**
 * 权限结果对象<br>
 * 封装了权限判断后的二者关系值、消息等<br>
 * 用法可以参考<code>com.xiaonei.message.action.SendMsgAction</code>中相关代码
 * @author fusong.li@opi-corp.com
 * 2008-3-4 下午04:42:25
 */
public class License {
	
	/**甲和乙之间的关系值*/
	private int path;
	
	/**是否有权限*/
	private boolean right;
	
	/**无权限的理由Message对象*/
	private Message promptMessage;

	/**
	 * 如果有权限，建议使用此构造函数
	 * @param right
	 * @param path
	 */
	public License(boolean right , int path){
		this.right = right;
		this.path = path;
	}
	/**
	 * 如果无权限，建议使用此构造函数
	 * @param right
	 * @param path
	 * @param promptMessage
	 */
	public License(boolean right , int path, Message promptMessage){
		this.right = right;
		this.path = path;
		this.promptMessage = promptMessage;
	}
	/**
	 * 如果无权限，返回无权限的理由
	 * @return
	 */
	public Message getPromptMessage(){
		return promptMessage;
	}
	/**
	 * 是否有权限
	 * @return true 有权限；false 无权限
	 */
	public boolean hasRight(){
		return this.right ;
	}
	/**
	 * 得到甲和乙之间的关系值path
	 * @return
	 */
	public int getPath(){
		return this.path;
	}
}