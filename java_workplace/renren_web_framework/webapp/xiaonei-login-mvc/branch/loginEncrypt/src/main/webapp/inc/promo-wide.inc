<%@ page pageEncoding="UTF-8" %>
<c:set var="adsid" value="538" />
<c:choose>
	<c:when test="${applicationScope.domain == 'kaixin.com'}">
		<c:choose>
			<c:when test="${channel=='首页'}">
				<div class="blank-holder">
					<div id="banner" class="banner clearfix">
						<div class="float-left">
							<!--AdMop Begin:-->
							<div id="ad1000010001"></div>
							<!--AdMop End-->
						</div>
						<div class="float-right">
							<!--AdMop Begin:-->
							<div id="ad1000010002"></div><!--AdMop End-->
						</div>
					</div>
				</div>
			</c:when>
			<%--<c:when test="${channel!='space'}" >
				<div class="blank-holder">
					<div id="banner" class="banner clearfix">
						<div class="float-left">
							<!--AdMop Begin:-->
							<div id="ad1000010017"></div>
							<!--AdMop End-->
						</div>
						<div class="float-right">
							<!--AdMop Begin:-->
							<div id="ad1000010018"></div><!--AdMop End-->
						</div>
					</div>
				</div>	
			</c:when>--%>
		</c:choose>
        </c:when>
        <c:when test="${applicationScope.domain == 'renren.com'}">
	<c:choose>
		<c:when test="${channel=='首页'}">
			<div class="blank-holder">
				<div id="banner" class="banner clearfix">
					<div class="float-left">
						<!--AdMop Begin:-->
						<div id="ad1000000037"></div>
						<!--AdMop End-->
					</div>
					<div class="float-right">
						<!--AdMop Begin:-->
						<div id="ad1000000038"></div><!--AdMop End-->
					</div>
				</div>
			</div>
		</c:when>
		<c:when test="${channel!='space'}" >
			<div class="blank-holder">
				<div id="banner" class="banner clearfix">
					<div class="float-left">
						<!--AdMop Begin:-->
						<div id="ad1000000003"></div>
						<!--AdMop End-->
					</div>
					<div class="float-right">
						<!--AdMop Begin:-->
						<div id="ad1000000004"></div><!--AdMop End-->
					</div>
				</div>
			</div>	
		</c:when>
	</c:choose>
	</c:when>
</c:choose>


