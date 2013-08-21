package org.jboss.netty.example.http.snoop.lwb;

import java.util.List;
import java.util.Set;

import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.handler.codec.http.HttpMethod;
import org.jboss.netty.handler.codec.http.HttpRequest;
import org.jboss.netty.handler.codec.http.HttpVersion;

public class ApiTestHanlder implements IReqHandler{

	public static final String CRLF = "\r\n";
	
	@Override
	public String handle(HttpRequest request) {

		StringBuilder responseContent = new StringBuilder(1024);
		String uri = request.getUri();
		HttpMethod method = request.getMethod();
		HttpVersion version = request.getProtocolVersion();
		
		responseContent.append(method);
		responseContent.append(' ');
		responseContent.append(uri);
		responseContent.append(' ');
		responseContent.append(version.toString());
		responseContent.append(CRLF);
		
		Set<String> headerNames = request.getHeaderNames();
		for (String headerName : headerNames) {
			List<String> headers = request.getHeaders(headerName);
			for (String headerValue : headers) {
				responseContent.append(headerName);
				responseContent.append(": ");
				responseContent.append(headerValue);
				responseContent.append(CRLF);
			}
		}

		ChannelBuffer content = request.getContent();
		if (content.readable()) {
			responseContent.append(CRLF);
			responseContent.append(content.toString("UTF-8"));
			responseContent.append(CRLF);
		}
		return responseContent.toString();
	}
}
