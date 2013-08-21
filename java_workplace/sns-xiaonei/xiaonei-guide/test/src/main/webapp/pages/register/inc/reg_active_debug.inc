			<%@page import="com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil"%>
			<%@page import="com.xiaonei.reg.common.constants.Globals"%>
			<%
			if(Globals.DEBUG_ENV){
				try{
					String link = "";
					String ip = "";
					ip = BizFilterUtil.getClientIP(request);
					if("10.2.17.188".equals(ip) 
						||"10.2.17.189".equals(ip)
						||"10.2.17.191".equals(ip)
						||"10.2.17.192".equals(ip)
						||"10.2.19.235".equals(ip)
						||"10.2.17.35".equals(ip) //heting.peng
						//||"10.2.17.70".equals(ip) //huihua.lin
						//||"10.2.19.152".equals(ip) //wenbin.lu
						||"123.125.40.250".equals(ip)
					)
					{
						link = RegEmailLogic.getInstance().getLinkforPub(request.getAttribute("email").toString());
					 	out.println(""+"<ul><a style=\"color:green\" href=\""+link+"\" >debug_active_common</a></ul>");
					 	link = RegEmailLogic.getInstance().getLinkforPage(request.getAttribute("email").toString(),request.getParameter("pageId"));
					 	out.println(""+"<ul><a style=\"color:red\" href=\""+link+"\" >debug_active_page</a></ul>");
					}
				}
				
				catch(Exception e ){
				}
			}
			
			%>