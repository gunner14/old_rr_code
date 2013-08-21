<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${applicationScope.domain == 'kaixin.com'}">
		<c:choose>
			<c:when test="${channel=='首页'}">
				<div id="ad1000010003" class="optimus"></div>
			</c:when>
			<c:when test="${channel!='space'}">
				<!--AdMop Begin:-->
				<div id="ad1000010019" class="optimus"></div>
				<div id="adTextLink1" class="block">
					<ul class="adtextlink">
						<li id="ad1000010020"></li>
						<li id="ad1000010021"></li>
						<li id="ad1000010022"></li>
					</ul>
				</div>
				<div id="ad1000010023" class="optimus"></div>
				<div id="adTextLink2" class="block">
					<ul class="adtextlink">
						<li id="ad1000010026"></li>
						<li id="ad1000010027"></li>
						<li id="ad1000010028"></li>
					</ul>
				</div>
				<!--AdMop End-->
				</c:when>
		</c:choose>
    </c:when>
    <c:when test="${applicationScope.domain == 'renren.com'}">
		<c:choose>
			<c:when test="${channel=='首页'}">
				<div id="ad1000000039" class="optimus"></div>
				<c:if test="${empty newHome}">
				<div id="adTextLink1" class="block">
					<ul class="adtextlink">
						<li id="ad1000000015"></li>
						<li id="ad1000000016"></li>
						<li id="ad1000000017"></li>
						<li id="ad1000000018"></li>
						<li id="ad1000000019"></li>
						<li id="ad1000000048"></li>
						<li id="ad1000000049"></li>
						<li id="ad1000000050"></li>
					</ul>
				</div>
					<div id="ad1000000040" class="optimus"></div>
				</c:if>
			</c:when>
			<c:when test="${channel!='space'}">
				<!--AdMop Begin:-->
				<div id="ad1000000005" class="optimus"></div>
				<div id="adTextLink1" class="block">
					<ul class="adtextlink">
						<li id="ad1000000015"></li>
						<li id="ad1000000016"></li>
						<li id="ad1000000017"></li>
						<li id="ad1000000018"></li>
						<li id="ad1000000019"></li>
						<li id="ad1000000026"></li>
						<li id="ad1000000027"></li>
						<li id="ad1000000028"></li>
					</ul>
				</div>
				<!--AdMop End-->
			</c:when>
		</c:choose>
    </c:when>
</c:choose>


