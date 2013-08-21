<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@ page import="java.util.Date"%>
<%out.println("" +" <br />"); %>


<%
if(!Globals.DEBUG_ENV){
	out.println("");
	return;
}
%>

<%out.println("请输入想要Home化用户的Id们。以换行分隔。" +" <br />"); %>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="com.xiaonei.reg.guide.logic.StepStatusLogic"%>
<%@page import="com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.xce.userstate.UserStateInfo"%>
<%@page import="com.xiaonei.page.service.AdminService"%>
<%@page import="com.xiaonei.platform.core.model.UserState"%>
<%@page import="com.xiaonei.platform.core.opt.base.action.BaseAction"%>
<%@page import="com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil"%>
<%@page import="java.io.PrintStream"%>
<%@page import="com.xiaonei.reg.guide.util.GuideLogger"%><style>
	input {
		border:1px solid blue;
	}
	
	textarea{
		border:1px solid red;
	}

</style>
<form action="">


ids:<br />

<textarea name="ids" ></textarea><br />

<input type="submit" value="提交" />
<input type="hidden" name="run" value="true" />
</form>

<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%!
public void log(String s){
	GuideLogger.printLog(s);
	
}
%>
<%out.println("- Start -" +" <br />"); %>
<%
String ip = BizFilterUtil.getClientIP(request);
String ids = request.getParameter("ids");

String [] idlist = ids.split("\n");

for(String i : idlist){
	//out.println(i+" :<br />");
	log(ip + " process : [" + i+"]");
	int nid = 0;
	try{
		nid = Integer.parseInt(i.trim());
	}
	catch(Exception e){
		out.println("<font color=red>[FATA!]</font>"+i+" is not an integer! error~ "+e.toString()+" <br />");
	}
	
	if(nid != 0){
		try{
			User user = SnsAdapterFactory.getUserAdapter().get(nid);
			boolean isadmin = AdminService.getInstance().isAdmin(user.getId());
			if(isadmin){
				UserState state = SnsAdapterFactory.getUserAdapter().getStateAdapter().getUserState(user.getId());
				boolean isdone = SnsAdapterFactory.getUserAdapter().getStateAdapter().getUserState(user.getId()).isGuideDone();
				if(isdone){
					out.println("<font color=blue>[WARN!]</font>"+user.getId()+":["+user.getName()+"] has no PAGE! <br />");
				}
				else{
					SnsAdapterFactory.getUserAdapter().getStateAdapter().setGuideDoneOn(user.getId());
					out.println("<font color=green>[DONE!]</font>"+user.getId()+":["+user.getName()+"] is now a HOME user. <br />");
				}
			}
			else{
				out.println("<font color=red>[ERRO!]</font>"+user.getId()+":["+user.getName()+"] has no PAGE! <br />");
			}
		}
		catch(Exception e){
			out.println("<font color=red>[FAIL!]</font>"+i+" failed! "+e.toString()+" <br />");
		}
	}
}

/*

int nid = Integer.parseInt(id);

int step = StepStatusLogic.getStepStatus(nid);

out.println("id:"+nid+" step:"+step+" <br />");

User user = SnsAdapterFactory.getUserAdapter().get(nid);
String froms = RegFromSourceLogic.getFromSource(user);

out.println("id:"+nid+" from:"+froms);
*/

%>

<%out.println("- End -" +" <br />"); %>