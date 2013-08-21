package org.jboss.netty.example.http.snoop.lwb;

import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.handler.codec.http.HttpRequest;

import util.Counter;

public class EchoTestHanlder implements IReqHandler{
	
	private Counter counter = new Counter(this.getClass().getSimpleName());
	
	@Override
	public String handle(HttpRequest request) {
		StringBuilder responseContent = new StringBuilder(1024);
		ChannelBuffer content = request.getContent();
		if (content.readable()) {
			responseContent.append(content.toString("UTF-8"));
		}
		counter.incr();
		return responseContent.toString();
	}
}
