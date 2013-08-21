<%@ page pageEncoding="UTF-8" %>
<%
//---------------------------------------------------------------------------------------------------
//-- struts的select option 组件-/inc/strutsOptionYearComponent.inc
//-- 外部jsp调用此inc可以传入2个参数:
//-- strutsOptionYearComponent_begin 必须的，最上面的的option的年份
//-- strutsOptionYearComponent_end 必须的，最下面的的option的年份 
//---------------------------------------------------------------------------------------------------
%>
<c:choose>
	<c:when test="${strutsOptionYearComponent_begin >= strutsOptionYearComponent_end}">
		<c:forEach begin="${strutsOptionYearComponent_end}" end="${strutsOptionYearComponent_begin}" varStatus="status">
			<option value="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}">${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}</:option>
		</c:forEach>
	</c:when>
	<c:otherwise>
		<c:forEach begin="${strutsOptionYearComponent_begin}" end="${strutsOptionYearComponent_end}" varStatus="status">
			<option value="${status.index}">${status.index}</:option>
		</c:forEach>
	</c:otherwise>
</c:choose>