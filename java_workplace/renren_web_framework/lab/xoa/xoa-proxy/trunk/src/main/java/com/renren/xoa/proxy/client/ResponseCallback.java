package com.renren.xoa.proxy.client;

import com.renren.xoa.commons.spdy.frame.Frame;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-6-4 下午04:17:28
 */
public interface ResponseCallback {

    public void frameReceived(Frame frame);
}
