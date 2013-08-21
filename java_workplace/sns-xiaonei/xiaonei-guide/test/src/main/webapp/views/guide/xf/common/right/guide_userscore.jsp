<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:if test="${!empty myLevel}">	
    <div class="side-item point" id="homeUserScore">		
        <c:if test="${showAwardCount}">
	        <ul class="point-hint">
	            <c:if test="${levelAwardCount >0 }">
	            <li>
	                <img height="16" width="16" src="${applicationScope.urlStatic}/imgpro/icons/upgrade.png" style="margin-right:3px"/><a href='${applicationScope.urlWww}/scores/award?levelcount=${levelAwardCount}<c:if test="${contineDaysAwardCount >0 }">&continuecount=${contineDaysAwardCount}</c:if>'><strong>${levelAwardCount}</strong>个等级升级奖励</a>
	            </li>
	            </c:if>
	            <c:if test="${contineDaysAwardCount >0 }">
	            <li>
	                <img height="16" width="16" src="${applicationScope.urlStatic}/imgpro/icons/loginmore.png" style="margin-right:3px"/><a href='${applicationScope.urlWww}/scores/award?continuecount=${contineDaysAwardCount}<c:if test="${levelAwardCount >0 }">&levelcount=${levelAwardCount}</c:if>'><strong>${contineDaysAwardCount}</strong>个连续登录奖励</a>
	            </li>
	            </c:if>
	        </ul>
        </c:if>
        <c:if test="${showTip && (showAwardCount||(loginCDays == 1)||(loginCDays == 3)||(loginCDays == 5)||(loginCDays == 7))}">
            <div id="scoreTipDiv" class="side-item-body">
                <div class="new-point-tips">
                    <div class="right-arrow"></div>
                    <div id="" class="tipsbody">
                        <c:choose>
                        <c:when test="${showAwardCount}">
                        <h3><img src="${applicationScope.urlStatic}/imgpro/icons/loginmore.png" width="16" height="16" alt="" />你有未领取的奖励！</h3>
                        <p>等级提升或者连续登录，<br />就有可能得到成长奖励。</p>
                        </c:when>
                        <c:when test="${loginCDays == 1}">
                        <h3><img src="${applicationScope.urlStatic}/imgpro/icons/bulb.png" width="12" height="16" alt="" /> 今日登录获得5分</h3>
                        <p>再连续登录6天，<br />每天登录就能获得7分。</p>
                        </c:when>
                        <c:when test="${loginCDays == 3}">
                        <h3><img src="${applicationScope.urlStatic}/imgpro/icons/red-clock.png" width="16" height="16" alt="" />加速升级中！</h3>
                        <p>再连续登录2天，<br />每天登录就能获得7分。</p>
                        </c:when>
                        <c:when test="${loginCDays == 5}">
                        <h3><img src="${applicationScope.urlStatic}/imgpro/icons/bulb.png" width="12" height="16" alt="" /> 今日登录获得7分</h3>
                        <p>再连续登录2天，<br />即可享受尊贵橙名特权。</p>
                        </c:when>
                        <c:when test="${loginCDays == 7}">
                        <h3><img src="${applicationScope.urlStatic}/imgpro/icons/name.png" width="16" height="16" alt="" />获得橙名特权！</h3>
                        <p>连续登录30天，<br />每次登录就能获得10分。</p>
                        </c:when>
                        </c:choose>
                    </div>
                    <a onclick="$('scoreTipDiv').hide()" href="javascript:;" class="x-to-hide"></a>
                </div>  
            </div>
            <script type="text/javascript">
                (function() {
                    var hasAward = ${showAwardCount? 'true' : 'false'};
                    var scroeTip = new XN.ui.fixPositionElement(
                    {
                        id: 'scoreTipDiv',
                        alignWith : 'homeUserScore',
                        alignType: '1-2',
                        tagName : 'div',
                        offsetX: -8,
                        offsetY: hasAward? 0 : -26
                    });
                    setTimeout(function(){$('scoreTipDiv').hide();},5000);
                })()
            </script>
        </c:if>
    </div>	
</c:if>
