
<%@page import="java.lang.reflect.Method"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%><%
//SnsAdapterFactory.getUserStateAdapter().setSelectedOn(host.getId())
String q = request.getQueryString();
String action = request.getParameter("action");
String run = request.getParameter("run");
String sid = request.getParameter("id");
int nid = 0;
try{
	nid = Integer.parseInt(sid);
}
catch(Exception e){
	
}
if(!MyStringUtils.equals("true",run)){
	return;
}
if(nid != 0){
	//request.getRequestDispatcher("profile.jsp").forward(request,response);
	if(MyStringUtils.equals("setstar",action)){
		SnsAdapterFactory.getUserStateAdapter().setSelectedOn(nid);
		try{
			/*
			Class<?> c = Class.forName("com.xiaonei.wService.invitereward.InviteRewardAdapter");
			for (Method m :c.getDeclaredMethods()){
                //System.out.println(m.getName());
                if(  m.getName().equals("addStar")){
                    //Class<?>[] ts = m.getParameterTypes();
                    //for (int i = 0; i < ts.length; i++) {
                    //    System.out.println(ts[i].getSimpleName());
                    //}
                    m.invoke(c.newInstance(),nid);
                }
            }*/
            //System.out.println("1 times");
			//com.xiaonei.wService.invitereward.InviteRewardAdapter.getInstance().addStar(334263054);
		}
		catch(Throwable e){
			out.println(e);
		}
	}
	else if(MyStringUtils.equals("removestar",action)){
		SnsAdapterFactory.getUserStateAdapter().setSelectedOff(nid);
	}
}

response.sendRedirect("profile.jsp?"+"id="+sid+"&run="+run);
%>