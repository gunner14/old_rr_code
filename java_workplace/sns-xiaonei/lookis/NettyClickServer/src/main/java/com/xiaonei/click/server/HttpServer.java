package com.xiaonei.click.server;

import java.net.InetSocketAddress;

import com.xiaonei.click.admin.ReloadAble;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public interface HttpServer extends ReloadAble {

    public void bind(InetSocketAddress add);
}
