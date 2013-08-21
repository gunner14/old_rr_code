<%@ page pageEncoding="utf-8" %>
<c:if test="${requestScope.guideZhengZhouGroup}">
	<div class="groupmod">
		<div class="title"><strong>郑州小组推荐</strong><a href="${applicationScope.urlXiaozu}/xiaozu/100542/members">(${requestScope.memberCount}个郑州人)</a></div>
		<div class="group-body clearfix">
			<div class="group-cover">
				<a href="${applicationScope.urlXiaozu}/xiaozu/100542/thread">
					<img src="http://fmn.rrimg.com/fmn048/event/20101012/1410/e_head_Rzat_0866000ac2bb5c40.jpg" alt="小组封面" width="100" height="67"/>
				</a>
				<a href="${applicationScope.urlXiaozu}/xiaozu/100542/thread">郑州人民公社</a>
				<p class="f-gray">关于郑州的吃喝玩乐</p>
			</div>
			<div class="group-article">
				<h3>最新帖子更新</h3>
				<ul class="group-title">
					<li><a href="http://xiaozu.renren.com/xiaozu/100542/thread/331743648?ref=market_zzxiaozu">尽心感受郑州的100个理由,第几点打动了你？</a></li>
					<li><a href="http://xiaozu.renren.com/xiaozu/100542/thread/331743653?ref=market_zzxiaozu">十大经典小资生活方式</a></li>
					<li><a href="http://xiaozu.renren.com/xiaozu/100542/thread/331744003?ref=market_zzxiaozu">男女必看的12部电影，你看过几部呢</a></li>
					<li><a href="http://xiaozu.renren.com/xiaozu/100542/thread/331749681?ref=market_zzxiaozu">6种饮食坏习惯，让你吃出秋乏</a></li>
					<li><a href="http://xiaozu.renren.com/xiaozu/100542/thread/331750276?ref=market_zzxiaozu">舒缓心理压力的十种分析法，我试过了，效果真的很好</a></li>
				</ul>
			</div>
		</div>
	</div>
</c:if>