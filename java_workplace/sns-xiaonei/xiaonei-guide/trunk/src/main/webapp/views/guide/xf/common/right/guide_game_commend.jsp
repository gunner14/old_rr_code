<%@ page pageEncoding="utf-8" %>
<%@ page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%@ page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<c:if test="${domainIdentified['siteNameAbbr']=='rr'}">
<%
try{               
    MemCacheManager mem = com.xiaonei.platform.core.cache.MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);  
    if(mem!=null){
	    if("first".equals(mem.get("arrive_guide_times"+user.getId()))){
	    	mem.set("arrive_guide_times"+user.getId(), "noFirst", (int)com.xiaonei.platform.core.cache.MemCacheKeys.hour*73);
%>
	<c:choose>
		<c:when test="${requestScope.appConnectId==44097}"><%--人人农场--%>
			<div class="side-item wide-sponsors">	
				<div class="side-item-header clearfix">
					<h4>推荐</h4>
					<div class="option">
						<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_RRNC" target="_blank">更多推荐</a>
					</div>
				</div>
				<div class="side-item-body clearfix">
					<div class="figure">     	
						<a target="_blank" href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_pic_RRNC" class="link"> 
							<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/activit/renren_farm_spread.png">
						</a>
					</div>
					<p class="guide-text">
						<a href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_words_RRNC" target="_blank">人人农场</a>
						&nbsp;送你1片地－麦子熟了，鸡也生了，瓜果蔬菜和小动物们等你来！
					</p>
				</div>
				<div class="footer" style="padding-bottom:10px;">								
					<a class="link" target="_blank" href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_button_RRNC">
						<img src="${applicationScope.urlStatic}/imgpro/guide/activit/enter_game_button.png">
					</a>
				</div>  
			</div>
		</c:when>
		<c:when test="${requestScope.appConnectId==53429}"><%--人人餐厅--%>			
			<div class="side-item wide-sponsors">	
				<div class="side-item-header clearfix">
					<h4>推荐</h4>
					<div class="option">
						<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_RRCT" target="_blank">更多推荐</a>
					</div>
				</div>
				<div class="side-item-body clearfix">
					<div class="figure">     	
						<a target="_blank" href="${applicationScope.urlApps}/rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_pic_RRCT" class="link"> 
							<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/activit/renren_dining_room.png">
						</a>
					</div>
					<p class="guide-text">
						<a href="${applicationScope.urlApps}/rr_rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_words_RRCT" target="_blank">人人餐厅</a>
						&nbsp;经营餐厅，学习菜谱，积累食材，装潢店面…变身餐饮业大亨不是梦！
					</p>						
				</div>
				<div class="footer" style="padding-bottom:10px;">								
					<a class="link" target="_blank" href="${applicationScope.urlApps}/rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_button_RRCT">
						<img src="${applicationScope.urlStatic}/imgpro/guide/activit/enter_game_button.png">
					</a>
				</div>  
				</div>			
		</c:when>
		<c:when test="${visiter.stage==30 || visiter.stage==90}">
<%
			int firstGamesCount=3; 
       		switch(new java.util.Random().nextInt(firstGamesCount)){
           		case 0:
%>         
			<div class="side-item wide-sponsors">	
				<div class="side-item-header clearfix">
					<h4>推荐</h4>
					<div class="option">
						<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_RRNC" target="_blank">更多推荐</a>
					</div>
				</div>
				<div class="side-item-body clearfix">
					<div class="figure">     	
						<a target="_blank" href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_pic_RRNC" class="link"> 
							<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/activit/renren_farm_spread.png">
						</a>
					</div>
					<p class="guide-text">
						<a href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_words_RRNC" target="_blank">人人农场</a>
						&nbsp;送你1片地－麦子熟了，鸡也生了，瓜果蔬菜和小动物们等你来！
					</p>
					
				</div>
				<div class="footer" style="padding-bottom:10px;">								
					<a class="link" target="_blank" href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_button_RRNC">
						<img src="${applicationScope.urlStatic}/imgpro/guide/activit/enter_game_button.png">
					</a>
				</div>  
			</div>			
<%  
	              break;
	           case 1:
%>		
			<div class="side-item wide-sponsors">	
				<div class="side-item-header clearfix">
					<h4>推荐</h4>
					<div class="option">
						<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_RRCT" target="_blank">更多推荐</a>
					</div>
				</div>
				<div class="side-item-body clearfix">
					<div class="figure">     	
						<a target="_blank" href="${applicationScope.urlApps}/rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_pic_RRCT" class="link"> 
							<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/activit/renren_dining_room.png">
						</a>
					</div>
					<p class="guide-text">
						<a href="${applicationScope.urlApps}/rr_rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_words_RRCT" target="_blank">人人餐厅</a>
						&nbsp;经营餐厅，学习菜谱，积累食材，装潢店面…变身餐饮业大亨不是梦！
					</p>					
				</div>
				<div class="footer" style="padding-bottom:10px;">								
					<a class="link" target="_blank" href="${applicationScope.urlApps}/rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_button_RRCT">
						<img src="${applicationScope.urlStatic}/imgpro/guide/activit/enter_game_button.png">
					</a>
				</div>  
			</div>
<%  
	              break;
	           case 2:
%>
			<div class="side-item wide-sponsors">	
				<div class="side-item-header clearfix">
					<h4>推荐</h4>
					<div class="option">
						<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_YXDT" target="_blank">更多推荐</a>
					</div>
				</div>
				<div class="side-item-body clearfix">
					<div class="figure">     	
						<a target="_blank" href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_pic_YXDT" class="link"> 
							<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/activit/games_hall_spead.png">
						</a>
					</div>
					<p class="guide-text">
						<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_words_YXDT" target="_blank">游戏大厅</a>
						&nbsp;人人网各类游戏集中营，种菜养鱼、开发岛屿、上千种小游戏……
					</p>					
				</div>
				<div class="footer" style="padding-bottom:10px;">								
					<a class="link" target="_blank" href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_button_YXDT">
						<img src="${applicationScope.urlStatic}/imgpro/guide/activit/enter_game_button.png">
					</a>
				</div>  
			</div>
<%  
	              break;
	           default: 
	              break; 
	       } 
%>

		</c:when>
	</c:choose>
<%
    	} 
	    else if(user.getStage()==30 || user.getStage()==90 || "44097".equals(request.getAttribute("appConnectId").toString())|| "53429".equals(request.getAttribute("appConnectId").toString())){
	        int gamesSpreadCount=3;  
	       	switch(new java.util.Random().nextInt(gamesSpreadCount)){
	           case 0:
%> 
	<div class="side-item wide-sponsors">	
		<div class="side-item-header clearfix">
			<h4>推荐</h4>
			<div class="option">
				<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_RRNC" target="_blank">更多推荐</a>
			</div>
		</div>
		<div class="side-item-body clearfix">
			<div class="figure">     	
				<a target="_blank" href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_pic_RRNC" class="link"> 
					<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/activit/renren_farm_spread.png">
				</a>
			</div>
			<p class="guide-text">
				<a href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_words_RRNC" target="_blank">人人农场</a>
				&nbsp;送你1片地－麦子熟了，鸡也生了，瓜果蔬菜和小动物们等你来！
			</p>
			
		</div>
		<div class="footer" style="padding-bottom:10px;">								
			<a class="link" target="_blank" href="${applicationScope.urlApps}/rr_farm?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_button_RRNC">
				<img src="${applicationScope.urlStatic}/imgpro/guide/activit/enter_game_button.png">
			</a>
		</div>  
	</div>	
<%  
	              break;
	           case 1:
%>
	<div class="side-item wide-sponsors">	
		<div class="side-item-header clearfix">
			<h4>推荐</h4>
			<div class="option">
				<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_RRCT" target="_blank">更多推荐</a>
			</div>
		</div>
		<div class="side-item-body clearfix">
			<div class="figure">     	
				<a target="_blank" href="${applicationScope.urlApps}/rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_pic_RRCT" class="link"> 
					<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/activit/renren_dining_room.png">
				</a>
			</div>
			<p class="guide-text">
				<a href="${applicationScope.urlApps}/rr_rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_words_RRCT" target="_blank">人人餐厅</a>
				&nbsp;经营餐厅，学习菜谱，积累食材，装潢店面…变身餐饮业大亨不是梦！
			</p>			
		</div>
		<div class="footer" style="padding-bottom:10px;">								
			<a class="link" target="_blank" href="${applicationScope.urlApps}/rr_cook?pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_button_RRCT">
				<img src="${applicationScope.urlStatic}/imgpro/guide/activit/enter_game_button.png">
			</a>
		</div>  
	</div>
<%  
	              break;
	           case 2:
%>
	<div class="side-item wide-sponsors">	
		<div class="side-item-header clearfix">
			<h4>推荐</h4>
			<div class="option">
				<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_YXDT" target="_blank">更多推荐</a>
			</div>
		</div>
		<div class="side-item-body clearfix">
			<div class="figure">     	
				<a target="_blank" href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_pic_YXDT" class="link"> 
					<img width="220" height="120" src="${applicationScope.urlStatic}/imgpro/guide/activit/games_hall_spead.png">
				</a>
			</div>
			<p class="guide-text">
				<a href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_words_YXDT" target="_blank">游戏大厅</a>
				&nbsp;人人网各类游戏集中营，种菜养鱼、开发岛屿、上千种小游戏……
			</p>			 
		</div>
		<div class="footer" style="padding-bottom:10px;">								
			<a class="link" target="_blank" href="${applicationScope.urlApp}/app/apps/list?selected=2&pma=p_guide_x5_m_pub_tips_leftmenu_share_a_click_button_YXDT">
				<img src="${applicationScope.urlStatic}/imgpro/guide/activit/enter_game_button.png">
			</a>
		</div> 
	</div>	
<%         
	              break;
	           default:
	              break; 
	       }
	    }
    }   		
}
catch(Exception e){
    e.printStackTrace();
} 
%>	
</c:if>