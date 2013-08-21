<!--{{TYPE_VALUE}}
{{ID}}
{{NAME}}
{{TINY_URL}}
{{CONCERN}}
{{URL}}-->
<li class="clearfix" id="commonFriend_{{ID}}" onmouseover="Sizzle('a.shut-new',this)[0].style.display='block'" onmouseout="Sizzle('a.shut-new',this)[0].style.display='none'">
        <a class="shut-new x-to-hide" href="javascript:;" title="不再推荐" onclick="XN.app.pymk.remove('{{ID}}');logRcd({action:'RecMixed_cancle', guest_id:{{ID}},type:'{{TYPE_VALUE}}'});return false;"></a>
        <span class="headpichold">
                <a href="{{URL}}" onclick="logRcd({action:'RecMixed_imgToPrf',guest_id:{{ID}},type:'{{TYPE_VALUE}}'})">
                        <img src="{{TINY_URL}}" width="50" height="50" />
                </a>
        </span>
        <span>
                <a class="name" href="{{URL}}" onclick="logRcd({action:'RecMixed_nameToPrf',guest_id:{{ID}},type:'{{TYPE_VALUE}}'})">{{NAME}} <img src="http://a.xnimg.cn/imgpro/icons/site.png" /></a>
                <p>
                        <a class="addfriend_action request-follow-site" data-siteid="{{ID}}" data-siteurl="{{URL}}" data-sitename="{{NAME}}" href="http://zhan.renren.com/zhan/{{ID}}" onclick="logRcd({action:'RecMixed_addFriend',guest_id:{{ID}},type:'{{TYPE_VALUE}}'});return false;"><img src="http://a.xnimg.cn/imgpro/icons/plus-green.png" /> 关注</a>
	        </p>
		<p><span class="pymk-type">人人小站({{CONCERN}}人关注)</span></p>
        </span>
        <input type="hidden" id="type_value_{{ID}}" value="{{TYPE_VALUE}}"/>
</li>
