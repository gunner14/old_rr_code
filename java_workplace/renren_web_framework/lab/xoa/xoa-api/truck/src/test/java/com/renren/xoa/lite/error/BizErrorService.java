package com.renren.xoa.lite.error;

import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaService;

@XoaService(serviceId = "blog.xoa.renren.com")
public interface BizErrorService {

	@XoaGet("/error")
	public ServiceFuture<String> get();
	
}
