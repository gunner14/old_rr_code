package com.xiaonei.platform.core.opt.permission.strategy;

import com.xiaonei.platform.core.opt.permission.Message;
import com.xiaonei.platform.core.opt.permission.PermissionFactory;
/**
 * 权限检查结果对象
 * @author yunlong.bai@opi-corp.com
 *
 */
public class Ret {
	private boolean comeon ;
	private boolean strategy ;
	private String className ;
	/**
	 * 权限检查结果对象
	 * @param comeon  是否继续权限检查
	 * @param strategy  是否拥有权限
	 * @param className  被检查出问题的权限的id
	 */
	public Ret(boolean comeon,boolean strategy,String className){
		this.comeon = comeon ;
		this.strategy = strategy ;
		this.className = className ;
	}
	public boolean hasRight(){
		return this.strategy ;
	}
	public boolean isComeon() {
		return comeon;
	}
	public void setComeon(boolean comeon) {
		this.comeon = comeon;
	}
	public boolean isStrategy() {
		return strategy;
	}
	public void setStrategy(boolean strategy) {
		this.strategy = strategy;
	}
	public String toString(){
		return "{common:"+comeon+",strategy:"+strategy+",className:"+this.className+",message:"+this.getMsg()+"}" ;
	}
	public Message getMsg() {
		Message msg=PermissionFactory.getInstance().getMessageByKey(this.className) ; 
		if(msg == null){
			try{
				throw new Exception() ;
			}catch(Exception e){}
		}
		return msg ;
	}

}
