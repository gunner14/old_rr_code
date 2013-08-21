<%@ page contentType="text/html;charset=UTF-8" %>
<str:replace replace="NL" with=" " newlineToken="NL"> 
	<style>
	#default-pandora{background:#F5F5F5;padding:10px;line-height:1.8em;}
	#default-pandora h2{height:2.0em;line-height:2.0em;}
	#default-pandora ul{list-style:disc inside;}
	#default-pandora span{float:right;width:235px;}
	</style>
	<div id="default-pandora">
	欢迎加入${domainIdentified["siteName"]}，这是新手上路指南。
	<h2>如何在${domainIdentified["siteName"]}找到同学？</h2>
	<ul>
	<li><strong>点击</strong>基本信息中的任意资料，会列会相关的同学。</li>
	<img src="${applicationScope.urlSmallStatic}/img/guide/step4.gif" />
	<li>使用<a href="${applicationScope.urlBrowse}/os.do">搜索</a>功能。对同学进行精确查找。</li>
	<img src="${applicationScope.urlSmallStatic}/img/guide/step5.gif" />
	</ul>
	<h2>加为好友后，你可以做什么？</h2>
	<ul>
	<li>快速在<a href="${applicationScope.urlProfile}/myfriendlist.do">好友列表</a>中找到好友；</li>
	<li>在首页第一时间了解好友的最新动态。</li>
	<img src="${applicationScope.urlSmallStatic}/img/guide/step3.gif" />
	</ul>
	<p>
	<span>如果你还有什么不懂，欢迎你向${domainIdentified["siteName"]}管理员咨询，或者到<a href="/GetTribe.do?portal=groupHome&id=230364971">新手上路群</a>问问。</span>
	<img src="${applicationScope.urlSmallStatic}/img/guide/group.jpg" />
	</p>
	</div>
</str:replace>	