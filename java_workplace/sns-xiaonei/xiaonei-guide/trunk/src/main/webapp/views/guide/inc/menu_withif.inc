<%
Boolean is_app_guide = (Boolean)request.getAttribute("is_app_guide");
if(is_app_guide == null) {is_app_guide =  false;}
%>

<c:set var="isappguide" value="<%=is_app_guide?1:0%>" />

<c:if test="${isappguide==1}">
	<c:set var="cleanpage" value="true" />
	<%@ include file="/inc/header.inc"%>
</c:if>
<c:if test="${0==isappguide}">
	<%@ include file="menu_nolinks.inc"%>
</c:if>

