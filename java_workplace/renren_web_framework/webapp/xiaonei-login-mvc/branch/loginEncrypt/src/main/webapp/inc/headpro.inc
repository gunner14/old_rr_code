<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>

<c:choose>
	<c:when test="${is_v6_user && (empty vipStyle || vipStyleId==2)}">
		<%@ include file="/inc/n/head2.inc" %>
	</c:when>
	<c:otherwise>
		<%@ include file="/inc/n/head1.inc" %>
	</c:otherwise>
</c:choose>
