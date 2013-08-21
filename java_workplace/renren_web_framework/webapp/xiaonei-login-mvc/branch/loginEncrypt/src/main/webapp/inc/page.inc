<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<ol class="pagerpro">
<c:if test="${empty itemPerPage}" >
	<c:set var="itemPerPage" value="20" />
</c:if>
<c:choose>
	<c:when test="${empty requestScope.curpage}">
		<c:set var="currentPage" value="0"/>
	</c:when>
	<c:otherwise>
		<c:set var="currentPage" value="${requestScope.curpage}"/> 
	</c:otherwise>
</c:choose>
<c:set var="navCount" value="${navCount}"/>
<c:set var="numberMod" value="${navCount % itemPerPage}"/>
<c:set var="countInt" value="${navCount-numberMod}"/>
<c:choose>
	<c:when test="${numberMod==0}">
		<fmt:parseNumber parseLocale="en_US" value="${countInt/itemPerPage}" integerOnly="true" var="pageNum" />
	</c:when>
	<c:otherwise>
		<fmt:parseNumber parseLocale="en_US" value="${countInt/itemPerPage + 1}" integerOnly="true" var="pageNum" />
	</c:otherwise>
</c:choose>

<!--<a href="#">共${lstCount}条${pageNum}</a>-->
<c:if test="${pageNum > 1}">
<c:if test="${pageNum<7}">
	<c:if test="${currentPage>0}">
		<li><a class="chn" title="上一页"  href="${navUrl}?curpage=${currentPage-1}${navUrlEnd}">上一页</a></li>
		<!--<a title="上一页" href="/${navUrl}?curpage=${currentPage-1}${navUrlEnd}">&lt; 上一页</a>-->
	</c:if>
	<c:if test="${currentPage>4}">
		<li><a class="chn" title="跳到第${currentPage-4}页" href="${navUrl}?curpage=${currentPage-5}${navUrlEnd}">${currentPage-4}</a></li>
	</c:if>
	<c:if test="${currentPage>3}">
		<li><a class="chn" title="跳到第${currentPage-3}页" href="${navUrl}?curpage=${currentPage-4}${navUrlEnd}">${currentPage-3}</a></li>
	</c:if>
	<c:if test="${currentPage>2}">
		<li><a class="chn" title="跳到第${currentPage-2}页" href="${navUrl}?curpage=${currentPage-3}${navUrlEnd}">${currentPage-2}</a></li>
	</c:if>
	<c:if test="${currentPage>1}">
		<li><a class="chn" title="跳到第${currentPage-1}页" href="${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a></li>
	</c:if>
	<c:if test="${currentPage>0}">
		<li><a class="chn" title="跳到第${currentPage}页" href="${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a></li>
	</c:if>
	<li class="current"><a href="#nogo">${currentPage+1}</a></li>
	<!--<span class="this-page">${currentPage+1}</span>-->
	<c:if test="${currentPage<pageNum-1}">
		<li><a class="chn" title="跳到第${currentPage+2}页" href="${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a></li>
	</c:if>
	<c:if test="${currentPage<pageNum-2}">
		<li><a class="chn" title="跳到第${currentPage+3}页" href="${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a></li>
	</c:if>
	<c:if test="${currentPage<pageNum-3}">
		<li><a class="chn" title="跳到第${currentPage+4}页" href="${navUrl}?curpage=${currentPage+3}${navUrlEnd}">${currentPage+4}</a></li>
	</c:if>
	<c:if test="${currentPage<pageNum-4}">
		<li><a class="chn" title="跳到第${currentPage+5}页" href="${navUrl}?curpage=${currentPage+4}${navUrlEnd}">${currentPage+5}</a></li>
	</c:if>
	<c:if test="${currentPage<pageNum-5}">
		<li><a class="chn" title="跳到第${currentPage+6}页" href="${navUrl}?curpage=${currentPage+5}${navUrlEnd}">${currentPage+6}</a></li>
	</c:if>
	<c:if test="${currentPage<pageNum-1}">
		<li><a class="chn" title="下一页" href="${navUrl}?curpage=${currentPage+1}${navUrlEnd}">下一页</a></li>
	</c:if>
