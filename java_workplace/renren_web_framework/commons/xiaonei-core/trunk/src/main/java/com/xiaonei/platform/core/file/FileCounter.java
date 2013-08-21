package com.xiaonei.platform.core.file;

public class FileCounter {
	private int counter=1 ;
	
	public synchronized int getCounter(){
		counter ++ ;
		return counter ;
	}

}
