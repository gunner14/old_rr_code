<%if("i".equals(request.getParameter("g"))){
	JSONObject jo = new JSONObject();
	for(Map.Entry<String,PpLogger> enLogger:
		PassportLoggerFactory.getLoggerMap().entrySet()) {
		jo.put(enLogger.getKey(),enLogger.getValue().isEnable());
	}
	InetAddress addr = InetAddress.getLocalHost();
	String ip = addr.getHostAddress();
	jo.put("ip",ip);
	
	out.print(jo.toString());
	return;
}
%>
<%if("l".equals(request.getParameter("s"))){
	JSONArray ja = JSONArray.fromObject(request.getParameter("t"));
	for(Map.Entry<String,PpLogger> enLogger:PassportLoggerFactory.getLoggerMap().entrySet()){
		if(ja.contains(enLogger.getKey())){
			enLogger.getValue().setEnable(true);
		}else{
			enLogger.getValue().setEnable(false);
		}
	}
	return;
}
%>
<%@ page pageEncoding="UTF-8"%>
<%@page import="com.xiaonei.login.logic.logger.*"%>
<%@page import="net.sf.json.*"%>
<%@page import="java.util.*"%>
<%@page import="java.net.*"%>
<%@page import="java.io.*"%>
<%@page import="net.sf.json.util.*"%>
<%
	String[] ipList = //ip list 
	{//start
		"127.0.0.1",
		"10.3.16.102",
		"10.3.16.103",
		"10.3.16.104",
		"10.3.16.15",
		"10.3.16.26",
		"10.3.16.27",
		"10.3.16.28",
		"10.3.16.29",
		"10.3.16.30",
		"10.3.16.42",
		"10.3.16.44",
		"10.3.16.45",
		"10.3.16.46",
		"10.3.16.47",
		"10.3.16.48",
		"10.3.16.49",
		"10.3.16.50",
		"10.3.16.81",
		"10.3.16.82",
		"10.3.16.84",
		"10.3.16.85",
		"10.3.16.86",
		"10.3.16.87",
		"10.3.16.88",
		"10.3.16.89",
		"10.3.16.90",
		"10.3.17.23",
		"10.3.17.24",
		"10.3.17.25",
		"10.3.17.26",
		"10.3.17.27",
		"10.3.17.28",
		"10.3.17.29",
		"10.3.17.30",
		"10.3.17.31",
		"10.3.17.32",
		"10.3.17.33",
		"10.3.17.34",
		"10.3.18.170",
		"10.3.18.178",
		"10.3.18.179",
		"10.3.18.189",
		"10.3.18.206",
	};//end
%>

<%
	if ("POST".equals(request.getMethod())){
		for(String ip : ipList){
			JSONArray logType=new JSONArray();
			for(String lt : request.getParameterValues("loggerType")){
				if(lt.startsWith(ip+"_")){
					logType.add(lt.replaceFirst(ip+"_",""));
				}
			}
			URL url = new URL("http://"+ip+"/logCtrl.jsp?s=l&t="+logType.toString());
			HttpURLConnection uc = null;
			BufferedReader in = null;
			try {
				uc = (HttpURLConnection) url.openConnection();
				uc.connect();
				in = new BufferedReader(new InputStreamReader(uc.getInputStream()));
				in.readLine();
			} catch (Throwable e) {
			} finally {
				if (in != null)
					in.close();
			}	
		}
	}
%>

<%
	Map<String,JSONObject> loggerMap = new HashMap<String,JSONObject>();
	for(String ip:ipList){
		URL url = new URL("http://"+ip+"/logCtrl.jsp?g=i");
		HttpURLConnection uc = null;
		BufferedReader in = null;
		uc = (HttpURLConnection) url.openConnection();
		uc.connect();
		String msg = null;
		try {
			in = new BufferedReader(new InputStreamReader(uc.getInputStream()));
			msg = in.readLine();
		} catch (Throwable e) {
		} finally {
			if (in != null)
				in.close();
		}
		if(!"".equals(msg)&& null != msg){
			JSONObject jo = JSONObject.fromObject(msg);
			loggerMap.put(ip,jo);
		}
	}
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
	<head>
		<title>Logger Setup</title>
		<meta http-equiv="pragma" content="no-cache">
		<meta http-equiv="cache-control" content="no-cache">
		<meta http-equiv="expires" content="0">
	</head>
	
	<body>
		Please choose the logger and the ip/host:
		<FORM METHOD="Post">
		<TABLE CELLPADDING="5" CELLSPACING="0" BORDER="1">
			<TR>
				<TD COLSPAN="2">
					<H2>Enable Disable Passport Logger</H2>
				</TD>
			</TR>
			<TR>
				<TD>
					<table border="1">
						<tr>
							<td>
								&nbsp;
							</td>
							<td>
								&nbsp;
							</td>
							<%for(Map.Entry<String,PpLogger> enLogger:PassportLoggerFactory.getLoggerMap().entrySet()) {%>
									<td>
										<%=enLogger.getKey().replaceFirst("passportLog-","") %>
									</td>
							<%} %>
						</tr>
						<tr>
							<td align="center">
								&nbsp;
							</td>
								<td>
									<input type="checkbox" onclick="seAll();">
								</td>
							<%for(Map.Entry<String,PpLogger> enLogger:PassportLoggerFactory.getLoggerMap().entrySet()) {%>
									<td align="center">
										<input type="checkbox" onclick="seType(<%=enLogger.getKey() %>);">
									</td>
							<%} %>
						</tr>
						<%for(Map.Entry<String,JSONObject> jologger:loggerMap.entrySet()) {%>
							<tr>
								<td>
									<%=jologger.getKey() %>
								</td>
								<td>
									<input type="checkbox" onclick="seIp('<%=jologger.getKey() %>')">
								</td>
								<%for(Map.Entry<String,PpLogger> enLogger:PassportLoggerFactory.getLoggerMap().entrySet()) {%>
										<td align="center">
											<input type="checkbox" 
											name="loggerType" 
											value="<%=jologger.getKey() %>_<%=enLogger.getKey() %>" 
											id="<%=jologger.getKey() %>_<%=enLogger.getKey() %>" 
											<%=enLogger.getValue().isEnable()?"checked":"" %> />
										</td>
								<%} %>
							</tr>
						<%} %>
					</table>
				</TD>
			</TR>
			<TR>
				<TD COLSPAN="2">
					<INPUT TYPE="Submit" NAME='Submit' VALUE='Apply Changes'><BR>
				</TD>
			</TR>
		</TABLE>
		</FORM>
	</body>
</html>
<script type="text/javascript">
function seAll(){
	var boxes = document.getElementsByName("loggerType");
	for(var i =0; i<boxes.length; i++){
	    boxes[i].checked=checked;
	}
	
}
function seIp(ip,checked){
	var boxes = document.getElementsByName("loggerType");
	for(var i =0; i<boxes.length; i++){
	//  alert(boxes[i].id.lastIndexOf("passportLog-loginSafe"))
	  if(boxes[i].id.indexOf(ip) != -1){
	    boxes[i].checked=checked;
	  }
	}
}
function seType(type){
	var boxes = document.getElementsByName("loggerType");
	for(var i =0; i<boxes.length; i++){
	  if(boxes[i].id.lastIndexOf(type) != -1){
	    boxes[i].checked=checked;
	  }
	}
}
"var boxes = document.getElementsByName(\"loggerType\");\r\n	for(var i =0; i<boxes.length; i++){\r\n	 	    boxes[i].checked=checked;\r\n	 	}"
</script>
