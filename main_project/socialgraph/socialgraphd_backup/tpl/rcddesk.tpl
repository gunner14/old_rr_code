<li onmouseout="Sizzle('a.shut-new',this)[0].style.display='none'" onmouseover="Sizzle('a.shut-new',this)[0].style.display='block'" id="commonFriend_{{ID}}" class="clearfix">
	<a onclick="XN.app.pymk.remove('{{ID}}');logRcd({action:'frtPage_recmmend_desk_forbid', guest_id:{{ID}},type:'{{TYPE_VALUE}}'});return false;" title="不再推荐" href="javascript:;" class="shut-new x-to-hide" style="display: none;">
	</a>
	<span class="headpichold">
		<a onclick="logRcd({action:'frtPage_recommend_desk_imgToPrf',guest_id:{{ID}}})" target="_blank" href="http://im.renren.com/?subver=57">
			<img width="50" height="50" src="http://a.xnimg.cn/imgpro/icons/renren-desk.jpg">
		</a>
	</span>
	<span>
		<a onclick="logRcd({action:'frtPage_recommend_desk_nameToPrf',guest_id:{{ID}}})" href="http://im.renren.com/desktop/rrsetup-57.exe?word1" target="_blank" class="name">人人桌面</a>
		<p>
		<a onclick="logRcd({action:'frtPage_recommend_desk_addFriend',guest_id:{{ID}},type:'{{TYPE_VALUE}}'});" href="http://im.renren.com/desktop/rrsetup-57.exe?word2" class="addfriend_action" target="_blank">
		<img src="http://a.xnimg.cn/imgpro/icons/renren-desk-download.gif"> 立即下载</a>
		</p>
		<p>
			<span class="pymk-type">有5,062,378位用户正在使用</span>
		</p>
	</span>
	<input type="hidden" id="type_value_{{ID}}" value="{{TYPE_VALUE}}" />
</li>
