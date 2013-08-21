package com.renren.xoa2.backdoor.test;

import com.renren.xoa2.annotation.XoaService;
import com.renren.xoa2.backdoor.test.TestService;

@XoaService("test.backdoor.xoa.renren.com")
public interface ITestService extends TestService.Iface{

    public void ping(int timeout);
}
