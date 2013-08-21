<%@ page pageEncoding="utf-8" %>
<%@ page import="com.xiaonei.commons.interceptors.access.track.decision.Decision"%>
<%@ page import="com.xiaonei.commons.interceptors.access.track.decision.DecisionResolver"%>
<c:choose>
	<c:when test="${visiter.id%10==0 || visiter.id%10==1 || visiter.id%10==2 || visiter.id%10==3 || visiter.id%10==4}">
<%
	if(request.getAttribute(DecisionResolver.DECISION_KEY)!=null){
	    ((Decision)request.getAttribute(DecisionResolver.DECISION_KEY)).makeDecision("guide_zottf"); 
	}
%>	
	</c:when>
	<c:when test="${visiter.id%10==5 || visiter.id%10==6 || requestScope.guide_page=='guide_netbarxfive'}">
<%
	if(request.getAttribute(DecisionResolver.DECISION_KEY)!=null){
	    ((Decision)request.getAttribute(DecisionResolver.DECISION_KEY)).makeDecision("guide_fsse"); 
	}
%>	
	</c:when>	
	<c:otherwise>
<%
	if(request.getAttribute(DecisionResolver.DECISION_KEY)!=null){
	    ((Decision)request.getAttribute(DecisionResolver.DECISION_KEY)).makeDecision("guide_fsse"); 
	}
%>	
	</c:otherwise>
</c:choose>





