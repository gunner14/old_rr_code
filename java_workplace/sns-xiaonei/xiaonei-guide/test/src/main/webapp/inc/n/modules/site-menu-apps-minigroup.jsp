<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<article class="site-menu-apps site-menu-minigroups">
	<h3>
		<a href="http://qun.renren.com/qun/index" ui-async="async" class="more">更多</a>我的群
	</h3>
	<section>
		<c:forEach var="item" items="${requestScope.miniGrouplist}" begin="0" end="2">
			<div class="apps-item">
				<div class="item-title">
					<span class="item-main"> <a href="http://qun.renren.com/qun/ugc/home/${item.id}?from=homeleft" class="with-count" id="mg_${item.id}" ui-async="async"> <img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/minigroup/${item.icon}.png" /> ${item.name} <c:if test="${item.newFeedsNum>0}">
								<span class="apps-item-count">${item.newFeedsNum}</span>
							</c:if>
					</a>
					</span>
				</div>
			</div>
		</c:forEach>
	</section>
	<c:if test="${empty miniGrouplist||fn:length(miniGrouplist)<3}">
		<section class="minigroup-create">
			<script type="text/javascript">
				XN.getFileVersion([ 'http://s.xnimg.cn/n/apps/minigroup/core/js/manage.js?ver=$revxxx$' ]);
			</script>
			<a href="#" onclick="if(!XN.app.miniGroup){XN.loadFile('http://s.xnimg.cn/n/apps/minigroup/core/js/manage.js',function(){XN.app.miniGroup.createGroup();});}else{XN.app.miniGroup.createGroup();}return false;">创建一个新群</a>
		</section>
	</c:if>
</article>
