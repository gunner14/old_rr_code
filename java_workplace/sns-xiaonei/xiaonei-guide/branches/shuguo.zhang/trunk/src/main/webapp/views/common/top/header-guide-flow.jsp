<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>
<div id="navBar" class="menu-bar ${domainIdentified['siteNameAbbr']}">
	<div class="navigation-wrapper">
		<div class="navigation clearfix">
			<c:choose>					
					<c:when test="${INC_HEADER_ISVIP==1}">
						<logo:logotag mode="vip"/>
					</c:when>
					<c:otherwise>
						<c:choose>
							<c:when test="${channel=='首页'}">
								<logo:logotag mode="home"/>
							</c:when>
							<c:otherwise>
								<logo:logotag mode="trunk"/>
							</c:otherwise>
						</c:choose>						
					</c:otherwise>
				</c:choose>	
            <div class="nav-body clearfix">
                <div class="nav-other">
                    <div class="menu">
                        <div class="menu-title last">
                            <a href="http://www.${applicationScope.sysdomain}/Logout.do">退出</a>
                        </div>
                    </div>
                </div>
            </div>
    	</div>
    </div>
</div>