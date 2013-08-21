<%@ page pageEncoding="UTF-8" %>
<c:if test="${(!empty rate) && (rate != 0)}"><span class="rate"><span class="star${rate}">${rate / 2} / 5</span></span></c:if>