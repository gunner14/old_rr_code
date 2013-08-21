<%@page import="java.util.Map.Entry"%>
<%@page import="java.util.HashMap"%>
<%@page import="java.util.Map"%>
<%@page import="org.apache.commons.lang.StringUtils"%>
<%@page import="java.lang.reflect.Method"%>
<%@page import="com.kaixin.register.form.RegForm"%>
<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<script type="text/javascript" src="http://s.xnimg.cn/js/form_control.js?ver=$revxxx$"></script>
<%
RegForm regform = (RegForm)request.getAttribute("regform");
if(null == regform){
	regform = (RegForm)request.getAttribute("regForm");
}
if(null != regform){
Method[] ms = regform.getClass().getMethods();
Map<String,String> map = new HashMap<String,String>();
Map<String,String> mapcity = new HashMap<String,String>();
for (Method m : ms) {
	if (StringUtils.equals(m.getName(), "getClass")) {

	} else if (StringUtils.startsWith(m.getName(), "get")) {
		String mname = StringUtils.removeStart(m.getName(), "get");
		mname = (mname.substring(0, 1).toLowerCase() + mname.substring(1));
		Object obj = m.invoke(regform);
		String value = obj==null?"":obj.toString();
		if(StringUtils.isNotEmpty(value)&&!"0".equals(value) && !StringUtils.contains(value, "ActionServletWrapper")){
		    if(StringUtils.contains(m.getName(), "city")){
		        mapcity.put(mname, value);
		    }else{
				map.put(mname, value);
		    }
		}
	}
}
%>
<script>
var valueSetter = new ChangeValue('regform');
<%for(String e: map.keySet()){%>
valueSetter.setValue('<%=e %>', '<%=map.get(e) %>');
<%}%>
<%if(mapcity!=null && mapcity.size() >0){%>
Profile.getInCityListOfProvince($("homeprovince").selectedIndex, 'homecitycode');
<%}%>
<%for(String e: mapcity.keySet()){%>
valueSetter.setValue('<%=e %>', '<%=mapcity.get(e) %>');
<%}%>
</script>
<%}%>