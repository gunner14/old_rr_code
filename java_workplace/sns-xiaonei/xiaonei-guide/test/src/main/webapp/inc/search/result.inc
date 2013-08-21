<c:choose>
	<c:when test="${item.type == 0}">
		<c:set var="name" value="姓名"/>
		<c:set var="headclass" value="Userimg"/>
		<c:set var="link" value="${applicationScope.urlMain}/profile.do?id=${item.id}&from=${offset + status.index}"/>
	</c:when>
	<c:when test="${item.type == 3}">
		<c:set var="name" value="应用"/>
		<c:set var="headclass" value="Appimg"/>
		<c:set var="link" value="${applicationScope.urlApp}/apps/application.do?id=${item.id}"/>
	</c:when>
	<c:when test="${item.type == 2}">
		<c:set var="name" value="公共主页"/>
		<c:set var="headclass" value="Starimg"/>
		<c:set var="link" value="${applicationScope.urlPage}/${item.id}?ref=search"/>
		<c:if test="${item.ptype==20}">
			<c:set var="link" value="${applicationScope.urlOrg}/${item.id}"/>
			<c:set var="name" value="俱乐部"/>
		</c:if>
	</c:when>
</c:choose>
<%
	com.xiaonei.result.ISearchResult nameSuffix = (com.xiaonei.result.ISearchResult)pageContext.getAttribute("item");
	StringBuilder suffix = new StringBuilder();
	StringBuilder vip = new StringBuilder();
	if((nameSuffix.getNameSuffix() & com.xiaonei.result.ISearchResult.NAME_SUFFIX_STAR) > 0){
		suffix.append("<a href=\""+com.xiaonei.platform.core.opt.OpiConstants.urlProfile+"/info/Help.do#help1_15\"><img alt=\"星级\" src=\""+com.xiaonei.platform.core.opt.OpiConstants.urlSmallStatic+"/img/star.gif\"/></a>");
	}
	if(nameSuffix.isVip()){
		vip.append("<a href=\""+com.xiaonei.platform.core.opt.OpiConstants.urlI+"/index.action?wc=120000\" target=\"_blank\"><img alt=\"VIP\" src=\""+com.xiaonei.platform.core.opt.OpiConstants.urlSmallStatic+"/imgpro/icons/vip_m.gif\"/></a>");
	}
	request.setAttribute("vip",vip.toString());
	request.setAttribute("suffix",suffix.toString());
%>


<li>
	<p class="avatar ${headclass}">
		<a target="_blank" href="${link}">
			<img src="${item.headImg}" alt="${item.headImg}"/>
		</a>
	</p>
	<div class="info">
		<dl>
			<dt>${name}:</dt>
			<dd>
				<strong><a href="${link}" target="_blank">${item.name}</a></strong>
				<span>${requestScope.suffix}</span>
				<span>${requestScope.vip}</span>
			</dd>

			<c:forEach items="${item.subTitle}" var="entry">
				<dt>${entry.key}:</dt>
				<%
				Object tmpItem = pageContext.getAttribute("entry.value");
				if(tmpItem instanceof java.lang.String){%>
					<dd>${entry.value}</dd>
				<%}else{%>
					<dd>
					<c:forEach items="${entry.value}" var="entryitem" varStatus="status">
						${entryitem}
						<c:if test="${! status.last}">
							<br />
						</c:if>
					</c:forEach>
					</dd>
				<%}%>
			</c:forEach>
		</dl>
	</div>
	<ul class="actions">
	<c:choose>
		<c:when test="${item.type == 0}">
			<%@ include file="/inc/search/useraction.inc" %>
		</c:when>
		<c:when test="${item.type == 3}">
			<%@ include file="/inc/search/appaction.inc" %>
		</c:when>
		<c:when test="${item.type == 2}">
			<%@ include file="/inc/search/pageaction.inc" %>
		</c:when>
	</c:choose>
	</ul>
</li>
