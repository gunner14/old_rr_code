<%@ page contentType="text/html;charset=UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>

<ul class="left-con">
	<li <c:if test="${navName eq 'index'}">class="curselect"</c:if>><a hidefocus="true" href="http://wiki.renren.com/adminwiki">管理首页</a></li>
	<li <c:if test="${navName eq 'createWiki'}">class="curselect"</c:if>><a hidefocus="true" href="http://wiki.renren.com/adminwiki/createWikiPage">创建wiki</a></li>
	<li <c:if test="${navName eq 'superAdmin'}">class="curselect"</c:if>><a hidefocus="true" href="http://wiki.renren.com/adminwiki/superAdmin">运营人员</a></li>
	<li <c:if test="${navName eq 'wikiWhite'}">class="curselect"</c:if>><a hidefocus="true" href="http://wiki.renren.com/adminwiki/wikiWhite">白名单</a></li>
	<li <c:if test="${navName eq 'operateIndex'}">class="curselect"</c:if>><a hidefocus="true" href="http://wiki.renren.com/adminwiki/operate-index/movie/latest">首页运营</a></li>
	<li <c:if test="${navName eq 'slide'}">class="curselect"</c:if>><a hidefocus="true" href="http://wiki.renren.com/adminwiki/operate-index/slide/page/movie">Slide运营</a></li>
</ul>