${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
	<#include "1,4">
	<h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 在班级 <a href="${(cUrl[0])?replace('xiaonei.com','renren.com')}" target="_blank">${cName[0]}</a> 上传了文件 <a href="${(fUrl[0])?replace('xiaonei.com','renren.com')}&ref=${vType}" target="_blank">${fTitle[0]}</a></h3>
	<div class="details">
		<div class="legend">
			<img src="http://s.xnimg.cn/a.gif" alt="班级" class="iClass" />
			<span class="duration"><@format>${time[0]}</@format></span>
		</div>
	</div>
<#include "1,1">
</li>
</#compress>