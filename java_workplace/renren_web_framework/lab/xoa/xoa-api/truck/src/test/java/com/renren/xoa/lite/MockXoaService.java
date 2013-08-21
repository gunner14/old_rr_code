package com.renren.xoa.lite;

import java.util.List;

import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaParam;
import com.renren.xoa.lite.annotation.XoaService;

@XoaService(serviceId = "blog.xoa.renren.com", hosts={"10.22.200.140:8188"})
public interface MockXoaService {

	@XoaGet("/benchmark/{size}/{sleep}")
	public ServiceFuture<String> getPhoto(@XoaParam("size")int size, @XoaParam("sleep")int sleep);
	
	
	@XoaGet("/benchmark/{size}/{sleep}")
	public ServiceFuture<List<String>> getPhotos(@XoaParam("size")int size, @XoaParam("sleep")int sleep);
}
