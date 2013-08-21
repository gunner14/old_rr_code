${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3>
	    <#if status?exists>
                <a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}:</a> ${status[0]}
         <#else>
                <a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a>
				<#if source?exists>
					使用<a href="http://i.renren.com/pe/home.action?ref=${vType}" target="_blank">照片美容工坊</a>美化了一张照片
				<#else>
					上传了${num}张照片至 <a href="${(aUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${aName[0]}</a>
				</#if>
         </#if>
		 <#if wap?exists><span class="from">通过<a href="http://mobile.renren.com/?${vType}" target="_blank">手机</a>发布 <a href="http://mobile.renren.com/?${vType}" target="_blank"><img src="http://s.xnimg.cn/a.gif"" alt="通过手机发布" class="iTinyphone" /></a></span></#if>
		 <#if appIconSrc?exists><span class="from"><a href="${appUrl[0]}" target="_blank"><img lala="${appIconSrc[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" alt="通过手机发布"/></a></span></#if>
	</h3>
	<div class="content">
			<a href="http://photo.renren.com/getphoto.do?id=${pId[0]}&owner=${fID[0]}&ref=${vType}" target="_blank"><img lala="${mSrc[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="photo"/></a>
	</div>
	<div class="details">
		<div class="legend">
			<img alt="相册" src="http://s.xnimg.cn/a.gif"" class="iPhoto"/>
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>