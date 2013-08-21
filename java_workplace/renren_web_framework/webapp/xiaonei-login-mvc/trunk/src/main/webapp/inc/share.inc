<%--
如何配置分享
1，action里调用buildShare();request.setAttribute(share,share);
2，页面包含这个文件
3，组件包和配置
	<action path="/share/submit" type="com.xiaonei.platform.component.share.action.SubmitShare" scope="request" validate="false"/>
	<action path="/share/popup" type="com.xiaonei.platform.component.share.action.PopShareAction" parameter="action" scope="request" validate="false">
	   <forward name="succ" path="/pages/share-ajax.jsp" />
	</action>
--%>
<%-- 添加的id被相册预取使用 --%>
<div class="share-actions" id="share-actions">
	<a href="javascript:void(0);" class="share"  onclick='pop_share();'>分享</a>
</div>
<input type="hidden" id="link" name="link" value="${share.link}" />
<input type="hidden" id="type" name="type" value="${share.type}" />
<input type="hidden" id="title" name="title" value="${share.title}" />
<input type="hidden" id="pic" name="pic" value="${share.pic }" />
<input type="hidden" id="fromno" name="fromno" value="${share.fromno }" />
<input type="hidden" id="fromname" name="fromname" id="fromname" value="${share.fromname }" />
<input type="hidden" id="fromuniv" name="fromuniv" id="fromuniv" value="${share.fromuniv }" />
<input type="hidden" id="albumid" name="albumid" value="${share.albumid}" />
<div id="summary" name="summary" style="display:none">${share.summary}</div>
<input id="largeurl" type="hidden" name="largeurl" value="${largeurl}"/>