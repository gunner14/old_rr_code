<%@ page pageEncoding="GBK" %>
<c:choose>
	<c:when test="${(requestScope.tribe.access == 1) && (requestScope.isMember > 3) && (!requestScope.flag)}">
		<div class="attention"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a>是私人群，非成员不能阅读群主题。<br /><a href="${applicationScope.urlGroup}/RequestTribe.do?id=${requestScope.tribe.id}">申请加入群</a></div>
	</c:when>
	<c:when test="${(requestScope.tribe.access == 1) && (requestScope.isMember == 3) && (!requestScope.flag)}">
		<div class="attention">您已经申请加入<a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a>，请等待批准，根据群管理员设置，在获得批准之前，您不能阅读群主题。</div>
	</c:when>
	<c:otherwise>
		<c:choose>
			<c:when test="${empty requestScope.label}">
				<c:set var="threadCount" value="${requestScope.tribe.threadCount}" />
			</c:when>
			<c:otherwise>
				<c:set var="threadCount" value="${requestScope.label.threadCount}" />
			</c:otherwise>
		</c:choose>
		<c:if test="${requestScope.tribe.labelCount>0}">
			<ul id="tabs">
				<li class="<c:if test='${!empty requestScope.label}'>in</c:if>activetab"><a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=0">所有</a></li>
				<c:forEach var="item" items="${requestScope.labelList}">
					<li class="<c:if test='${item.id != requestScope.label.id}'>in</c:if>activetab"><a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=${item.id}">${item.name}</a></li>
				</c:forEach>
			</ul>
		</c:if>
		<c:if test="${threadCount > 30}">
			<c:set var="navUrl" value="GetThreadList.do" />
			<c:set var="navUrlEnd" value="&amp;id=${requestScope.tribe.id}&amp;label=${requestScope.label.id}" />
			<c:set var="navCount" value="${threadCount}" />
			<div class="page">
				<c:set var="itemPerPage" value="30" />
				<%@ include file="/inc/pnav_GBK.inc" %>
			</div>
		</c:if>
		<c:choose>
			<c:when test="${threadCount>0}">
				<table class="threadlist">
					<tr>
						<th class="threadTitle">主题</th>
						<th class="threadAuthor">作者</th>
						<th class="threadStat">回复<!-- / 阅读--></th>
						<th class="threadResponder">最后回复</th>
					</tr>
					<c:set var="row" value="odd" />
					<c:forEach var="item" items="${requestScope.threadList}">
						<tr class="${row}">
							<td class="threadTitle"><c:if test="${item.type==5}"><img src="${applicationScope.urlSmallStatic}/img/headtopic.gif" alt="置顶" /> </c:if><a href="${applicationScope.urlGroup}/GetThread.do?id=${item.id}&amp;parentpage=${currentPage}&amp;curpage=0&amp;label=${requestScope.label.id}&amp;tribeId=${requestScope.tribe.id}">${item.title}</a></td>
							<td class="threadAuthor"><a href="${applicationScope.urlMain}/profile.do?id=${item.owner}">${item.ownerName}</a></td>
							<td class="threadStat">${item.postCount - 1}<!-- / ${item.viewCount}--></td>
							<td class="threadResponder"><span class="time"><fmt:formatDate value="${item.lastTime}" type="both" pattern="MM-dd HH:mm" /></span> | <a href="${applicationScope.urlMain}/profile.do?id=${item.editor}">${item.editorName}</a></td>
						</tr>
						<c:choose>
							<c:when test="${row == 'odd'}">
								<c:set var="row" value="even" />
							</c:when>
							<c:otherwise>
								<c:set var="row" value="odd" />
							</c:otherwise>
						</c:choose>
					</c:forEach>
				</table>
			</c:when>
			<c:otherwise>
				<div class="attention">
						<c:if test="${!empty requestScope.label}">本分版</c:if>
						暂时没有帖子</div>
			</c:otherwise>
		</c:choose>
		<c:if test="${navCount > 30}">
			<div class="page">
				<c:set var="itemPerPage" value="30" />
				<%@ include file="/inc/pnav_GBK.inc" %>
			</div>
		</c:if>
	</c:otherwise>
