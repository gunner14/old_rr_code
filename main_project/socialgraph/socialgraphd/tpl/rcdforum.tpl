<li class="clearfix" id="commonFriend_{{ID}}" onmouseover="Sizzle('a.shut-new',this)[0].style.display='block'" onmouseout="Sizzle('a.shut-new',this)[0].style.display='none'">
	<a class="shut-new x-to-hide" href="#nogo" title="不再推荐" onclick="XN.app.pymk.remove('{{ID}}');logRcd({action:'RecMixed_cancle', guest_id:{{ID}}, type:'{{TYPE_VALUE}}'});return false;"></a>
	<span class="headpichold">
		<a href="{{APP_URL}}" target="_blank" onclick="logRcd({action:'RecMixed_imgToPrf',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'})">
			<img src="{{TINY_URL}}" width="50" height="50" />
		</a>
	</span>
	<span>
		<a class="name" href="{{APP_URL}}" target="_blank" onclick="logRcd({action:'RecMixed_nameToPrf',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'})">{{NAME}} <img src="http://a.xnimg.cn/imgpro/icons/xiaozu12.png" /></a>
		<p>
			<a class="addfriend_action request-join-group" href="#nogo" data-id="{{ID}}" data-name="{{NAME}}" data-type="{{TYPE_VALUE}}" data-url="{{APP_URL}}" onclick="logRcd({action:'RecMixed_addFriend',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'});"><img src="http://a.xnimg.cn/imgpro/icons/plus-green.png" /> 加入小组</a>
		</p>
		<p style="color:#888;">
			{{#FIELDS}}
				<a href="http://www.renren.com/{{FIELD_ID}}/profile?ref=rcdforum" target="_blank" title="{{FIELD_FULL_NAME}}">{{FIELD_NAME}}</a>&nbsp;
			{{/FIELDS}}等{{COMMONS}}位好友已加入
		</p>
	</span>
	<input type="hidden" id="type_value_{{ID}}" value="{{TYPE_VALUE}}" />
</li>
