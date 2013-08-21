package com.xiaonei.tools.profiler;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class ProfilerServlet extends HttpServlet{
	private static final long serialVersionUID = 1L;
	private static String version = "1.0";
	
	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		handlerRequest(req,resp);
	}

	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		handlerRequest(req,resp);
	}
	
	private void handlerRequest(HttpServletRequest request,HttpServletResponse response){
		String domain = request.getHeader("HOST");
		ProfilerManager profilerManager = ProfilerManager.getInstanceByDomain(domain);
		PrintWriter out = null;
		try {
			out = response.getWriter();
		} catch (IOException e) {
			e.printStackTrace();
			return;
		}
		
		StringBuffer sb = new StringBuffer();
		sb.append(
	      "<!DOCTYPE html  PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
		
		sb.append("<html><head><title>xiaonei-profiler-version ");
		sb.append(version);
		sb.append("</title><style>\n");
		sb.append(
	      "h1 { background: #ccddff; margin : 0 -0.5em 0.25em -0.25em; padding: 0.25em 0.25em; }\n");
		sb.append(
	      "h2 { background: #ccddff; padding: 0.25em 0.5em; margin : 0.5em -0.5em; }\n");
		sb.append("table { border-collapse : collapse; }\n");
		sb.append("th { background : #c78ae6; border-left : 1px; border-right : 1px}\n");
		sb.append("tr { border-bottom : 1px dotted; }\n");
		sb.append(".number { text-align : right; }\n");
		sb.append("table table tr { border-bottom : none; }\n");
		sb.append("</style><head><body><table border=0><tr><th>Name</th><th>Average Time</th><th>Min Time</th><th>Max Time</th><th>Invocation Count</th></tr>");
		
		
		java.util.Iterator<ProfilerPoint> enums = profilerManager.profilerIterator();
		while(enums.hasNext()){
			ProfilerPoint point = enums.next();
			if(point.getInvocationCount()==0)continue;
			sb.append("<tr class='level0'><td>"+point.getName()+"</td><td class=number>"+point.getAverageTime()+"</td><td class=number>"+point.getMinTime()+"</td><td class=number>"+point.getMaxTime()+"</td><td class=number>"+point.getInvocationCount()+"</td></tr>");
		}
		
		sb.append("</table></body></html>");
		out.println(sb);
	}
	
}