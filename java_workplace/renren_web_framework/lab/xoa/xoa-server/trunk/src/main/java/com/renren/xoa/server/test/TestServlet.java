package com.renren.xoa.server.test;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Enumeration;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * 测试XOA的servlet
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-8 下午03:48:53
 */
public class TestServlet extends HttpServlet  {

	private static final String CRLF = "\r\n";
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	@Override
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doService(request, response);
	}
	
	@Override
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doService(request, response);
	}
	
	@Override
	public void doPut(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doService(request, response);
	}
	
	@Override
	public void doDelete(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doService(request, response);
	}
	
	private void doService(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		
		request.setCharacterEncoding("UTF-8");
		
		String method = request.getMethod();
		String protocol = request.getProtocol();
		String uri = request.getRequestURI();
		String queryString = request.getQueryString();
		if (queryString != null) {
			uri += "?" + queryString;
		}
		
		response.setContentType("text/plain; charset=UTF-8");
		PrintWriter out = response.getWriter();
		
		StringBuilder sb = new StringBuilder();
		sb.append("{");
		
		sb.append("'method':'");
		sb.append(method);
		sb.append("',");
		
		sb.append("'uri':'");
		sb.append(uri);
		sb.append("',");
		
		sb.append("'protocol':'");
		sb.append(protocol);
		sb.append("',");
		
		sb.append("'headers':{");
		Enumeration<String> headerNames = request.getHeaderNames();
		while (headerNames.hasMoreElements()) {
			String headerName = headerNames.nextElement();
			Enumeration<String> headers = request.getHeaders(headerName);
			while (headers.hasMoreElements()) {
				String headerValue = headers.nextElement();
				sb.append("'");
				sb.append(headerName);
				sb.append("':");
				sb.append("'");
				sb.append(headerValue);
				sb.append("',");
			}
		}
		if (sb.charAt(sb.length() - 1) == ',') {
			sb.deleteCharAt(sb.length() - 1);
		}
		sb.append("},");
		
		sb.append("'body-params':{");
		Map<String, String[]> paramMap = request.getParameterMap();
		for (Entry<String, String[]> entry : paramMap.entrySet()) {
			for (String value : entry.getValue()) {
				sb.append("'");
				sb.append(entry.getKey());
				sb.append("':");
				sb.append("'");
				sb.append(value);
				sb.append("',");
				//不编码，不考虑末尾的&了
			}
		}
		if (sb.charAt(sb.length() - 1) == ',') {
			sb.deleteCharAt(sb.length() - 1);
		}
		sb.append("}");
		
		sb.append("}");
		
		//System.out.println(sb.toString());
		out.append(sb.toString());
		out.flush();
		//System.out.println(sb.toString());
		//System.out.println(request.getParameter("1"));
		/*out.append(method);
		out.append(' ');
		out.append(uri);
		out.append(' ');
		out.append(protocol);
		out.append(CRLF);
				
		Enumeration<String> headerNames = request.getHeaderNames();
		while (headerNames.hasMoreElements()) {
			String headerName = headerNames.nextElement();
			Enumeration<String> headers = request.getHeaders(headerName);
			while (headers.hasMoreElements()) {
				String headerValue = headers.nextElement();
				out.append(headerName);
				out.append(": ");
				out.append(headerValue);
				out.append(CRLF);
			}
		}
		
		out.append(CRLF);
		
		Map<String, String[]> paramMap = request.getParameterMap();
		for (Entry<String, String[]> entry : paramMap.entrySet()) {
			for (String value : entry.getValue()) {
				out.append(entry.getKey());
				out.append("=");
				out.append(value);
				out.append("&");
				//不编码，不考虑末尾的&了
			}
		}
		out.flush();*/
		
	}
	
}
