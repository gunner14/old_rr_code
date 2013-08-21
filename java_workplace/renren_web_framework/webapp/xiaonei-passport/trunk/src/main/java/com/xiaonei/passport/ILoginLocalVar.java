package com.xiaonei.passport;

public interface ILoginLocalVar {
	
	public void setAttribute(String key, Object value);
	
	public Object getAttribute(String key);
	
	public void removeAttribute(String key);
}
