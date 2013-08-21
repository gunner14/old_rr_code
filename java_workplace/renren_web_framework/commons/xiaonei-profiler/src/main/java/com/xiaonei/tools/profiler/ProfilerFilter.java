package com.xiaonei.tools.profiler;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class ProfilerFilter implements Filter {
	@Override
	public void destroy() {
		
	}

	@Override
	public void init(FilterConfig arg0) throws ServletException {
		
	}

	public void doFilter(ServletRequest servletRequest,
			ServletResponse servletResponse, FilterChain chain)
			throws IOException, ServletException {
		HttpServletRequest request = (HttpServletRequest) servletRequest;
		HttpServletResponse response = (HttpServletResponse) servletResponse;
		String domain = request.getHeader("HOST");
		
		ProfilerManager profilerManager = ProfilerManager.getInstanceByDomain(domain);
		if(!profilerManager.isEnable()){
			chain.doFilter(servletRequest, servletResponse);
			return;
		}
		
		String name = request.getServletPath();
		ProfilerPoint profiler = profilerManager.getProfilerPoint(name,domain);
		if(profiler==null){
			chain.doFilter(request, response);
			return;
		}
		
		profiler.start();
		try {
			chain.doFilter(request, response);
		} finally {
			profiler.finish();
		}
	}
}