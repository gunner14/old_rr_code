package com.xiaonei.reg.register.controllers.base;

public class RegForward {
	private String url;
	private boolean isRedirect;
	private String reTo="";
	public RegForward(){
		
	}
	public RegForward(String url){
		this.url = url;
	}
	public RegForward(String url,boolean isRedirect){
		this.url = url;
		this.isRedirect = isRedirect;
	}
	
	public RegForward setReTo(String reTo){
		this.reTo = reTo;
		return this;
	}
	public String go(){
		if(isRedirect){
			return "r:"+reTo+this.url; 
		}else{
			return "f:"+reTo+this.url; 
		}
	}
	public void setRedirect(boolean isRedirect) {
		this.isRedirect = isRedirect;
	}
	public String getPath() {
		return this.url;
	}
}
