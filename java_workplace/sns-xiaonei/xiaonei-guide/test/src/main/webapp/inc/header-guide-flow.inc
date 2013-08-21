<!-- add -->
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
					<div id="navMessage" style="display:none">
						<span class="remind " title="提醒">
							<a href="#nogo" onclick="return false;" hidefocus="true">&nbsp;</a>
							<i style="display: none; "><u><b>&nbsp;</b><var>0</var></u><em>&nbsp;</em></i>
						</span>
						<span class="apply" title="请求">
							<a href="#nogo" onclick="return false;" hidefocus="true">&nbsp;</a>
							<i style="display: none; "><u><b class="apply">&nbsp;</b><var>0</var></u><em>&nbsp;</em></i>
						</span>
						<span class="notice" title="通知">
							<a href="#nogo" onclick="return false;" hidefocus="true">&nbsp;</a>
							<i style="display: none; "><u><b class="notice">&nbsp;</b><var>0</var></u><em>&nbsp;</em></i>
						</span>
						<div id="showMessage" style="display:none;">
							<div id="loadTip">&nbsp;</div>
							<iframe name="showMessageIframe" id="showMessageIframe" src="about:blank" frameborder="0" allowtransparency="true" scrolling="no" width="294" height="0"></iframe>
						</div>
					</div>
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