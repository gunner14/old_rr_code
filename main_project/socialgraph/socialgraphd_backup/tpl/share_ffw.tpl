<li class="clearfix" id="commonFriend_{{ID}}" uid="{{ID}}" onmouseover="Sizzle('a.shut-new',this)[0].style.display='block'" onmouseout="Sizzle('a.shut-new',this)[0].style.display='none'">
	<a logtype="cancle" uid="{{ID}}" class="shut-new x-to-hide" href="javascript:;" title="不再推荐此人" onclick="XN.app.pymk.remove('{{ID}}');return false;"></a>
	<span class="headpichold">
		<a logtype="imgToPrf" uid="{{ID}}" href="http://www.renren.com/profile.do?id={{ID}}">
			<img logtype="imgToPrf" uid="{{ID}}" src="{{TINY_URL}}" width="50" height="50" onload="clipImage(this)" />
		</a>
	</span>
	<span>
		<a logtype="nameToPrf" uid="{{ID}}" class="name" href="http://www.renren.com/profile.do?id={{ID}}">{{NAME}}</a>
		<p>
			<a logtype="addFriend" uid="{{ID}}" class="addfriend_action" href="javascript:;" onclick="showRequestFriendDialog('{{ID}}','{{NAME}}','{{TINY_URL}}', '', '', XN.app.pymk.removeFromAdd);return false;"><img src="http://a.xnimg.cn/imgpro/icons/plus-green.png" /> 加为好友</a>
		</p>
		<p><a logtype="info" uid="{{ID}}" href="javascript:;" onclick="XN.app.pymk.showProfile('{{ID}}');">查看资料</a><span class="pymk-type">({{FIELD}}个共同好友)</span></p>	
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
	<input type="hidden" id="guest_{{ID}}" value="{{ID}}"/>
	<input type="hidden" id="log_cancle_{{ID}}" value="RecShare_cancle"/>
	<input type="hidden" id="log_imgToPrf_{{ID}}" value="RecShare_imgToPrf"/>
	<input type="hidden" id="log_nameToPrf_{{ID}}" value="RecShare_nameToPrf"/>
	<input type="hidden" id="log_addFriend_{{ID}}" value="RecShare_addFriend"/>
	<input type="hidden" id="log_info_{{ID}}" value="RecShare_info"/>
	<input type="hidden" id="log_show_{{ID}}" value="RecShare_show"/>
	<input type="hidden" id="log_addFriendEnd_{{ID}}" value="RecShare_addFriendEnd"/>
</li>

