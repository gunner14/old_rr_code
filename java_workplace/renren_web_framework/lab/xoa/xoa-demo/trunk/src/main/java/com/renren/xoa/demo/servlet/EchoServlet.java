package com.renren.xoa.demo.servlet;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.URLEncoder;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class EchoServlet extends HttpServlet {
	
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		
		//request.setCharacterEncoding("UTF-8");
		
		System.out.println("request charset=" + request.getCharacterEncoding());
		
		System.out.println("queryString=" + request.getQueryString());
		String title = request.getParameter("title");
		System.out.println("title=" + title);
		response.setCharacterEncoding("UTF-8");
		response.setContentType("text/plain");
		byte[] bytes = title.getBytes();
		
		System.out.println("bytes.length=" + bytes.length);
		
		ServletOutputStream os = response.getOutputStream();
		os.write(bytes);
		os.flush();
		/*PrintWriter out = response.getWriter();
		System.out.println("response charset=" + response.getCharacterEncoding());
		out.print(title);
		out.flush();*/
	}
	
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doGet(request, response);
	}

	public static void main(String[] args) throws Exception{
		System.out.println(URLEncoder.encode("首页", "UTF-8"));
	}
	
}
