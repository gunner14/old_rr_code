<%@ page pageEncoding="utf-8" %>
<c:if test="${domainIdentified['siteNameAbbr']=='rr'}">
	<div class="side-item hot-page">
		<div class="side-item-header clearfix">
 			<h4>热门公共主页</h4>
			<div class="option">
				<a href="${applicationScope.urlPage}/home/allpages?&amp;pma=p_guide_netbar_m_hotpage_v1_a_pagemore" target="_blank">更多</a>
			</div>
		</div>
		<div class="side-item-body clearfix">
			<div class="hp-figure">
				<a href="${applicationScope.urlPage}/600002235?&amp;pma=p_guide_netbar_m_hotpage_v1_a_view_page_1_pic" target="_blank">
					<img src="${applicationScope.urlStatic}/imgpro/guide/hot-page-lyc.png" width="80px" height="80px" />
				</a>
				<p>
					<span class="num">1</span>
					<a href="${applicationScope.urlPage}/600002235?&amp;pma=p_guide_netbar_m_hotpage_v1_a_view_page_1_name" target="_blank">李宇春</a>
				</p>
            </div>
			<ul class="hp-more">
				<li>
					<span class="num">2</span>
					<a href="${applicationScope.urlPage}/600002233?&amp;pma=p_guide_netbar_m_hotpage_v1_a_view_page_2" target="_blank">光良</a>
				</li>
				<li>
					<span class="num">3</span>
					<a href="${applicationScope.urlPage}/600002300?&amp;pma=p_guide_netbar_m_hotpage_v1_a_view_page_3" target="_blank">S.H.E</a>
				</li>
				<li>
					<span class="num">4</span>
					<a href="${applicationScope.urlPage}/600002342?&amp;pma=p_guide_netbar_m_hotpage_v1_a_view_page_4" target="_blank">陈奕迅</a>
				</li>
				<li>
					<span class="num">5</span>
					<a href="${applicationScope.urlPage}/600002426?&amp;pma=p_guide_netbar_m_hotpage_v1_a_view_page_5" target="_blank">李开复</a>
				</li>
				<li>
					<span class="num">6</span>
					<a href="${applicationScope.urlPage}/600002256?&amp;pma=p_guide_netbar_m_hotpage_v1_a_view_page_6" target="_blank">羽泉</a>
				</li>
			</ul>
	 	</div>
	</div>
</c:if>