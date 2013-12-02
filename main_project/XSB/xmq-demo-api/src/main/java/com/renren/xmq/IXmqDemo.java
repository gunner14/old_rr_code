package com.renren.xmq;


import java.nio.ByteBuffer;

import com.renren.xoa2.annotation.XoaService;

@XoaService("xmq_demo.xmq.xoa.renren.com")
public interface IXmqDemo extends XmqDemo.Iface {
    
    public void incr(int amount);
    
    public void sendData(ByteBuffer data);
    
    public int get();
}
