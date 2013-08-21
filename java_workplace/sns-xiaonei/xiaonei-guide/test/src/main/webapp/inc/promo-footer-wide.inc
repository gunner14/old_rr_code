<%@ page pageEncoding="UTF-8" %>
<c:if test="${empty newHome}">
<c:choose>
        <%--<c:when test="${applicationScope.domain == 'kaixin.com'}">
		<c:choose>
			<c:when test="${channel=='首页'}">
				<div id="bannerBottom" class="banner clearfix">
					<div class="float-left">
						<!--AdMop Begin:-->
						<div id="ad1000010011"></div>
						<!--AdMop End-->
					</div>
					<div class="float-right">
						<!--AdMop Begin:-->
						<div id="ad1000010012"></div>
						<!--AdMop End-->
					</div>
				</div>
			</c:when>
			<c:when test="${channel!='space' && channel!='app'}" >
				<div id="bannerBottom" class="banner clearfix">
					<div class="float-left">
						<!--AdMop Begin:-->
						<div id="ad1000010024"></div>
						<!--AdMop End-->
					</div>
					<div class="float-right">
						<!--AdMop Begin:-->
						<div id="ad1000010025"></div>
						<!--AdMop End-->
					</div>
				</div>
			</c:when>
		</c:choose>
        </c:when>--%>
        <c:when test="${applicationScope.domain == 'renren.com'}">
		<c:choose>
			<c:when test="${channel=='首页'}">
				<div id="bannerBottom" class="banner clearfix">
					<div class="float-left">
						<!--AdMop Begin:-->
						<div id="ad1000000041"></div>
						<!--AdMop End-->
					</div>
					<div class="float-right">
						<!--AdMop Begin:-->
						<div id="ad1000000042"></div>
						<!--AdMop End-->
					</div>
				</div>
			</c:when>
			<c:when test="${channel!='space' && channel!='app'}" >
				<div id="bannerBottom" class="banner clearfix">
					<div class="float-left">
						<!--AdMop Begin:-->
						<div id="ad1000000007"></div>
						<!--AdMop End-->
					</div>
					<div class="float-right">
						<!--AdMop Begin:-->
						<div id="ad1000000008"></div>
						<!--AdMop End-->
					</div>
				</div>
			</c:when>
		</c:choose>
        </c:when>
</c:choose>
</c:if>



