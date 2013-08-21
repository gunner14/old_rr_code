<#include "1,0">
<li id="feed${id[0]}" class="page-flyer">
	<div class="figure">
		<a href="http://page.renren.com/pa/v?pid=${pageID[0]}&from=hnn">
			<img lala="${img[0]}" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" width="80" />
		</a>
	</div>
	<div class="article">
		<h4>
			<a href="http://page.renren.com/pa/v?pid=${pageID[0]}&from=hnn">${pageName[0]}</a>
			<img lala="http://xnimg.cn/imgpro/icons/follow.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" title="通过人人真实身份验证"/>
		</h4>
		<p>${body[0]}</p>			
		<p><a class="follow" href="${linkUrl[0]}">${linkName[0]}</a></p>
	</div>
	<#include "1,1">
</li>