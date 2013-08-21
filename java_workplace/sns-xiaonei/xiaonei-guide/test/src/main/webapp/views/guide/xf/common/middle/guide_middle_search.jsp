<%@ page pageEncoding="utf-8" %>
<div class="search-friend">
	<div class="tabs" id="search-tab">
		<c:choose>
			<c:when test="${visiter.stage==20}">
				<a href="javascript:;" id="tab_all" onclick="ajax_stat_link('more','guide_people_st');">全部</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_nuniv" class="current" onclick="ajax_stat_link('more','guide_people_st');">大学同学</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_middstu" onclick="ajax_stat_link('more','guide_people_st');">中学同学</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_min" onclick="ajax_stat_link('more','guide_people_st');">小学同学</a>
			</c:when>
			<c:when test="${visiter.stage==10}">
				<a href="javascript:;" id="tab_all" onclick="ajax_stat_link('more','guide_people_st');">全部</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_hs" class="current" onclick="ajax_stat_link('more','guide_people_st');">高中同学</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_junior" onclick="ajax_stat_link('more','guide_people_st');">初中同学</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_min" onclick="ajax_stat_link('more','guide_people_st');">小学同学</a>
			</c:when>
			<c:when test="${visiter.stage==30}">
				<a href="javascript:;" id="tab_all" onclick="ajax_stat_link('more','guide_people_st');">全部</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_worker" class="current" onclick="ajax_stat_link('more','guide_people_st');">同事</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_stud" onclick="ajax_stat_link('more','guide_people_st');">同学</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_msn" onclick="ajax_stat_link('more','guide_people_st');">msn联系人</a>	
			</c:when>
			<c:otherwise>
				<a href="javascript:;" id="tab_all" class="current" onclick="ajax_stat_link('more','guide_people_st');">全部</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_worker" onclick="ajax_stat_link('more','guide_people_st');">找同事</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_stud" onclick="ajax_stat_link('more','guide_people_st');">找同学</a><span class="pipe">|</span>
				<a href="javascript:;" id="tab_hometown" onclick="ajax_stat_link('more','guide_people_st');">找老乡</a>
			</c:otherwise>
		</c:choose>	
	</div>
	<%@ include file="/views/guide/xf/common/middle/guide_search_form.jsp"%>
</div>
