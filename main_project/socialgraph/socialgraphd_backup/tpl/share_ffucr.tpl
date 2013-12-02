<li class="clearfix" id="commonFriend_{{ID}}" onmouseover="Sizzle('a.shut-new',this)[0].style.display='block'" onmouseout="Sizzle('a.shut-new',this)[0].style.display='none'">
	<a class="shut-new x-to-hide" href="javascript:;" title="不再推荐此人" onclick="XN.app.pymk.remove('{{ID}}');logRcd({action:'RecShare_forbid', guest_id:{{ID}},type:'{{TYPE_VALUE}}'});return false;"></a>
	<span class="headpichold">
		<a href="http://www.renren.com/profile.do?id={{ID}}" onclick="logRcd({action:'RecShare_imgToPrf',guest_id:{{ID}},type:'{{TYPE_VALUE}}'})">
			<img src="{{TINY_URL}}" width="50" height="50" onload="clipImage(this)" />
		</a>
	</span>
	<span>
		<a class="name" href="http://www.renren.com/profile.do?id={{ID}}" onclick="logRcd({action:'RecShare_nameToPrf',guest_id:{{ID}},type:'{{TYPE_VALUE}}'})">{{NAME}}</a>
		<p>
			<a class="addfriend_action" href="javascript:;" onclick="showRequestFriendDialog('{{ID}}','{{NAME}}','{{TINY_URL}}', '', '', XN.app.pymk.removeFromAdd);logRcd({action:'RecShare_addFriend',guest_id:{{ID}},type:'{{TYPE_VALUE}}'});return false;"><img src="http://a.xnimg.cn/imgpro/icons/plus-green.png" /> 加为好友</a>
		</p>
		<p><a href="javascript:;" onclick="XN.app.pymk.showProfile('{{ID}}');logRcd({action:'RecShare_info',guest_id:{{ID}},type:'{{TYPE_VALUE}}'});">查看资料</a><span class="pymk-type">({{FIELD}}个共同好友)</span></p>	
	</span>	
	<div style="display:none;" class="comefrom" id="more-mayknow-{{ID}}">
		<div id="more-person-data-{{ID}}"></div>
		<p><span class="profile-label">共同好友：</span>&nbsp;
		{{#FIELDS}}
			<a href="http://www.renren.com/profile.do?id={{FIELD_ID}}">{{FIELD_NAME}}</a>&nbsp;
		{{/FIELDS}}等{{FIELD}}人
		</p>
	</div>
	<input type="hidden" id="type_value_{{ID}}" value="{{TYPE_VALUE}}"/>
</li>

