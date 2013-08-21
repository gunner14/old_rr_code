<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<p class="avatar"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${item.id}"><img alt="${item.name}" src="${item.head_Url}"/></a></p>
<div class="info">
    <dl>
        <dt>名称:</dt>
        <dd><strong><a href="${applicationScope.urlGroup}/GetTribe.do?id=${item.id}">${item.name}</a></strong></dd>
        <dt>人数:</dt>
        <dd>${item.userCount}</dd>
    </dl>
</div>
   