</c:if>
<c:if test="${pageNum>6}">
	<c:if test="${currentPage>-1 && currentPage<pageNum-3}">
		<c:if test="${currentPage>2}">
			<li><a class="chn" title="第一页" href="${navUrl}?curpage=0${navUrlEnd}">第一页</a></li>
		</c:if>
		<c:if test="${currentPage>0}">
			<li><a class="chn" title="上一页" href="${navUrl}?curpage=${currentPage-1}${navUrlEnd}">上一页</a></li>
		</c:if>
		<c:if test="${currentPage>1}">
			<li><a class="chn" title="跳到第${currentPage-1}页" href="${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a></li>
		</c:if>
		<c:if test="${currentPage>0}">
			<li><a class="chn" title="跳到第${currentPage}页" href="${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a></li>
		</c:if>
		<li class="current"><a href="#nogo">${currentPage+1}</a></li>
		<li><a class="chn" title="跳到第${currentPage+2}页" href="${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a></li>
		<li><a class="chn" title="跳到第${currentPage+3}页" href="${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a></li>
		<c:if test="${currentPage<2}">
			<li><a class="chn" title="跳到第${currentPage+4}页" href="${navUrl}?curpage=${currentPage+3}${navUrlEnd}">${currentPage+4}</a></li>
		</c:if>
		<c:if test="${currentPage<1}">
			<li><a class="chn" title="跳到第${currentPage+5}页" href="${navUrl}?curpage=${currentPage+4}${navUrlEnd}">${currentPage+5}</a></li>
		</c:if>
		<li><a class="chn" title="下一页" href="${navUrl}?curpage=${currentPage+1}${navUrlEnd}">下一页</a></li>
		
		<li><a class="chn" title="最后页" href="${navUrl}?curpage=${pageNum-1}${navUrlEnd}">最后页</a></li>
	</c:if>
	<c:if test="${(currentPage < pageNum - 0)&&(currentPage>pageNum-4)}">
		<li><a class="chn" title="第一页" href="${navUrl}?curpage=0${navUrlEnd}">第一页</a></li>
		
		<li><a class="chn" title="上一页" href="${navUrl}?curpage=${currentPage-1}${navUrlEnd}">上一页</a></li>
		<c:if test="${currentPage>pageNum-2}">
			<li><a class="chn" title="跳到第${currentPage-3}页" href="${navUrl}?curpage=${currentPage-4}${navUrlEnd}">${currentPage-3}</a></li>
		</c:if>
		<c:if test="${currentPage>pageNum-3}">
			<li><a class="chn" title="跳到第${currentPage-2}页" href="${navUrl}?curpage=${currentPage-3}${navUrlEnd}">${currentPage-2}</a></li>
		</c:if>
		<li><a class="chn" title="跳到第${currentPage-1}页" href="${navUrl}?curpage=${currentPage-2}${navUrlEnd}">${currentPage-1}</a></li>
		<li><a class="chn" title="跳到第${currentPage}页" href="${navUrl}?curpage=${currentPage-1}${navUrlEnd}">${currentPage}</a></li>
		<li class="current"><a href="#nogo">${currentPage+1}</a></li>
		<c:if test="${currentPage<pageNum-1}">
			<li><a class="chn" title="跳到第${currentPage+2}页" href="${navUrl}?curpage=${currentPage+1}${navUrlEnd}">${currentPage+2}</a></li>
		</c:if>
		<c:if test="${currentPage<pageNum-2}">
			<li><a class="chn" title="跳到第${currentPage+3}页" href="${navUrl}?curpage=${currentPage+2}${navUrlEnd}">${currentPage+3}</a></li>
		</c:if>
		<c:if test="${currentPage<pageNum-1}">
			<li><a class="chn" title="下一页" href="${navUrl}?curpage=${currentPage+1}${navUrlEnd}">下一页</a></li>
		</c:if>
		<c:if test="${currentPage<pageNum-3}">
			<li><a title="最后页" href="${navUrl}?curpage=${pageNum-1}${navUrlEnd}">最后页</a></li>
		</c:if>
	</c:if>
</c:if>
</c:if>
</ol>
