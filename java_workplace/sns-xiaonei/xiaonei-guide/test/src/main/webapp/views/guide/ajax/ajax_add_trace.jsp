
<%@page import="com.xiaonei.platform.core.utility.StringFormat"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="org.apache.log4j.Logger"%>
<%
	User host = null;
	Integer userId = 0;
	try {
		host = (User) request.getAttribute("host");
		if (host != null) {
			userId = host.getId();
		}
		request.setAttribute("userId", userId);
	} catch (Exception e) {
		e.printStackTrace();
	}
	if (host != null) {
		try {
			Logger logger = Logger.getLogger("xnlog.guide");			
			//String p = "h:" + request.getAttribute("userId") + " p:" + request.getParameter("p")+ " a:" + request.getParameter("a") + " f:" + request.getParameter("f");
			String pageid = getNotNullStr(request.getParameter("p"));
			String actid = getNotNullStr(request.getParameter("a"));
			String fujia = getNotNullStr(request.getParameter("f"));
			if((pageid.equals("p21") || pageid.equals("p22"))
				&& actid.equals("ga1")  
				&& fujia.indexOf('_') > 0
				&& fujia.indexOf(':') < 0
			){
				String [] fs = fujia.split("_");
				String fs10 = "0";
				String fs11 = "0";
				if(fs!=null && fs.length == 2){
					fs10 = fs[0];
					fs11 = fs[1];
				}
				else if(fs.length == 1){
					fs10 = fs[0];
				}
				else{
					
				}
				int nfs10 = 0;
				int nfs11 = 0;
				nfs10 = Integer.parseInt(fs10);
				nfs11 = Integer.parseInt(fs11);
				String log = userId + "," + pageid + "," + "ga10" + "," + nfs10;
				logger.info(log);
				log = userId + "," + pageid + "," + "ga11" + "," + nfs11;
				logger.info(log);
				log = userId + "," + pageid + "," + "ga1" + "," + (nfs10+nfs11);
				logger.info(log);
			}
			else{
				String log = "" + request.getAttribute("userId") + "," + request.getParameter("p")+ "," + request.getParameter("a") + "," + request.getParameter("f");
				logger.error(log);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
%>

<%!String getNotNullStr(String a){
	String ret = "";
	if(a == null) {
		
	}
	else{
		ret = a.trim();	
	}
	return ret;
}
%>