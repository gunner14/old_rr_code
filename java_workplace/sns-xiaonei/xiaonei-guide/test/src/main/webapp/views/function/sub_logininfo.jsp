<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_logininfo -->
<%@page import="com.xiaonei.xce.scorecache.DefaultScoreCache"%>
<%@page import="com.xiaonei.xce.scorecache.ScoreCacheAdapter"%>
<%@page import="java.text.SimpleDateFormat"%>
<%@page import="com.dodoyo.datum.logic.KeepUseConfigLogic"%><div></div>
<%
//User host;
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<ul>
    <%
    if(true){
    %>
		<h2>Login Info</h2>
	<%
		int userId = host.getId();//用户id
	    DefaultScoreCache scoredata = ScoreCacheAdapter.getInstance().getScoreData(userId);//取用户积分对象
		/*
	    List<Boolean> activedays = new ArrayList<Boolean>();//scoredata.getActiveDays();//从积分对象中取活跃天数
	    int i = 0;
	    for(boolean b: activedays){
	    	out.println((i++)+""+b+"<br />");
	    }
	    int absentcount=1;//开始统计没上线的天数
	    for( ; absentcount<=29; ++absentcount ){
	        if( activedays.get(absentcount) ){
	            break;
	        }
	    }//至此absentcount表示上次上线那天距离今天的天数;
	    Calendar c = Calendar.getInstance();
	    c.add(Calendar.DAY_OF_MONTH, -absentcount);
	    c.set(Calendar.HOUR_OF_DAY, 0);
	    c.set(Calendar.MINUTE, 0);
	    c.set(Calendar.SECOND, 0);
	    c.set(Calendar.MILLISECOND, 0);
	    long timelastzero = c.getTimeInMillis();//表示上次登录那天的0点
	    long timenow = System.currentTimeMillis();//当前时间
	    */
	    if(scoredata == null){
	    	 out.println("scoredata null");
	    }
	    else{
		    SimpleDateFormat fmt = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		    out.println("Last Login:"+fmt.format(scoredata.getLastLoginTime())+"<br />");
		    out.println("Keep Login:"+scoredata.getContinueLoginDays()+"<br />");
		    out.println("History Login:"+scoredata.getHistoryLoginDays()+"<br />");
	    }
	    
	    boolean isk = KeepUseConfigLogic.getInstance().isKeepUse(userId);
	    out.println("IsKeepUse:"+isk+"<br />");
	%>
	 	    
    <%
    }
    %>
    <table>
        <tr><td>
    
        </td></tr>
    <%
    %>
    </table>
</ul>
<!-- ------------} --> 
<%
}
%>