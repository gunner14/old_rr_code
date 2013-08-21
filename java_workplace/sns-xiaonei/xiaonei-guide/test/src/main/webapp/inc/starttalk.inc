<%@ page pageEncoding="UTF-8" %>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/xn/gossip.js?ver=$revxxx$"></script>

<form method="post" onsubmit="gossip_submit(this,${visiter.stage},${visiter.selected});return false;">
	<div id="starttalkForm" style="display:block;">
		<textarea id="cmtbody" name="body" cols="100%" rows="4" tabindex="1" onfocus="focusAction(event)"></textarea>
		<%--
		<c:if test="${requestScope.visiter.stage ==30 && requestScope.visiter.selected==0}">
		<p><a href="${applicationScope.urlProfile}/notselectuser.do?action=no">成为星级用户</a>，留言不需输验证码</p>
		<p><span class="code">请输入验证码:<input type="text" class="inputtext" name="verifyCode" id="verifyCode"/> <img class="validate-num" style="float:none;" id="verifyPic" src="http://icode.${applicationScope.domain}/getcode.do?t=gossip&rnd=<%=System.currentTimeMillis()%>"/> <a href="#nogo" onclick="refreshCode();">看不清换一张? </a></span></p>
		
		</c:if>
		--%>
		<label class="labelRadio" for="whisper">
			<input type="checkbox" id="whisper" value="1" tabindex="3" name="only_to_me"/>
		 悄悄话
		</label>
		<p>
			<c:if test="${requestScope.visiter.id != requestScope.guestBase.id}">
				<a class="talk-gift" href="${applicationScope.urlGift}/send.do?fid=${requestScope.guestBase.id}">送${pageScope.h}礼物</a>
			</c:if>
			<input type="submit" value="留言" class="subbutton" id="editorFormBtn" tabindex="2" />
			<!-- span id="cancelReplay_span" style="display:none;"><input type="button" value="取消回复" class="inputbutton" onclick="cancelLeaveWord()" tabindex="3" /></span -->
			<c:if test="${requestScope.visiter.auth >= 10}">
			<%--<label class="labelRadio" for="whisper"><input type="checkbox" id="whisper" value="1" tabindex="3" name="only_to_me"/>悄悄话</label>
			--%></c:if>
			<!-- span class="discription">(每条最多2000字)</span -->
			<input type="hidden" name="curpage" value="${param.curpage}" />
			<input type="hidden" name="from" value="${from}" />
			<input type="hidden" name="id" value="${requestScope.guestBase.id}" />
			<input type="hidden" name="cc" id="leaveWordTo" value="${requestScope.guestBase.id}" />
			<input type="hidden" name="ak" value="${requestScope.ak}" />
			<input type="hidden" name="cccc" id="gossipReplyToId"/>
			<input type="hidden" name="tsc" id="tsc" value="${temp_sys_code}" />
		</p>
	</div>
<form>
<c:if test="${empty requestScope.host}">
<script type="text/javascript">
getEl("starttalkBtn").disabled="true";
getEl("starttalkBtn").value="登录后才能留言";
</script>
</c:if>
