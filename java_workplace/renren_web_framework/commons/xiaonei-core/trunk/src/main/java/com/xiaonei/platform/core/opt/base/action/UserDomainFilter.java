package com.xiaonei.platform.core.opt.base.action;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.oro.text.regex.MalformedPatternException;
import org.apache.oro.text.regex.Pattern;
import org.apache.oro.text.regex.PatternCompiler;
import org.apache.oro.text.regex.PatternMatcher;
import org.apache.oro.text.regex.Perl5Compiler;
import org.apache.oro.text.regex.Perl5Matcher;

public class UserDomainFilter implements Filter {

	public void destroy() {
		// TODO Auto-generated method stub
		
	}

	public void doFilter(ServletRequest arg0, ServletResponse arg1, FilterChain arg2) throws IOException, ServletException {
		// TODO Auto-generated method stub
		//System.out.println("---------------userdomain------------") ;
		HttpServletRequest request= (HttpServletRequest)arg0 ;
		HttpServletResponse response= (HttpServletResponse)arg1 ;
		String serverName = request.getServerName();
		if (serverName != null && serverName.contains("5q.com")) {
			String uri = request.getRequestURI();
			String queryString = request.getQueryString();
			String temp = uri;
			if(queryString!=null&&queryString.trim().length()>0){
				temp+="?" + queryString;
			}
			//增加对 http://5q.com/fusongli 这种链接的屏蔽
			if(check(temp)){
				//System.out.println("filter:------tmp"+temp) ;
				return ;
			}
			serverName = serverName.replaceAll("5q", "xiaonei");
			String url = "http://" + serverName + uri ;
			//System.out.println("filter:"+url) ;
			if(queryString!=null&&queryString.trim().length()>0){
				url+="?" + queryString;
			}
			try {
				response.sendRedirect(url);
				return ;
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		arg2.doFilter(arg0, arg1) ;
	}
	private boolean check(String logEntry) {
		///url.do?code=fusongli
		String regexp="";
		regexp = "^/url.do\\?code=(\\w+)$";
//		regexp = "^/(\\w+)/([pf])(\\w+)$";
		try {
			PatternCompiler compiler =  new Perl5Compiler();
			Pattern pattern  = compiler.compile(regexp);
			PatternMatcher matcher = new Perl5Matcher();
			if(matcher.contains(logEntry,pattern)){
//				MatchResult result = matcher.getMatch();
//				System.out.println("----匹配http://5q.com/fusongli,a:"+result.group(1));
//				System.out.println("b:"+result.group(2));
				return true;
			}else{
				return false;
			}
		} catch (MalformedPatternException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}

	public void init(FilterConfig arg0) throws ServletException {
		// TODO Auto-generated method stub
		
	}

}
