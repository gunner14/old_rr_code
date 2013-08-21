package com.xiaonei.page.xoa.task;

public interface CallBack {
	public void exceptionCall(Exception e,String attachMsg);
	public void successCall();
	public void failedCall();
}
