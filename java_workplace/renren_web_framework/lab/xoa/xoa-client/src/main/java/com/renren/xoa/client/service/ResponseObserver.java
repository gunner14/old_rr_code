package com.renren.xoa.client.service;

import com.renren.xoa.commons.netty.SpdyHttpResponse;

/**
 * @author Lookis (lookisliu@gmail.com)
 * 
 */
public interface ResponseObserver {

    public void messageReceived(SpdyHttpResponse response);
}
