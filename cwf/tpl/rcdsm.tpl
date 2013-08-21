<li onmouseout="Sizzle('a.shut-new',this)[0].style.display='none'" onmouseover="Sizzle('a.shut-new',this)[0].style.display='block'" id="commonFriend_{{ID}}" class="clearfix" data-index="0">
	<a onclick="XN.app.pymk.remove('{{ID}}'); logRcd({action:'RecMixed_cancle', guest_id:{{ID}}, type:'{{TYPE_VALUE}}'}); return false;" title="不再推荐此人" href="javascript:;" class="shut-new x-to-hide" style="display: none;"></a>
	<span class="headpichold">
		<a href="http://j.renren.com/u/{{ID}}" target="_blank" onclick="logRcd({action:'RecMixed_imgToPrf',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'})" >
			<img width="50" height="50" onload="clipImage(this)" src="{{TINY_URL}}"/>
		</a>
	</span>
	<span>
		<a href="http://j.renren.com/u/{{ID}}" target="_blank" class="name" onclick="logRcd({action:'RecMixed_imgToPrf',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'})" >{{NAME}}<img src="http://a.xnimg.cn/imgpro/icons/j-hipster.png" /></a>
		<p>
			<a onclick="logRcd({action:'RecMixed_addFriend',guest_id:{{ID}}, type:'{{TYPE_VALUE}}'}); return false;" href="javascript:;" class="addfriend_action request-join-jd" data-id="{{ID}}" data-name="{{NAME}}">
				<img src="http://a.xnimg.cn/imgpro/icons/plus-green.png" /> 关注达人
			</a>
		</p>
		 <p style="color:#888;">
                        {{#FIELDS}}
                                <a href="http://www.renren.com/{{FIELD_ID}}/profile?ref=rcdforum" target="_blank" title="{{FIELD_FULL_NAME}}">{{FIELD_NAME}}</a>&nbsp;
                        {{/FIELDS}}等{{COMMONS}}位好友已关注
                </p>
	</span>
	<input type="hidden" value="{{TYPE_VALUE}}" id="type_value_{{ID}}"/>
</li>
