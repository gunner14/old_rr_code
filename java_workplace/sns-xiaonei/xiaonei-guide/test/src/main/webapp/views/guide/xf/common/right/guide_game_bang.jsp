<%@ page pageEncoding="utf-8" %>
<c:if test="${requestScope.guide_social=='social_bad'}">
	<div class="game-bang">
	    <h4>游戏榜</h4>
	    <div class="bang-nav-tabs">
	        <ul class="clearfix">
	            <li class="selected" id="guideapptab1">
	                <a onfocus="this.blur()" href="javascript:;" >最流行</a>
	            </li>
	            <li id="guideapptab2">
	            	<a onfocus="this.blur()" href="javascript:;" >最活跃</a>
	            </li>
	         </ul>
	    </div>
		<div class="game-lists" id="appListTabCon"></div>
		<div class="game-more">
			<a href="${applicationScope.urlApp}/app/apps/list?selected=2&amp;type=0&amp;category=0&amp;origin=2801" id="appshowmore" target="_blank">
				更多»
			</a>
		</div>
	</div>
</c:if>
