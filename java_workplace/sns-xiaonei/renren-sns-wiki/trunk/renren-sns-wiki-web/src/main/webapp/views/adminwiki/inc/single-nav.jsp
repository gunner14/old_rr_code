<%@ page contentType="text/html;charset=UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<div>
<c:choose>
<c:when test="${not empty wiki}">
<a href="http://wiki.renren.com/adminwiki/modifyWikiInfo/${wiki.id }?light=${light }&wikiType=${wikiType}">修改资料</a> | 
<a href="http://wiki.renren.com/adminwiki/modifyWikiHead/${wiki.id }?light=${light }&wikiType=${wikiType}">修改头像</a> | 
<a href="http://wiki.renren.com/adminwiki/modifyWikiCover/${wiki.id }?light=${light }&wikiType=${wikiType}">修改头图</a> | 
<a href="http://wiki.renren.com/adminwiki/modifyWikiWord/${wiki.id }?light=${light }&wikiType=${wikiType}">切词相关</a>
</c:when>
<c:otherwise>
wiki不存在
</c:otherwise>
</c:choose>
</div>
