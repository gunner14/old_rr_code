<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@ page import="java.util.Date"%>


<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%
if(!Globals.DEBUG_ENV){
	out.println("");
	return;
}
%>

<style>
	input {
		border:1px solid blue;
	}

</style>
<form action="">

from:<input name="fromsource" value="" />
id:<input name="regaccount" value="renren_r_" />
<select name="gender">
gender:<option value="男生">男生</option>
<option value="女生">女生</option>
<option value="">春哥</option>
</select>
name:<input name="name" value="李了一" />
<input type="submit" />
<input type="hidden" name="run" value="true">
</form>


<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%
if(!Globals.DEBUG_ENV){
	out.println("are you a hacker? :)");
	return;
}
String from = request.getParameter("fromsource");
String regaccount = request.getParameter("regaccount");
String gender = request.getParameter("gender");
String name = request.getParameter("name");
if(gender.equals(""))gender=null;
	for (int i = 0; i < 1; i++) {
		RegAccountPojo account = new RegAccountPojo();
		account.setPwd("renren");
		account.setIp("255.255.255.255");

		//account.setGender(i % 2 == 1 ? "男生" : "女生");
		account.setGender(gender);
		account.setName(name+from);
		account.setAccount(regaccount);
		try {
			User user = RegAccountLogic.getInstance().create(account);

			System.out.println("i:"+i+" id:" + user.getId() + " name:"
					+ user.getName() + " email:" + user.getAccount()
					+ " gender:" + user.getGender());
			com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic.update(user.getId(),from);


			ActivateLogic.getInstance().activateUser(user);
			System.out.println("id:" + user.getId() + " activate ok!");
			
			
	        RegUserConfigPojo uc = new RegUserConfigPojo(user);
	        RegUserConfigLogic.getInstance().configUserInfo(uc);
	        System.out.println("user "+user.getId()+" uc ok!");
			Thread.sleep(500);
			out.println("id:"+user.getId());
		} catch (Throwable e) {
			out.println(regaccount+" :注册失败! 99%的可能性是id已存在……");
			System.out.println("email:renren_" + i);
			e.printStackTrace();
			return;
		}
	}
	out.println("<input value="+regaccount+" />"+" :注册并激活成功!");
%>