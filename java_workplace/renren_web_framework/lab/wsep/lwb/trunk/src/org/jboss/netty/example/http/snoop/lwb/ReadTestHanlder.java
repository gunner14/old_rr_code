package org.jboss.netty.example.http.snoop.lwb;

import org.jboss.netty.handler.codec.http.HttpRequest;

public class ReadTestHanlder implements IReqHandler{

	@Override
	public String handle(HttpRequest request) {
		return "";
	}

}
