package com.renren.xoa.demo.controllers;

import java.util.Enumeration;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

public class EchoController {

	
	public EchoController() {

	}

	@Get
	@Put
	@Post
	@Delete
	public Object index(Invocation inv) {
		StringBuilder sb = new StringBuilder();
		sb.append("@");
		
		HttpServletRequest request = inv.getRequest();

		String method = request.getMethod();
		sb.append("method=" + method + "\r\n");
		
		Enumeration<String> headerNames = request.getHeaderNames();
		while (headerNames.hasMoreElements()) {
			String headerName = headerNames.nextElement();
			Enumeration<String> values = request.getHeaders(headerName);
			while (values.hasMoreElements()) {
				String value = values.nextElement();
				sb.append(headerName);
				sb.append(": ");
				sb.append(value);
				sb.append("\r\n");
			}
		}
		
		sb.append("\r\n");
		Enumeration<String> paramNames = request.getParameterNames();
		while (paramNames.hasMoreElements()) {
			String paramName = paramNames.nextElement();
			String[] values = request.getParameterValues(paramName);
			for (String value : values) {
				sb.append(paramName);
				sb.append("=");
				sb.append(value);
				sb.append("&");
			}
		}
		if (sb.length() > 0 && sb.charAt(sb.length() - 1) == '&') {
			sb.deleteCharAt(sb.length() - 1);
		}
		System.out.println(sb.toString());
		return sb.toString();
	}
	
}
