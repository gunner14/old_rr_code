package com.xiaonei.platform.core.opt.permission;
/**
 * 权限检查有问题返回的错误信息
 * 
 * @author yunlong.bai@opi-corp.com
 *
 */
public class Message {
	private String key ;
	private String message ;
	private String redirectUrl ;
	private boolean isRedirect=false ;
	/**
	 * 
	 * @param key
	 * @param message
	 */
	public Message(String key,String message){
		this.key = key ;
		this.message = message ;
	}
	public String getKey() {
		return key;
	}
	public void setKey(String key) {
		this.key = key;
	}
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
	public String toString(){
		return "{key:"+this.key+",message:"+message+",isRedirect:"+this.isRedirect+",redirectUrl:"+this.redirectUrl+"}" ;
	}
	public boolean isRedirect() {
		return isRedirect;
	}
	public void setRedirect(boolean isRedirect) {
		this.isRedirect = isRedirect;
	}
	public String getRedirectUrl() {
		return redirectUrl;
	}
	public void setRedirectUrl(String redirect) {
		this.redirectUrl = redirect;
	}

}
