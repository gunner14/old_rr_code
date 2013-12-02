<li class="clearfix" id="commonFriend_{{ID}}" onmouseover="Sizzle('a.shut-new',this)[0].style.display='block'" onmouseout="Sizzle('a.shut-new',this)[0].style.display='none'">
	<a class="shut-new x-to-hide" href="#nogo" title="不再推荐" onclick="XN.app.pymk.remove('{{ID}}');logRcd({action:'frtPage_recmmend_app_forbid', guest_id:{{ID}}, type:'{{TYPE_VALUE}}'});return false;"></a>
	<span class="headpichold">
		<a href="{{APP_URL}}?origin=50121" target="_blank" onclick="logRcd({action:'frtPage_recommend_app_imgToPrf',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'})">
			<img src="{{TINY_URL}}" width="50" height="50" />
		</a>
	</span>
	<span>
		<a class="name" href="{{APP_URL}}?origin=50121" target="_blank" onclick="logRcd({action:'frtPage_recommend_app_nameToPrf',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'})">{{NAME}} <img src="http://a.xnimg.cn/imgpro/icons/rcdapp.png" /></a>
		<p>
			<a class="addfriend_action" href="{{APP_URL}}?origin=50122" onclick="logRcd({action:'frtPage_recommend_app_addFriend',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'});" target="_blank"><img src="http://a.xnimg.cn/imgpro/icons/plus-green.png" /> 添加应用</a>
		</p>
		<p style="color:#888;">
			{{#FIELDS}}
				<a href="http://www.renren.com/{{FIELD_ID}}/profile?ref=rcdapp" target="_blank" title="{{FIELD_FULL_NAME}}">{{FIELD_NAME}}</a>&nbsp;
			{{/FIELDS}}等{{COMMONS}}位好友已安装
		</p>
	</span>
	<input type="hidden" id="type_value_{{ID}}" value="{{TYPE_VALUE}}" />
</li>
