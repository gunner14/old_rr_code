<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<%--任务是否都完成--%>
<c:set var="done" value="${loginDay  >= 3 && friendCount >= 4  && feedCount >= 1}" />

<%--添加好友状态--%>
<c:choose>
	<c:when test="${friendCount == 0}">
		<c:set var="task_addfriend_st" value="task_undone" />
	</c:when>
	<c:when test="${friendCount > 0 && friendCount < 4}">
		<c:set var="task_addfriend_st" value="task_doing" />
	</c:when>
	<c:otherwise>
		<c:set var="task_addfriend_st" value="task_done" />
	</c:otherwise>
</c:choose>

<%--连续登陆状态--%>
<c:choose>
	<c:when test="${loginDay > 0 && loginDay < 3}">
		<c:set var="task_login_st" value="task_doing" />
	</c:when>
	<c:otherwise>
		<c:set var="task_login_st" value="task_done" />
	</c:otherwise>
</c:choose>

<%--向好友晒生活--%>
<c:choose>
	<c:when test="${feedCount <= 0}">
		<c:set var="task_feed_st" value="task_undone" />
	</c:when>
	<c:otherwise>
		<c:set var="task_feed_st" value="task_done" />
	</c:otherwise>
</c:choose>


<c:if test="${newGuideStep}">
<style type="text/css">
.guide2home{ width: 238px; min-height: 100px; background: #f1f4fa; border: 1px solid #d9dce2; margin-bottom: 10px; overflow-x: hidden;}
.guide2home .title{ line-height: 42px; padding-left: 8px; font-weight: 700; border-bottom: 1px solid #d9dce2;}
.guide2home .title span{color:#D2584D;}
.guide2home .undone,
.guide2home .done{ border-top: 1px solid #fff; padding: 10px; position: relative;}

.guide2home .done a{ display: block; width: 92px; height: 27px; line-height: 26px; text-align: center; color: #fff; background: url(http://a.xnimg.cn/imgpro/guide/g2h/g2h_1.png) 0 0 no-repeat; margin:0 auto;}
.guide2home .done a:hover{ text-decoration: none; background-position: left -27px;}
.guide2home .done a:active{ background-position: left -54px;}

.guide2home .undone .task{ margin-bottom: 5px;}
.guide2home .undone .task_name{ font-weight: 500; padding:5px 0 5px 22px; line-height: 22px; height: 22px;}
.guide2home .undone .task_undone{ background: url(http://a.xnimg.cn/imgpro/guide/g2h/g2h_6.png) left center no-repeat;}
.guide2home .undone .task_doing{ background: url(http://a.xnimg.cn/imgpro/guide/g2h/g2h_5.png) left center no-repeat;}
.guide2home .undone .task_done{ background: url(http://a.xnimg.cn/imgpro/guide/g2h/g2h_7.png) left center no-repeat;}
.guide2home .undone ul{ width: 196px; height: 34px; position: relative; background: url(http://a.xnimg.cn/imgpro/guide/g2h/g2h_2.png) center center no-repeat; margin-top: 3px;}
.guide2home .undone li{width: 32px; height: 24px; padding-top: 6px; line-height: 20px; color: #b2b2b2; text-align: center; float: left; display: inline;}

.guide2home .undone .cur{ background: url(http://a.xnimg.cn/imgpro/guide/g2h/g2h_3.png) center center no-repeat; color: #478ac4;}
.guide2home .undone .last{ background: url(http://a.xnimg.cn/imgpro/guide/g2h/g2h_4.png) center center no-repeat; text-indent: -9999px;}

.guide2home .undone .task_addfriend li{ margin:0 7px 0 8px;}

.guide2home .undone .task_login li{position: absolute; top: 1px;}
.guide2home .undone .task_login li.one{left:8px;}
.guide2home .undone .task_login li.two{left:80px;}
.guide2home .undone .task_login li.three{left:149px;}

</style>
<div class="guide2home">
	<h4 class="title">完成任务，立即体验<span>VIP</span>特权</h4>

	<c:if test="${!done}">
		<div class="undone">
			<div class="task task_addfriend">
				<a class="task_name ${task_addfriend_st}" href="http://friend.renren.com/myfriendlistx.do?nur=task_findfri#item_1" target="_self">和4个人成为好友</a>
				<c:if test="${friendCount > 0}">
					<ul>
						<li <c:if test="${friendCount == 1}">class="cur"</c:if>><strong>1</strong>人</li>
						<li <c:if test="${friendCount == 2}">class="cur"</c:if>><strong>2</strong>人</li>
						<li <c:if test="${friendCount == 3}">class="cur"</c:if>><strong>3</strong>人</li>
						<li <c:if test="${friendCount >= 4}">class="last"</c:if>>完成</li>
					</ul>
				</c:if>
			</div>
			<div class="task task_addphoto">
				<a id="task_name" class="task_name flashUploader ${task_feed_st}" onclick="return false;" href="#nogo">向好友晒晒生活</a>
			</div>
			<div class="task task_login">
				<p class="task_name ${task_login_st}">连续登录3天</p>
				<ul>
					<li class="one <c:if test="${loginDay == 1}">cur</c:if>"><strong>1</strong>天</li>
					<li class="two <c:if test="${loginDay == 2}">cur</c:if>"><strong>2</strong>天</li>
					<li class="three <c:if test="${loginDay >= 3}">last</c:if>"><strong>3</strong>天</li>
				</ul>
			</div>
		</div>
	</c:if>

	<c:if test="${done}">
		<div class="done">
			<a href="#nogo" onclick="return false;" id="guide2home">开通VIP特权</a>
		</div>
	</c:if>
</div>
<script type="text/javascript">
object.use('dom, events', function(dom, events) {
    dom.ready(function(){
		var guide2home = dom.id('guide2home'),
			task_name = dom.id('task_name');

		if(!!guide2home) {
			guide2home.addEvent('click', function() {
				new XN.net.xmlhttp({
					url: 'http://guide.' + XN.env.domain + '/guide2home',
					method: 'post',
					onSuccess: function(e) {
						window.location.href = 'http://guide.renren.com';
					},
					onError: function() {
						 XN.DO.showError('网络错误，请稍后再试...');
					}
				});
			});
		}

		if(!!task_name) {
			task_name.addEvent('click', function() {
				new XN.net.xmlhttp({
					url: 'http://guide.' + XN.env.domain + '/guideForStatistics',
					method: 'post'
				});
			});
		}
	});
});
</script>
</c:if>