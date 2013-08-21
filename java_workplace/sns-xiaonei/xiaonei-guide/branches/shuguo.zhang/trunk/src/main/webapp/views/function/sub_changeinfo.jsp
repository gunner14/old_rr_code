<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_changeinfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>

<%@page import="com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic"%>
<%@page import="com.xiaonei.reg.register.model.RegExtraMsg"%>
<%@page import="java.util.HashMap"%>
<%@page import="com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.reg.guide.logic.FillInfoLogic"%>
<%@page import="com.xiaonei.reg.register.logic.additional.ActivateLogic"%>
<%@page import="java.lang.reflect.Method"%>
<%@page import="com.xiaonei.platform.core.model.UserPassport"%>
<%@page import="com.xiaonei.platform.core.model.UserName"%>
<%@page import="com.xiaonei.wservice.userscore.login.client.LoginRecordAdapter"%>
<%@page import="com.xiaonei.wservice.userscore.login.LoginDays"%>
<%@page import="com.xiaonei.xce.scorecache.ScoreCacheAdapter"%>
<%@page import="com.xiaonei.userscore.reward.RewardLogic"%>
<%@page import="com.xiaonei.userscore.reward.RewardType"%>
<%@page import="com.dodoyo.datum.client.DatumNetworkClient"%>
<%@page import="mop.hi.oce.domain.network.UniversityInfo"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null && chost != null){
	//From Source
	String newfrom = request.getParameter("fromsource");
	if(MyStringUtils.isNullOrEmpty(newfrom)){
		if(!MyStringUtils.isNullOrEmpty(request.getParameter("fromsource_text"))){
			newfrom = request.getParameter("fromsource_text");
			RegFromSourceLogic.update(host.getId(),newfrom);
		}
	}
	else{
	    RegFromSourceLogic.update(host.getId(),newfrom);
	}
	
	//Extra Info
	String extrakey = request.getParameter("extrakey");
    String extravalue = request.getParameter("extravalue");
    if(!MyStringUtils.isNullOrEmpty(extrakey) && !MyStringUtils.isNullOrEmpty(extravalue)){
        RegExtraMsg msg = new RegExtraMsg();
        msg.setInput_key(extrakey);
        msg.setInput_value(extravalue);
        Map<String,RegExtraMsg> msgMap = new HashMap<String,RegExtraMsg> ();
        msgMap.put(extrakey,msg);
        RegExtraMsgLogic.getInstance().saveMsg(host,msgMap);
    }
	
    //Stage
    String newuserstage = request.getParameter("userstage");
    if(MyStringUtils.isNullOrEmpty(newuserstage)){
    	if(!MyStringUtils.isNullOrEmpty(request.getParameter("userstage_text"))){
            newuserstage = request.getParameter("userstage_text");
            Integer n_newuserstage = MyStringUtils.stringToObjectnew(newuserstage.toString(),Integer.class);
            FillInfoLogic.getInstance().saveStage(host, n_newuserstage);
        }
    }
    else{
    	Integer n_newuserstage = MyStringUtils.stringToObjectnew(newuserstage.toString(),Integer.class);
    	FillInfoLogic.getInstance().saveStage(host, n_newuserstage);
    }
    
    
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("userpassportstatus_text"))){
        Integer n_userpassportstatus = MyStringUtils.stringToObjectnew(request.getParameter("userpassportstatus_text").toString(),Integer.class);
        //host.getUserPassportInfo().setStatus(n_userpassportstatus);
        if(n_userpassportstatus == 0){
            ActivateLogic.getInstance().activateUser(host);
        }
        UserPassport up = SnsAdapterFactory.getUserPassportAdapter().getUserPassport(host.getId()); 
        up.setStatus(n_userpassportstatus);
        SnsAdapterFactory.getUserPassportAdapter().setUserPassport(host.getId(),up);
    }
    
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("userdomain_text"))){
        String domain = request.getParameter("userdomain_text");
        UserPassport up = SnsAdapterFactory.getUserPassportAdapter().getUserPassport(host.getId());
        up.setDomain(domain);
        SnsAdapterFactory.getUserPassportAdapter().setUserPassport(host.getId(),up);
    }
    
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("username_text"))){
        String username = request.getParameter("username_text");
        UserName un = SnsAdapterFactory.getUserNameAdapter().getUserName(host.getId());
        un.setName(username);
        SnsAdapterFactory.getUserNameAdapter().setUserName(host.getId(),un);
    }
    
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("userinvitee_text"))){
        Integer n_userinvitee_text = MyStringUtils.stringToObjectnew(request.getParameter("userinvitee_text").toString(),Integer.class);
        //host.getUserPassportInfo().setStatus(n_userpassportstatus);
        out.println(chost.getId()+" invite "+n_userinvitee_text);
        if(n_userinvitee_text != 0){
            try{
                /*Class<?> c = Class.forName("com.xiaonei.wService.invitereward.InviteRewardAdapter");
                for (Method m :c.getDeclaredMethods()){
                    //System.out.println(m.getName());
                    if(  m.getName().equals("addInvite")){
                        //Class<?>[] ts = m.getParameterTypes();
                        //for (int i = 0; i < ts.length; i++) {
                        //    System.out.println(ts[i].getSimpleName());
                        //}
                        m.invoke(c.newInstance(),81120,host.getId());
                    }
                }*/
                com.xiaonei.wService.invitereward.InviteRewardAdapter.getInstance().addInvite(chost.getId(),host.getId());
            }
            catch(Throwable e){
                out.println(e);
            }
        	
        }
    }
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("userinviteestar"))){
    	if(MyStringUtils.equals("on",request.getParameter("userinviteestar"))){
    	      try{
    	    	  out.println("userinviteestar:"+request.getParameter("userinviteestar"));
    	            /*Class<?> c = Class.forName("com.xiaonei.wService.invitereward.InviteRewardAdapter");
    	            for (Method m :c.getDeclaredMethods()){
    	                //System.out.println(m.getName());
    	                if(  m.getName().equals("addStar")){
    	                    //Class<?>[] ts = m.getParameterTypes();
    	                    //for (int i = 0; i < ts.length; i++) {
    	                    //    System.out.println(ts[i].getSimpleName());
    	                    //}
    	                    m.invoke(c.newInstance(),host.getId());
    	                }
    	            }*/
    	    	  com.xiaonei.wService.invitereward.InviteRewardAdapter.getInstance().addStar(host.getId());
    	        }
    	        catch(Throwable e){
    	            out.println(e);
    	        }
    	}
    }
    
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("setkeeponuse"))){
    	if(MyStringUtils.equals("on",request.getParameter("setkeeponuse"))){
    	        try{
    	    	  com.dodoyo.datum.logic.KeepUseConfigLogic.getInstance().turnOn(host.getId());
	   	        }
	   	        catch(Throwable e){
	   	            out.println(e);
	   	        }
    	}
    	else{
	 		try{
	    		com.dodoyo.datum.logic.KeepUseConfigLogic.getInstance().turnOff(host.getId());
	 	    }
 	        catch(Throwable e){
 	        	out.println(e);
 	        }
    	}
    }
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("cankeeponuse"))){
    	if(MyStringUtils.equals("on",request.getParameter("cankeeponuse"))){
 	        try{
 	    		SnsAdapterFactory.getUserStateAdapter().setKeepUseOn(host.getId());
 	        }
 	        catch(Throwable e){
 	            out.println(e);
 	        }
    	}
    	else{
	 		try{
	 			SnsAdapterFactory.getUserStateAdapter().setKeepUseOff(host.getId());
	 	    }
 	        catch(Throwable e){
 	        	out.println(e);
 	        }
    	}
    }
    
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("univnetworkid"))){
        int univId = 0;
        int univYear = 0;
        String univDept = request.getParameter("univnetworkdept");
        String univName = request.getParameter("univnetworkname");
        try{
            univId = Integer.parseInt(request.getParameter("univnetworkid"));
            univYear = Integer.parseInt(request.getParameter("univnetworkyear"));
        }
        catch(Exception e){
            
        }
        if(univId!=0){
            boolean setResult = DatumNetworkClient.getInstance().setCurrentNetWork(univId, host);
            UniversityInfo university = new UniversityInfo();
            university.setUniversityId(univId);
            university.setDepartment(univDept);
            university.setUniversityName(univName);
            university.setEnrollYear(univYear);
            university.setUserId(host.getId());
          
        	DatumNetworkClient.getInstance().addOneUnivNetwork(host, university);
        	DatumNetworkClient.getInstance().setCurrentNetWork(university.getUniversityId(), host);
        }
    }
    
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("keeplogin_text"))){
        Integer n_keeplogin = MyStringUtils.stringToObjectnew(request.getParameter("keeplogin_text").toString(),Integer.class);
        
        int userId = host.getId();
        int cd = n_keeplogin;
        
        
        LoginDays d = LoginRecordAdapter.getInstance().getLoginDays(userId);
        int hd = d.getHistroyDays();
        //System.out.println(" hd:"+hd);
        
        if (d == null) {
            //System.err.println("no days");
            return;
        }
        if (cd >= 0) {
            d.setContinueDays(cd);
        }

        //MemcacheAdapter.setLoginDays(d);
        //LoginRecordDao.updateLogindays(Arrays.asList(d), userId % 100);
        ScoreCacheAdapter.getInstance().setLoginDays(userId, hd, cd, System.currentTimeMillis(), 4);
        
    }
    
    if(!MyStringUtils.isNullOrEmpty(request.getParameter("keeplogin_event_text"))){
        Integer n_keeplogin = MyStringUtils.stringToObjectnew(request.getParameter("keeplogin_event_text").toString(),Integer.class);
        int userId = host.getId();
        //UserScoreEventAdapter.getInstance().sendEvent(userId, eventContent);
        RewardLogic.getInstance().sendAward(userId,RewardType.LOGINMORE_20LOTTERY,null);
        
    }
    
    
