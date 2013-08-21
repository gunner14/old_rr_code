package com.renren.xoa.lite.type;

import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaService;


@XoaService(serviceId = "blog.xoa.renren.com")
public interface TypeService {

	@XoaGet("/ok")
	public ServiceFuture<String[]> getStringArray();
	
	
}
