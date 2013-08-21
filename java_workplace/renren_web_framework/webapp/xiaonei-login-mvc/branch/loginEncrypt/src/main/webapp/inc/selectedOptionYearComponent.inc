<%@ page pageEncoding="UTF-8" %>
<%
//---------------------------------------------------------------------------------------------------
//-- select option 组件-/inc/selectOptionYearComponent.inc
//-- 外部jsp调用此inc可以传入2个参数:
//-- selectOptionYearComponent_begin 必须的，最上面的的option的年份
//-- selectOptionYearComponent_end 必须的，最下面的的option的年份 
//-- selectedOptionYearValue 选择的数值
//---------------------------------------------------------------------------------------------------
%>
<c:choose>
	<c:when test="${strutsOptionYearComponent_begin >= strutsOptionYearComponent_end}">
		<c:forEach begin="${strutsOptionYearComponent_end}" end="${strutsOptionYearComponent_begin}" varStatus="status">
		<c:choose>
			<c:when test="${(strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end))==selectedOptionYearValue}">
			<option value="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}"  selected="selected">${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}</option>
			</c:when>			
			<c:otherwise>
			<option value="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}">${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}</option>
			</c:otherwise>
		</c:choose>
		</c:forEach>
	</c:when>
	<c:otherwise>
		<c:forEach begin="${strutsOptionYearComponent_begin}" end="${strutsOptionYearComponent_end}" varStatus="status">
			<c:if test="${status.index!=selectedOptionYearValue}"><option value="${status.index}">${status.index}</option></c:if>
			<c:if test="${status.index==selectedOptionYearValue}"><option value="${status.index}" selected="selected">${status.index}</option></c:if>
		</c:forEach>
	</c:otherwise>
</c:choose>