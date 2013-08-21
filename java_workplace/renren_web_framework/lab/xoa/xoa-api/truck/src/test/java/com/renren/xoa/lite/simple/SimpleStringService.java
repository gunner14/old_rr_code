package com.renren.xoa.lite.simple;

import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaService;


@XoaService(serviceId = "test.xoa.renren.com")
public interface SimpleStringService {

	@XoaGet("/ok")
	public ServiceFuture<String> getOk();
	
	public void haha();
	
}
