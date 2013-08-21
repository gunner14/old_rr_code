<c:if test="${not fn:contains(PAGE_MODE, 'notopad, ') and visiter.userStateInfo.guideDone}">
	<c:set var="ad1">
		<c:choose>
			<c:when test="${!KAIXIN}">
				<c:choose>
					<c:when test="${visiter.stage == 10 && (channel=='日志'||channel=='日志终端')}">ad1000000051</c:when>
                    <c:when test="${visiter.stage == 10 && channel=='首页'}">ad100000000066</c:when>
					<c:otherwise>
						<c:choose>
							<c:when test="${channel=='首页'}">ad100000000037</c:when>
							<c:when test="${channel!='space'}">ad1000000003</c:when>
						</c:choose>
					</c:otherwise>
				</c:choose>
			</c:when>
			<c:when test="${KAIXIN}">
				<c:if test="${channel=='首页'}">ad1000010001</c:if>
			</c:when>
		</c:choose>
	</c:set>
	<c:set var="ad2">
		<c:choose>
			<c:when test="${!KAIXIN}">
				<c:choose>
					<c:when test="${visiter.stage == 10 && (channel=='日志'||channel=='日志终端')}">ad1000000052</c:when>
					<c:otherwise>
						<c:choose>
							<c:when test="${channel=='首页'}">ad1000000038</c:when>
							<c:when test="${channel!='space'}">ad1000000004</c:when>
						</c:choose>
					</c:otherwise>
				</c:choose>
			</c:when>
			<c:when test="${KAIXIN}">
				<c:if test="${channel=='首页'}">ad1000010002</c:if>
			</c:when>
		</c:choose>
	</c:set>
	
	<c:choose>
		<c:when test="${channel=='首页'}">
			<div id="header-wide-banner" class="wide-banner" style="width:958px;overflow:hidden;">
				<div class="single-box">
					<div id="${ad1}" style="height:90px;overflow:hidden;"></div>
				</div>
			</div>		
		</c:when>
		<c:otherwise>
			<div id="header-wide-banner" class="wide-banner">
				<div class="box">
					<div id="${ad1}" style="height:90px;overflow:hidden;"></div>
				</div>
				<div class="box">
					<div id="${ad2}" style="height:90px;overflow:hidden;"></div>
				</div>
			</div>
		</c:otherwise>
	</c:choose>
	
</c:if>

