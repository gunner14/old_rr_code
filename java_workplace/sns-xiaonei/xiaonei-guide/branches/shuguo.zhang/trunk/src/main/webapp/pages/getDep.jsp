<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<select name="department" class="inputtext" tabindex="7">
<option value="">请选择</option>
<c:forEach var="item" items="${requestScope.departmentList}">
  <option value="${item.name}" >${item.name}</option>
</c:forEach>
</select>
