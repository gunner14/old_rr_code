package com.renren.xoa.lite;

import java.io.IOException;

import com.renren.xoa.lite.impl.listener.ServiceFailureListener;

public class MyL extends ServiceFailureListener<String, IOException>{

	@Override
	public String onError(ServiceFuture future) throws IOException {
		// TODO Auto-generated method stub
		return null;
	}

	public static void main(String[] args) {
		
		MyL l = new MyL();
		
		//l.getReturn();
		
	}
	
}
