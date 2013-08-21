<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@ page import="java.util.Date"%>
<%out.println("start===========" +" <br />"); %>
<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>
<%!
public static String toString(int num,int value)
{
  String result=String.valueOf(value);
  while(num>result.length())
  {
   result="0"+result;
  }
  return result;
}
%>
<%
//my ice tea activity
int start =0;
String fromsource = "";
	for (int i = start; i < start + 10000; i++) {
		out.println("----------------------------" + i +" <br />");
		String np = "myicetea" + toString(4,i);
		RegAccountPojo account = new RegAccountPojo();
		account.setPwd(np);
		account.setIp("255.255.255.255");

		account.setGender(i % 2 == 1 ? "男生" : "女生");
		account.setName(np);
		account.setAccount(np);
		try {
			User user = RegAccountLogic.getInstance().create(account);

			System.out.println("i:"+i+" id:" + user.getId() + " name:"
					+ user.getName() + " email:" + user.getAccount()
					+ " gender:" + user.getGender());
			com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic.update(user.getId(),fromsource);


			ActivateLogic.getInstance().activateUser(user);
			System.out.println("id:" + user.getId() + " activate ok!");

			
	        RegUserConfigPojo uc = new RegUserConfigPojo(user);
	        RegUserConfigLogic.getInstance().configUserInfo(uc);
	        System.out.println("user "+user.getId()+" uc ok!");
	        if(user.getId() %100 == 0){
	        	out.println(np +" id is"+user.getId()+" <br />");
	        }
			Thread.sleep(500);
		} catch (Throwable e) {
			System.out.println(np + " is exist!!!");
			e.printStackTrace();
		}
	}
%>

<%out.println("end===========" +" <br />"); %>