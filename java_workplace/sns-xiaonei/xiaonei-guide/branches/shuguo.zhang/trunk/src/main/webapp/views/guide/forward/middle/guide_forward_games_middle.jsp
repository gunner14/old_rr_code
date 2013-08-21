<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<div class="guide-game">
	<h2>人人网热门游戏</h2>
	<div class="slide clearfix">
		<div class="pic" id="show">
			<a href="${applicationScope.urlApps}/rr_cook?ref=lp_rrct" style="display: none;" target="_blank" >
				<img src="${applicationScope.urlStatic}/imgpro/guide/guide-rrcook.jpg" width="650px" height="399px" />
			</a> 
			<a href="${applicationScope.urlApps}/paopaoyu?ref=lp_ppy" target="_blank" >
				<img src="${applicationScope.urlStatic}/imgpro/guide/guide-ppy.jpg" width="650px"	height="399px" />
			</a> 
			<a href="${applicationScope.urlApps}/happyfarm?ref=lp_kxnc" style="display: none;" target="_blank" >
				<img src="${applicationScope.urlStatic}/imgpro/guide/gudie-kxfram.jpg" width="650px" height="399px" />
			</a>
		</div>
		<div class="preview" id="preview">
			<ul>
				<li><a href="${applicationScope.urlApps}/rr_cook?ref=lp_rrct" target="_blank" class="rrcook"><span>人人餐厅</span></a></li>
				<li><a href="${applicationScope.urlApps}/paopaoyu?ref=lp_ppy" target="_blank" class="ppy current"><span>泡泡鱼</span></a></li>
				<li><a href="${applicationScope.urlApps}/happyfarm?ref=lp_kxnc" target="_blank" class="kxfarm"><span>开心农场</span></a></li>
			</ul>
		</div>
	</div>
	<script type="text/javascript">
		XN.DOM.readyDo(function(){
			var oPreviews = $("preview").getElementsByTagName("a");
			var oShows = $("show").getElementsByTagName("a");			
			function reset(){
				for(var i = 0 ; i< oShows.length; i++){
					oShows[i].style.display="none";
					XN.element.delClass(oPreviews[i],"current");
				}
			}
			var cur=0,si1,t=2500;
			function slide(){
				si1 = setTimeout(function(){
					reset();
					oShows[cur].style.display="block";
					XN.element.addClass(oPreviews[cur],"current");
                    cur = cur < oShows.length-1 ? cur + 1 : 0;
					slide();
				},t)
			}
			for(var i = 0; i < oShows.length ; i++){
				(function(i){
					oShows[i].onmouseover = oPreviews[i].onmouseover = function(){
						reset();
						clearInterval(si1);
						oShows[i].style.display="block";
						XN.element.addClass(oPreviews[i],"current");
					}
					oShows[i].onmouseout = oPreviews[i].onmouseout = function(){
						cur = i;
						//slide();
					}
				})(i)
			}
			//slide();
		})		
	</script>	
	<%@ include file="/views/guide/forward/middle/guide_commend_game.jsp"%>
</div>
