<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt"%>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xn" %>
<%@ page import="com.xiaonei.platform.component.tools.UserUtil" %>
<%
com.xiaonei.platform.core.model.User user = null;
%>
<%
try {
	user = (com.xiaonei.platform.core.model.User)request.getAttribute("host");	
} catch(Exception e) {
}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>	    
	  	<link href="${applicationScope.urlSmallStatic}/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-kx.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />        
        <c:set var="INC_COMMON_CSS" value="true" />
        <%@ include file="/inc/headpro.inc"%>	  	
	  	<title>${domainIdentified["siteName_fillinfo"]} - ${requestScope.host.name}</title>
	</head>
	<body id="dashboardPage" class="home" g="999_999">
		<c:set var="cleanpage" value="ture" /> 		
		<%@ include file="/inc/header-guide-flow.inc" %>
		<%@ include	file="/inc/wrapper.inc"%>
		<div class="guide-game">
			<h2>请选择你最感兴趣的游戏开始玩：</h2>
			<div class="guide-box">
				<div class="slide clearfix">
					<div class="pic" id="show">
						<a href="${applicationScope.urlApps}/finalfight/?ref=kaixin_app_guide" style="display:none;">
							<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/1.jpg" width="600px" height="360px" />
						</a>
						<a href="${applicationScope.urlApps}/soletower/?ref=kaixin_app_guide">
							<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/2.jpg" width="600px" height="360px" />
						</a>
						<a href="http://gamestat.${applicationScope.sysdomain}/game/294" style="display:none;">
							<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/9.jpg" width="600px" height="360px" />
						</a>
						<a href="${applicationScope.urlApps}/rr_cook/?ref=kaixin_app_guide" style="display:none;">
							<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/4.jpg" width="600px" height="360px" />
						</a>
						<a href="${applicationScope.urlApps}/rr_farm/?ref=kaixin_app_guide" style="display:none;">
							<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/5.jpg" width="600px" height="360px" />
						</a>
						<a href="${applicationScope.urlApps}/ronhospital/?ref=kaixin_app_guide" style="display:none;">
							<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/6.jpg" width="600px" height="360px" />
						</a>
						<a href="${applicationScope.urlApps}/muchang/?ref=kaixin_app_guide" style="display:none;">
							<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/7.jpg" width="600px" height="360px" />
						</a>
						<a href="${applicationScope.urlApps}/cutepig/?ref=kaixin_app_guide" style="display:none;">
							<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/8.jpg" width="600px" height="360px" />
						</a>
					</div>
					<div class="preview" id="pre_con">
						<a id="pre_show" href="javascript:;" class="pgup"></a>
						<ul id="previews">
							<li dis="0"><a href="${applicationScope.urlApps}/finalfight/?ref=kaixin_app_guide">
								<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/1s.jpg" width="92px" height="70px" />
							</a></li>
							<li dis="1" class="current"><a href="${applicationScope.urlApps}/soletower/?ref=kaixin_app_guide">
								<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/2s.jpg" width="92px" height="70px" />
							</a></li>
							<li dis="2"><a href="http://gamestat.${applicationScope.sysdomain}/game/294">
								<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/9s.jpg" width="92px" height="70px" />
							</a></li>
							<li dis="3"><a href="${applicationScope.urlApps}/rr_cook/?ref=kaixin_app_guide">
								<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/4s.jpg" width="92px" height="70px" />
							</a></li>
							<li dis="4"><a href="${applicationScope.urlApps}/rr_farm/?ref=kaixin_app_guide">
								<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/5s.jpg" width="92px" height="70px" />
							</a></li>
							<li dis="5"><a href="${applicationScope.urlApps}/ronhospital/?ref=kaixin_app_guide">
								<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/6s.jpg" width="92px" height="70px" />
							</a></li>
							<li dis="6"><a href="${applicationScope.urlApps}/muchang/?ref=kaixin_app_guide">
								<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/7s.jpg" width="92px" height="70px" />
							</a></li>
							<li dis="7"><a href="${applicationScope.urlApps}/cutepig/?ref=kaixin_app_guide">
								<img src="${applicationScope.urlStatic}/imgpro/guide/game-kx/8s.jpg" width="92px" height="70px" />
							</a></li>
						</ul>
						<a id="next_show" href="javascript:;" class="pgdn"></a>
					</div>
				</div>
			</div><!--guide-box end-->
			<p>有 <strong>${requestScope.playCount}</strong> 用户正在开心网玩这些游戏哦 
			<strong><a href="${applicationScope.urlApp}/app/apps/list?origin=119&ref=kaixin_app_guide">去游戏中心看看</a></strong>
			<a href="/guidexf.do?ref=guide_later_play" class="skip">以后再玩&raquo;</a></p>
		</div><!--guide-game end-->
		<script type="text/javascript">
			XN.dom.ready(function(){
				var _as = XN.array.build( $('previews').getElementsByTagName('li') );
                var _ss = XN.array.build( $('show').getElementsByTagName('a') );
                var default_focus = 1;
                function show(el){
					while( el.tagName.toLowerCase() !== 'li' ){
						el = el.parentNode;
                    }
					XN.array.each($('previews').getElementsByTagName('li'), function(i, v){
						$(v).delClass('current'); 
                        var index = parseInt(v.getAttribute('dis'));
                        $(_ss[index]).hide();
                        if ( v === el ) default_focus = i;
                    }); 
					$(el).addClass('current');
					var index = parseInt(el.getAttribute('dis'));
					$(_ss[index]).show();
				}
				function pre(){
					var lis = $('previews').getElementsByTagName('li');
					$('previews').appendChild(lis[0]); 
					show($('previews').getElementsByTagName('li')[default_focus]); 
				}
				XN.event.addEvent(_as, 'mouseover', function(e){
					var _ce = XN.event.element(e || window.event);
					show(_ce);
				});
                XN.event.addEvent('pre_show', 'click', pre);
				XN.event.addEvent('next_show', 'click', function(){
					var lis = $('previews').getElementsByTagName('li');
					$('previews').insertBefore(lis[lis.length - 1], lis[0]);
					show($('previews').getElementsByTagName('li')[default_focus]); 
				});
                XN.event.addEvent('pre_con', 'mouseover', function(){
					clearTimer();  
				});
				XN.event.addEvent('pre_con', 'mouseout', function(){
					startTimer();  
				});
				var timer = null;
				function clearTimer(){
					if ( !timer ) return;
					clearInterval(timer);
					timer = null;
				}
				function startTimer(){
					if (timer) return;
					timer = setInterval(pre, 2600);
				}
                startTimer();
			});
		</script>
		<%@ include file="/inc/footer_guide.inc"%>				
	</body>
</html>
