<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%
//---------------------------------------------------------------------------------------------------
//-- struts的select option 组件-/inc/strutsOptionBirthdayYearComponent.inc
//-- 外部jsp调用此inc可以传入2个参数:
//-- strutsOptionYearComponent_begin 必须的，最上面的的option的年份
//-- strutsOptionYearComponent_end 必须的，最下面的的option的年份 
//---------------------------------------------------------------------------------------------------
%>
<script type="text/javascript">
        var pleaseSelectOption = document.getElementsByName("birth_year")[0];
        var myOption = pleaseSelectOption.options[0];
        if(myOption != null && myOption.text == '请选择'){
                pleaseSelectOption.remove(0);
        }

</script>
<c:set var="browserType" value="${header['User-Agent']}" />
<c:set var="specialYear" value="${fn:indexOf(browserType, 'MSIE') > 0 ? '1995' : '1976'}" />

<c:choose>
        <c:when test="${strutsOptionYearComponent_begin >= strutsOptionYearComponent_end}">
                <c:set var="strutsOptionYearComponent_begin" value="${strutsOptionYearComponent_begin == 2011 ? 2012 : strutsOptionYearComponent_begin}" />
                <c:forEach begin="${strutsOptionYearComponent_end}" end="${strutsOptionYearComponent_begin}" varStatus="status">
                        <c:if test="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end) == specialYear}">
                                <option value="" selected="selected">请选择</option>
                                <option value="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}">${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}</option>
                        </c:if>

                        <c:if test="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end) != specialYear}">
                                <option value="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}">${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)}</option>
                                <c:if test="${(strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)) == '1960'}" >
                                        <option value="1760">60后</option>
                                </c:if>
                                <c:if test="${(strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)) == '1970'}" >
                                        <option value="1770">70后</option>
                                </c:if>
                                <c:if test="${(strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)) == '1980'}" >
                                        <option value="1780">80后</option>
                                </c:if>
                                <c:if test="${(strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end)) == '1990'}" >
                                        <option value="1790">90后</option>
                                </c:if>
                        </c:if>
                </c:forEach>
        </c:when>
        <c:otherwise>
                <c:forEach begin="${strutsOptionYearComponent_begin}" end="${strutsOptionYearComponent_end}" varStatus="status">
                        <c:set var="strutsOptionYearComponent_end" value="${strutsOptionYearComponent_end == 2011 ? 2012 : strutsOptionYearComponent_end}" />
                        <c:if test="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end) == '1960'}" >
                                <option value="1760">60后</option>
                        </c:if>
                        <c:if test="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end) == '1970'}" >
                                <option value="1770">70后</option>
                        </c:if>
                        <c:if test="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end) == '1980'}" >
                                <option value="1780">80后</option>
                        </c:if>
                        <c:if test="${strutsOptionYearComponent_begin-(status.index-strutsOptionYearComponent_end) == '1990'}" >
                                <option value="1790">90后</option>
                        </c:if>
                        <option value="${status.index}">${status.index}</option>
                </c:forEach>
        </c:otherwise>
</c:choose>