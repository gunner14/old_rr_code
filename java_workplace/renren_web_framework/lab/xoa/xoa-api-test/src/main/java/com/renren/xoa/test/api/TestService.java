package com.renren.xoa.test.api;

import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaParam;
import com.renren.xoa.lite.annotation.XoaService;


@XoaService(serviceId = "blog.xoa.renren.com")
public interface TestService {

    @XoaGet("/text/{size}/{timeout}")
    public ServiceFuture<String> getText(@XoaParam("size") int size, @XoaParam("timeout") int timeout);
    
}
