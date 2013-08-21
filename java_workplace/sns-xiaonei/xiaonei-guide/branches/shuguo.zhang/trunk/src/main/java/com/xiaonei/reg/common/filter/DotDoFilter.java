/**
 * 
 */
package com.xiaonei.reg.common.filter;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.common.constants.Globals;

/**
 * @author wangtai
 * 
 */
public class DotDoFilter implements Filter {

	@Override
	public void destroy() {

	}

	@Override
	public void doFilter(ServletRequest servletrequest,
			ServletResponse servletresponse, FilterChain filterchain)
			throws IOException, ServletException {
		if (!(servletrequest instanceof HttpServletRequest)) {
			filterchain.doFilter(servletrequest, servletresponse);
			return;
		}
		if (!(servletresponse instanceof HttpServletResponse)) {
			filterchain.doFilter(servletrequest, servletresponse);
			return;
		}
		HttpServletRequest request = (HttpServletRequest) servletrequest;
		HttpServletResponse response = (HttpServletResponse) servletresponse;

		String serverName = request.getServerName();
		String uri = request.getRequestURI();
		if (!StringUtils.endsWith(uri, ".do")) {
			filterchain.doFilter(servletrequest, servletresponse);
			return;
		}
		String queryString = request.getQueryString();
		String url = "http://" + serverName + uri;
		if (StringUtils.isNotEmpty(queryString)) {
			url += "?" + queryString;
		}
		if (Globals.DEBUG_ENV) {
			System.out.print("serverName:" + serverName + " uri:" + uri
					+ " queryString:" + queryString);
			System.out.println("url:" + url);
		}
		try {
			request.getRequestDispatcher(url).forward(request, response);
			return;
		} catch (IOException e) {
			e.printStackTrace();
		}
		filterchain.doFilter(servletrequest, servletresponse);
	}

	@Override
	public void init(FilterConfig filterconfig) throws ServletException {

	}

}