%>

<!-- {------------ --> 

<%
String change = request.getParameter("change");
if(MyStringUtils.equals("on",change)){
%>
<h2 class="title">Change Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">

    <%
    if(MyStringUtils.equals("on",change)){
    %>
    <font color="red">此模块有风险，操作需谨慎！</font>
    <hr size="1"/>
    <form action="">
		[<%=host.getId()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />改成:
		<select name="fromsource">
		<option value="">-右边手填-</option>
		<option value="SC_REG">纯普通</option>
		<option value=":NETBAR">纯网吧</option>
		<option value="S_COOKREG:NETBAR">餐厅网吧</option>
		<option value="S_FARMREG:NETBAR">农场网吧</option>
		<option value="S_MZMG:NETBAR">面馆网吧</option>
		<option value="S_COOKREG">纯餐厅</option>
		<option value="S_FARMREG">纯农场</option>
		<option value="S_MZMG">纯面馆</option>
		<option value="APPTL:LOVER">LOVER</option>
		</select>
		<%
		String oldfrom = RegFromSourceLogic.getFromSource(host);
		%>
		
		填写:<input name="fromsource_text" size="10" value="<%=oldfrom==null?"":oldfrom%>" />
		<input type="submit" value="Go"/>
		<input type="hidden" name="run" value="true">
	</form>
	<hr size="1"/>
	<form method="get" action="">
        [<%=host.getId()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />添加: 
        <input type="text" id="s" name="extrakey" size="16"  value="reg_page_lover" /> 
        <input type="text" id="s" name="extravalue"  size="10" value="600039359" />
        <input type="submit" id="x" value="Go" />
        <input type="hidden" name="run" value="true">
    </form>
    <hr size="1"/>
    <form action="">
        [<%=host.getId()%>]-[<%=host.getStage()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />改成:
        <select name="userstage">
        <option value="">-右边手填-</option>
        <option value="0">无阶段</option>
        <option value="10">中学</option>
        <option value="20">大学</option>
        <option value="30">白领</option>
        <option value="90">均否</option>
        </select>
        <%
        %>
        
        填写:<input name="userstage_text" size="8" value="<%=host.getStage()%>" />
        <input type="submit" value="Go"/>
        <input type="hidden" name="run" value="true">
    </form>
    <hr size="1"/>
    
    <form action="">
        [<%=host.getId()%>]-[<%=host.getUserPassportInfo().getStatus()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />
        激活请填0:<input name="userpassportstatus_text" size="8" value="<%=host.getUserPassportInfo().getStatus()%>" />
        <input type="submit" value="Go"/>
        <input type="hidden" name="run" value="true">
    </form>
    <hr size="1"/>
    <form action="">
        [<%=chost.getId()%>](<%=chost.getName()%>)-[invite]-
        <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />
        who:<input name="userinvitee_text" size="8" value="<%=host.getId()%>" />
        star:<input type=checkbox name="userinviteestar" size="8" />
        <input type="submit" value="Go"/>
        <input type="hidden" name="run" value="true">
    </form>
    <hr size="1"/>
    
    <form action="">
        [<%=host.getId()%>]-[<%=host.getDomain()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />
        用户域:<input name="userdomain_text" size="8" value="<%=host.getDomain()%>" />
        <input type="submit" value="Go"/>
        <input type="hidden" name="run" value="true">
    </form>
    <hr size="1"/>
    <form action="">
        [<%=host.getId()%>]-[<%=host.getName()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />
        名字:<input name="username_text" size="8" value="<%=host.getName()%>" />
        <input type="submit" value="Go"/>
        <input type="hidden" name="run" value="true">
    </form>
    <hr size="1"/>
    <form action="">
        [<%=host.getId()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />
        程名显示:(CAN:<%=com.dodoyo.datum.logic.KeepUseConfigLogic.getInstance().isKeepUse(host.getId()) %>|ON:(<%=!com.dodoyo.datum.logic.KeepUseConfigLogic.getInstance().isTurnedOff(host.getId()) %>))
        <br />
        <%
        boolean cankeeponuse = com.dodoyo.datum.logic.KeepUseConfigLogic.getInstance().isKeepUse(host.getId());
        boolean setkeeponuse = !com.dodoyo.datum.logic.KeepUseConfigLogic.getInstance().isTurnedOff(host.getId());
        %>
        <input type="radio" checked="checked" value="on" name="cankeeponuse" <%if(cankeeponuse){out.print("checked");} %>>可以<input type="radio" value="off" name="cankeeponuse" <%if(!cankeeponuse){out.print("checked");} %>>不可以
        <br />
        <input type="radio" checked="checked" value="on" name="setkeeponuse" <%if(setkeeponuse){out.print("checked");} %>>显示<input type="radio" value="off" name="setkeeponuse" <%if(!setkeeponuse){out.print("checked");} %>>不显示
        <input type="submit" value="Go"/>
        <input type="hidden" name="run" value="true">
    </form>
    <hr size="1"/>
    <form action="">
    	[<%=host.getId()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />
    添加大学网络：<br />id:<input name="univnetworkid" id="univnetworkid" value="0"/><br />
    	name:<input name="univnetworkname" id="univnetworkname" value="大学"/><br />
    	dept:<input name="univnetworkdept" id="univnetworkdept" value="其它院系"/><br />
    	year:<input name="univnetworkyear" id="univnetworkyear" value="2008"/><br />
    	<input type="hidden" name="run" value="true">
    	<input type="submit" value="Go"/>
    </form>
    
    <hr size="1"/>
	    <%if (chost!=null && isMan(chost.getId())){%>
	    <form action="">
	        [<%=host.getId()%>]-[<%=host.getName()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />
	        改变Keep:<input name="keeplogin_text" size="8" value="1" />
	        <input type="submit" value="Go"/>
	        <input type="hidden" name="run" value="true">
	    </form>
	    <hr size="1"/>
	    <form action="">
	        [<%=host.getId()%>]-[<%=host.getName()%>] <input name="id" type="hidden" size="10" value="<%=host.getId()%>" />
	        事件:<input name="keeplogin_event_text" size="8" value="15" />
	        <input type="submit" value="Go"/>
	        <input type="hidden" name="run" value="true">
	    </form>
	    <hr size="1"/>
	    <%}%>
    
    <%
    }
    %>
    <table>
        <tr><td>
    
        </td></tr>
    </table>
<ul>
</ul>
</div>
<%
 }
%>
<!-- {------------ --> 
<%
}
%>