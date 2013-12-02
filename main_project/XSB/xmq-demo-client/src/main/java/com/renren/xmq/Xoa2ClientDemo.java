package com.renren.xmq;

import com.renren.xoa2.client.XmqServiceFactory;

public class Xoa2ClientDemo {

    public static void main( String[] args )
    {
        XmqServiceFactory factory = new XmqServiceFactory();
        IXmqDemo service = factory.getService(IXmqDemo.class, 300);
        service.incr(321);
    }
}
