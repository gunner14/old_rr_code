<%@ page pageEncoding="utf-8" %>
<c:if test="${requestScope.disUniversityDiary}">
<%
	if(System.currentTimeMillis()%2==0){
%>
	<script type="text/javascript">
		//<![CDATA[
		//图片滚动效果
		var indexScroll = {
			bigImageId:"bigImg",
			scrollHeight:119,
			tabId:"news-tab",
			selectedStyle:"selected",
			totalcount:4,
			autotimeintval:2222,
			objName:"indexScroll"
		}
		XN.dom.readyDo(
			function(){ScrollNews.init(indexScroll);}
		)
		// ]]>
	</script>
	<div class="side-item carousel">								
		<div class="slide-show">
			<div id="news-tab">
				<h1>小美的大学日记</h1>
				<ul>
					<li id="news-tab_3" class=""><a href="javascript:;">4</a></li>
					<li id="news-tab_2" class=""><a href="javascript:;">3</a></li>
					<li id="news-tab_1" class=""><a href="javascript:;">2</a></li>
					<li id="news-tab_0" class="selected"><a href="javascript:;">1</a></li>
				</ul>
			</div>
			<div class="slide-box">
				<div id="bigImg">
					<ul>
						<li>
							<a href="${applicationScope.urlGuide}/ajaxGuideSearchUniversitySchoolMate?nur=guide_wb_1" target="_blank">
								<img src="${applicationScope.urlStatic}/imgpro/guide/activit/ad_1.png">
							</a>
						</li>
						<li>
							<a target="_blank" href="${applicationScope.urlGuide}/ajaxGuideRedirectToUniversityPage?nur=guide_wb_2">
								<img src="${applicationScope.urlStatic}/imgpro/guide/activit/ad_2.png">
							</a>
						</li>
						<li>
							<a href="${applicationScope.urlGuide}/ajaxGuideSearchHighSchoolMate?nur=guide_wb_3" target="_blank">
								<img src="${applicationScope.urlStatic}/imgpro/guide/activit/ad_3.png">
							</a>
						</li>
						<li>
							<a href="${applicationScope.urlGuide}/ajaxGuideRedirectToHighschoolPage?nur=guide_wb_4" target="_blank">
								<img src="${applicationScope.urlStatic}/imgpro/guide/activit/ad_4.png">
							</a>
						</li>
					</ul>
				</div>
			</div>
		</div><!--slide-show end-->
	</div><!--side-item carousel end-->
<%
	}
	else{
%>
<div class="side-item carousel">								
	<div class="slide-show">
		<div id="app-gift">
		   <div class='giftHead'>
				<h4>新生活动礼包</h4>
				<a href='javascript:;' class='refresh'></a>
		   </div>
		   <div class='giftBody'>
				<ul>
				    <li><a href='${applicationScope.urlApps}/littlewar/newstudents.html' target='_blank'><img src='${applicationScope.urlStatic}/imgpro/app/gift/1.jpg' /></a></li>
					<li><a href='${applicationScope.urlApps}/paopaoyu/?origin=nur' target='_blank'><img src='${applicationScope.urlStatic}/imgpro/app/gift/2.jpg' /></a></li>
					<li><a href='${applicationScope.urlApps}/boyaa_pet/index.php?reqfrom=2000' target='_blank'><img src='${applicationScope.urlStatic}/imgpro/app/gift/3.png' /></a></li>
					<li><a href='${applicationScope.urlApps}/rrisland/?origin=nur' target='_blank'><img src='${applicationScope.urlStatic}/imgpro/app/gift/4.jpg' /></a></li>
					<li><a href='${applicationScope.urlApps}/wondergarden/v1/?origin=nur' target='_blank'><img src='${applicationScope.urlStatic}/imgpro/app/gift/5.jpg' /></a></li>
					<li><a href='${applicationScope.urlApps}/happyfarmtwo/newstudents.html' target='_blank'><img src='${applicationScope.urlStatic}/imgpro/app/gift/6.jpg' /></a></li>
				</ul>
		   </div>
		</div>
	</div><!--slide-show end-->
</div><!--side-item carousel end-->
<script type="text/javascript">
     appGiftAD();
</script>
<%
	}
%>
</c:if>

