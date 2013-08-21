package com.renren.xoa.demo.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.annotate.JsonSerialize;

import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.commons.test.BlogFactory;

public class BenchmarkServlet extends HttpServlet {

	public BenchmarkServlet() {
		initObjectMapper();
	}
	
	private ObjectMapper objectMapper;
	
	private static final long serialVersionUID = 1L;
	
	private BlogFactory blogFactory = BlogFactory.getInstance();
	
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		
		int k = 0;
		String uri = request.getRequestURI();
		String[] ss = uri.split("/");
		k = Integer.parseInt(ss[2]);
		Blog[] blogs = blogFactory.getDataBySize(k);
		/*String accept = request.getHeader("Accept");
		if (accept != null && accept.contains("application/json")) {
			response.setContentType("application/json");
		} else {	//浏览器调试的时候返回application/json是不能正常显示的
			response.setContentType("text/plain");
		}*/
		response.setCharacterEncoding("UTF-8");
		
		objectMapper.writeValue(response.getWriter(), blogs);
	}
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doGet(request, response);
	}

	private void initObjectMapper() {
		objectMapper = new ObjectMapper();
		//设置ObjectMapper只序列化非默认值的属性，这样可以节省流量
		objectMapper.getSerializationConfig().setSerializationInclusion(
				JsonSerialize.Inclusion.NON_DEFAULT);
	}
	
}
