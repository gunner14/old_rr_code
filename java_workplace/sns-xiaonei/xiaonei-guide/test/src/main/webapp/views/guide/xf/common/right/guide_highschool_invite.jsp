<%@ page pageEncoding="utf-8" %>
<c:if test="${domainIdentified['sys.domain']=='renren.com' && visiter.stage==10 && requestScope.disGuideHighschoolInvite && visiter.domain=='renren.com'}">	
	<div class="side-item" id="highschoolSummer">
		<div class="side-item-body">
			<div class="highschool-invite-reg clearfix" style="position: relative; width: 220px;">
			<%--
				<a href="${applicationScope.urlInvite}/inviteHS?isf=rqf" class="act-icon float-left" target="_blank">
					<img src="${applicationScope.urlStatic}/n/apps/home/res/activity/vip-to-u.png" width="50" height="50" style="display:block;" />
				</a>
				<div class="act-content" style="margin-left:60px;position:relative;width:160px;_width:157px;">
					<h4 style="margin-bottom:8px;">VIP暑期大放送</h4>
					<p><a href="${applicationScope.urlInvite}/inviteHS?isf=rqf" target="_blank">邀请好友获取VIP奖励</a></p>
					<a href="javascript:;" class="x-to-hide" onclick="if($('highschoolSummer'))$('highschoolSummer').hide();ajax_set_mem('homeRightInvite${visiter.id}','21');" style="position:absolute;top:0;right:0;"></a>
				</div>
			--%>
				<a href="${applicationScope.urlInvite}/inviteHS?isf=rqf" target="_blank">
					<img src="${applicationScope.urlStatic}/n/apps/home/res/activity/vip-to-u-big.png" />
				</a>
				<a href="javascript:;" class="x-to-hide" onclick="if($('highschoolSummer'))$('highschoolSummer').hide();ajax_set_mem('guideHighschoolInvite_${visiter.id}','21');" style="position:absolute;top:2px;right:2px;"></a>
			</div>
		</div>
	</div>	
</c:if>