</c:choose>
<c:choose>
	<c:when test="${(requestScope.tribe.access == 2) && (requestScope.isMember > 3)}">
		<div class="attention"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a> 是半公开群，非成员可以阅读群主题但不能发表和回复。你可以<a href="${applicationScope.urlGroup}/RequestTribe.do?id=${requestScope.tribe.id}">申请加入群</a></div>
	</c:when>
	<c:when test="${(requestScope.tribe.access == 2) && (requestScope.isMember == 3)}">
		<div class="attention">您已经申请加入<a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a>，请等待批准，根据群管理员设置，在获得批准之前，您可以阅读群主题但不能发表和回复。</div>
	</c:when>
	<c:when test="${(requestScope.tribe.access == 3) || (requestScope.isMember <= 1)}">
		<script type="text/javascript">
		var isClicked = false;
		function onReferClick()
		{
			isClicked = true;
			if(isAllowed)
				getEl('textedit').focus();
		}
		function for5qtopic(){
		  subnumstr = new String(getEl("subject").value);
			if (subnumstr.length ==0)
			{
				alert('主题标题不能为空');
				getEl("subject").focus();
				return false;
			}
			if(getEl('body')==null||getEl('body')==""){
			    alert("内容不能为空!") ;
			    return false ;
			}
			var numsub = getEl("theFile").value.length;
			isAttEmptyGoCommSubmit(numsub);			
			return true ;
			
		}
		var fileCtrlLen;
		function isAttEmptyGoCommSubmit(fileCtrlLen)
		{
			if(fileCtrlLen == 0) //没有附件
				document.editorForm.action = "${applicationScope.urlGroup}/gbkPost.do";
			else
				document.editorForm.action = "${applicationScope.urlUpload}/gbkPost.do";
				
			document.editorForm.submit;
		}		
		</script>
		<form action="${applicationScope.urlUpload}/gbkPost.do" method=post name="editorForm" enctype="multipart/form-data" onsubmit="return for5qtopic();">
			<p><label for="subject">新主题:</label><input type="text" class="inputtext" id="subject" name="subject" /></p>
			<c:if test="${requestScope.tribe.labelCount > 0}">
				<p><label for="label">分版:</label>
					<select name="label" id="label" class="select">
						<option value="0">所有</option>
						<c:forEach var="item" items="${requestScope.labelList}" >
							<option value="${item.id}" <c:if test='${requestScope.label.id==item.id}'>selected="selected" </c:if>>${item.name}</option>
						</c:forEach>
					</select>
				</p>
			</c:if>
			<%-- div id="actionMenu"><a href="#" onclick="return dlgAction('img','textedit')"><img src="${applicationScope.urlSmallStatic}/img/blog/image.gif" alt="[插入图片]" /></a><a href="#" onclick="return dlgAction('link','textedit')"><img src="${applicationScope.urlSmallStatic}/img/blog/hyperlink.gif" alt="[插入链接]" /></a><a href="#" onclick="getEl('theFileDiv').style.display='inline';return false;"><img src="${applicationScope.urlSmallStatic}/img/paperclip.gif" alt="[上传图片或文件]" /></a><span id="theFileDiv"> <html:file property="theFile" styleId="theFile" size="25" /> <input type="button" onclick="getEl('theFileDiv').style.display='none'" value="取消" /></span></div>
			<div class="resizable-textarea">
				<textarea id="textedit" name="body" cols="100%" rows="10" class="resizable" onfocus="if(isClicked){cc(event); isClicked=false}"></textarea>
			</div --%>
			<textarea name="body" id="body" cols="100%" rows="10" style="display:none"></textarea>
			<%@ include file="/inc/editor.inc" %>
			<div id="upFile">
			上传文件或图片（<a href="${applicationScope.urlProfile}/info/Help.do#help1_30" target="_blank">查看支持类型</a>） <input type="file" name="theFile" id="theFile" />
			</div>
			<p>
				<input type="hidden" name="tribe" value="${requestScope.tribe.id}" />
				<input type="hidden" name="curpage" value="${currentPage}" />
				<input type="hidden" name="ak" value="${requestScope.ak}" />				
				<input type="submit" id="textEditSubBtn" name="submit" class="subbutton" value="发表主题" />
			</p>
		</form>
	</c:when>
	<c:when test="${(requestScope.tribe.access == 1) && (requestScope.isMember >= 2)}">
		<div class="attention"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a> 是私人群，你可以<a href="${applicationScope.urlGroup}/RequestTribe.do?id=${requestScope.tribe.id}">申请加入群</a></div>
	</c:when>
</c:choose>
