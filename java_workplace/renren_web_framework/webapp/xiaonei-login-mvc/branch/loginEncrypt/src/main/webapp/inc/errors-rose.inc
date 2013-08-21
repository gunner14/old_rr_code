<%@ taglib prefix="spring" uri="http://www.springframework.org/tags" %>
<%@ taglib prefix="rose" uri="http://paoding.net/rose" %>
<rose:flash key="msg" prefix="<div class=messages_div>" suffix="</div>"/>
<c:forEach items="${pageMessages.messages}" var="pageMessage">
	<c:if test="${pageMessage.type=='message'}">
		<div class="messages_div">${pageMessage}</div>
	</c:if>
	<c:if test="${pageMessage.type=='warning'}">
		<div class="warning_div">${pageMessage}</div>
	</c:if>
	<c:if test="${pageMessage.type=='error'}">
		<div class="errors_div">${pageMessage}</div>
	</c:if>
</c:forEach> 

