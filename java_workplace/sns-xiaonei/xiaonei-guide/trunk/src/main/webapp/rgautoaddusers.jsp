<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@ page import="java.util.Date"%>
<%out.println("0 or 1===========" +" <br />"); %>


<%
if(!Globals.DEBUG_ENV){
	out.println("");
	return;
}
%>


<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="java.util.Random"%><style>
	input {
		border:1px solid blue;
	}

</style>
<form action="">

from:<input name="fromsource" value="" />
start:<input id="start" name="start" value="" />
<select name="gender">
gender:<option value="男生">男生</option>
<option value="女生">女生</option>
<option value="">春哥</option>
</select>
<input type="submit" />
<input type="hidden" name="run" value="true" />
<input type="button" value="test" onclick="dofill()"/>
</form>

<%
if(!"true".equals(request.getParameter("run"))){
	return;
}
%>

<%

String chars = "赵钱孙李周吴郑王冯陈褚卫蒋沈韩杨朱秦尤许何吕施张孔曹严华金魏陶姜戚谢邹喻柏水窦章云苏潘葛奚范彭郎鲁韦昌马苗凤花方俞任袁柳邓鲍史唐费廉岑薛雷贺倪汤藤殷罗华郝邬安常乐于时付皮卞齐康伍余元卜顾盈平黄和穆肖尹姚邵湛汪祁毛禹狄米贝明藏计伏成戴谈宋茅庞熊纪舒屈项祝董梁樊胡凌霍虞万支柯昝管卢英";
int charslen = chars.length() - 1;
int start =2550;
Random rand = new Random();
String fromsource = "";
String strstart = request.getParameter("start");
start = Integer.parseInt(strstart);
	for (int i = start; i < start+50; i++) {
		
		int len = rand.nextInt(2)+2;
		String name = "";
		
		for(int j = 0;j<len ; j++){
			int pos = rand.nextInt(charslen);
			name = name + chars.substring(pos,pos+1); 
		}
		
		if(i%10==0){
			out.println("----------------------------" + i +" <br />");
		}
		RegAccountPojo account = new RegAccountPojo();
		account.setPwd("renren");
		account.setIp("255.255.255.255");

		account.setGender(i % 2 == 1 ? "男生" : "女生");
		//account.setName("人人测试_" + i);
		account.setName(name);
		account.setAccount("renren_i" + i);
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
	        if(user.getId() %10 == 0){
	        	//out.println("renren_i" + i +" id is["+user.getId()+"] <br />");
	        } 
	        if(true){
	        	out.println("renren_i" + i +" id is["+user.getId()+"] "+name+" [renren]<br />");
	        }
			Thread.sleep(500);
		} catch (Throwable e) {
			System.out.println("email:renren_" + i);
			out.println("email:renren_i" + i+" 重复！");
			e.printStackTrace();
			break;
		}
	}
%>

<%out.println("end===========" +" <br />"); %>

<script language="javascript">
function dofill(){
document.getElementById("start").value = <%= start+50 %>;
}
</script>