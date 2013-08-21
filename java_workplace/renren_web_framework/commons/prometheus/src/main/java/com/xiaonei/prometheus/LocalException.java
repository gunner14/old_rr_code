package com.xiaonei.prometheus;

public class LocalException extends RuntimeException{
	private static final long serialVersionUID = 167336777220962044L;
	public LocalException(){
		super();
	}
	public LocalException(String msg){
		super(msg);
	}
}
