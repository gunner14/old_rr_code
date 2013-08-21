package org.jboss.netty.example.http.snoop.lwb;

import org.jboss.netty.handler.codec.http.HttpRequest;

/**
 * 
 * 实在想不出好名字了，先叫这个吧
 * @author Li Weibo
 * @since 2009-11-10
 */
public interface IReqHandler {

	public String handle(HttpRequest request);
	
}
