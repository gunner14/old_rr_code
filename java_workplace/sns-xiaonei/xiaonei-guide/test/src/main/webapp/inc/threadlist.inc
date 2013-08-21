<style>
.gapbutton{ background:none transparent; border:none; color:#3B5888; cursor:pointer; padding:0; margin:0; height:15px;}
</style>
<script type="text/javascript">
function moveThread(obj){
	var labelName = obj.options[obj.selectedIndex].text;
	if(confirm("确认将所选帖子"+labelName)==true){
		$("newLabelName").value=labelName.substring(4);
		$("act").value="mvThread";	
		$("threadsForm").submit();
	}
	obj.selectedIndex=0;
	return;
}
function checkThreadStatus(obj){
	$('act').value=obj.name;
	var threadsForm = $("threadsForm");
	for(var i=0 ;i<threadsForm.elements.length;i++){
		if(threadsForm.elements[i].type=="checkbox" && threadsForm.elements[i].checked){
			var chbox = threadsForm.elements[i];
			if(obj.name=="topThread" && chbox.getAttribute("toped")==5){
				chbox.checked='';
			}
			if(obj.name=="eliteThread" && chbox.getAttribute("elited")==-1){
				chbox.checked='';
			}
			if(obj.name=="unEliteThread" && chbox.getAttribute("elited")!=-1){
				chbox.checked='';
			}
			if(obj.name=="downThread" && chbox.getAttribute("toped")!=5){
				chbox.checked='';
			}
			if(obj.name=="lockThread" && chbox.getAttribute("closed")==1){
				chbox.checked='';
			}
			if(obj.name=="openThread" && chbox.getAttribute("closed")!=1){
				chbox.checked='';
			}
		}
	}
}
function allCheckBox(check){
	var allInputs = $('groupForum').getElementsByTagName('input');
	for(i=0;i<allInputs.length;i++){
		if(allInputs[i].name=='threads'){
			allInputs[i].checked=check;
		}
	}
}
document.domain='${applicationScope.sysdomain}';
</script>
<%@ page pageEncoding="UTF-8" %>
<input type="hidden" id="groupAuTime" name="groupAuTime" value="${timeStr}" />
<div class="box" id="groupForum">
	<c:if test="${elite!=1}">
		<h3>讨论区&nbsp;<span class="normalFont">|</span>&nbsp;<a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=${labelId}&amp;elite=1">精华区</a></h3>
	</c:if>
	<c:if test="${elite==1}">
		<h3><a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=${labelId}&amp;elite=0">讨论区</a>&nbsp;<span class="normalFont">|</span>&nbsp;精华区</h3>
	</c:if>
	<div class="boxcont">
	<c:choose>
		<c:when test="${(requestScope.tribe.access == 1) && (requestScope.isMember > 3) && (!requestScope.flag) && (requestScope.visiter.auth < 10)}">
			<div class="attention"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a>是私密群组，非成员不能阅读群组主题。<br /><a href="${applicationScope.urlGroup}/RequestTribe.do?id=${requestScope.tribe.id}">申请加入群</a></div>
		</c:when>
		<c:when test="${(requestScope.tribe.access == 1) && (requestScope.isMember == 3) && (!requestScope.flag)}">
			<div class="attention">您已经申请加入<a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a>，请等待批准，根据群组管理员设置，在获得批准之前，您不能阅读群组主题。</div>
		</c:when>
		<c:otherwise>
			<c:if test="${requestScope.tribe.labelCount==0 && requestScope.isMember <1}">
				<ul id="tabs">
					<li class="<c:if test='${labelId!=0}'>in</c:if>activetab"><a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=0&amp;elite=${elite}">所有</a></li>
					<li  class="<c:if test='${labelId!=-1}'>in</c:if>activetab"><a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=-1&amp;elite=${elite}">管理帖子</a></li>
				</ul>
			</c:if>
			<c:if test="${requestScope.tribe.labelCount>0}">
				<ul id="tabs">
					
					<li class="<c:if test='${labelId!=0}'>in</c:if>activetab"><a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=0&amp;elite=${elite}">所有</a></li>
					<c:if test='${requestScope.isMember <1}'><li  class="<c:if test='${labelId!=-1}'>in</c:if>activetab"><a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=-1&amp;elite=${elite}">无分版帖</a></li></c:if>
					<c:forEach var="item" items="${requestScope.labelList}">
						<li class="<c:if test='${item.id != requestScope.label.id}'>in</c:if>activetab"><a href="${applicationScope.urlGroup}/GetThreadList.do?id=${requestScope.tribe.id}&amp;label=${item.id}&amp;elite=${elite}">${item.name}</a></li>
					</c:forEach>
				</ul>
			</c:if>
			<c:if test="${threadCount > 30}">
				<c:set var="navUrl" value="GetThreadList.do" />
				<c:set var="navUrlEnd" value="&amp;id=${requestScope.tribe.id}&amp;label=${requestScope.labelId}&amp;elite=${elite}" />
				<c:set var="navCount" value="${threadCount}" />
				<div class="page">
					<c:set var="itemPerPage" value="30" />
					<%@ include file="/inc/pnav.inc" %>
				</div>
			</c:if>
			<c:choose>
				<c:when test="${threadCount>0}">
					<table class="threadlist">
						<tr>
							<c:if test="${(requestScope.isMember<1)&&requestScope.labelId!=0}"><th style="width:16px;"><input type="checkbox" name="allCheck" id="allCheck" onclick="allCheckBox(this.checked)"></th></c:if>
							<th class="threadTitle">主题</th>
							<th class="threadAuthor">作者</th>
							<th class="threadStat">回复</th>
							<th class="threadStat">阅读</th>
							<th class="threadResponder">最后回复</th>
						</tr>
						<c:set var="row" value="odd" />
						<form action="MultTribeThread.do" method="post" id="threadsForm" name="threadsForm">
						<input type="hidden" name="tribeId" value="${tribe.id}" />
						<input type="hidden" name="labelId" value="${labelId}" />
						<input type="hidden" name="elite" value="${elite}" />
						<input type="hidden" name="newLabelName" id="newLabelName" value="" />
						<input type="hidden" name="act" id="act" value=""/>
						<input type="hidden" name="curpage" id="curpage" value=""/>
						<c:forEach var="item" items="${requestScope.threadList}">
							<tr class="${row}">
								<c:if test="${(requestScope.isMember<1)&&requestScope.labelId!=0}"><td><input type="checkbox" name="threads" id="thread_${item.id}" value="${item.id}_${item.postCount - 1}" elited="${item.status}" closed="${item.closed}" toped="${item.type}" label="${item.label}"></td></c:if>
								<td class="threadTitle"><c:if test="${item.type==5}"><img src="${applicationScope.urlSmallStatic}/img/headtopic.gif" id="top_${item.id}" alt="置顶" /> </c:if><c:if test="${item.closed==1}"><font color="#FF6600">[锁]</font></c:if><a href="${applicationScope.urlGroup}/GetThread.do?id=${item.id}&amp;parentpage=${currentPage}&amp;curpage=0&amp;label=${requestScope.label.id}&amp;tribeId=${requestScope.tribe.id}">${item.title}<c:if test="${item.status==-1}"><font color="red">[精]</font></c:if></a></td>
								<td class="threadAuthor"><a href="${applicationScope.urlMain}/profile.do?id=${item.owner}">${item.ownerName}</a></td>
								<td class="threadStat">${item.postCount - 1}</td>
								<td class="threadStat">${item.viewCount}</td>
								<td class="threadResponder"><a href="${applicationScope.urlMain}/profile.do?id=${item.editor}">${item.editorName}</a><br /><span class="time"><fmt:formatDate value="${item.lastTime}" type="both" pattern="MM-dd HH:mm" /></span></td>
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
						<c:if test="${requestScope.isMember <1 &&requestScope.labelId!=0}">
						<tr><td colspan="3" align="left" style="text-align:left; height:25px; padding-left:4px;">管理员批量操作: 
								<input class="gapbutton" type="submit" name="delThread" value="删除" onclick="$('act').value=this.name;return confirm('确认删除所选帖子？');" />&nbsp;
								<input class="gapbutton" type="submit" name="eliteThread" value="加精华" onclick="$('act').value=this.name;return confirm('确认加所选帖子为精华帖？');" />&nbsp;
								<input class="gapbutton" type="submit" name="unEliteThread" value="取消精华" onclick="$('act').value=this.name;return confirm('确认取消所选帖子的精华？');" />&nbsp;
								<input class="gapbutton" type="submit" name="lockThread" value="锁定" onclick="checkThreadStatus(this);$('act').value=this.name;return confirm('确认锁定所选帖子？');"  />&nbsp;

								<input class="gapbutton" type="submit" name="openThread" value="解锁" onclick="checkThreadStatus(this);$('act').value=this.name;return confirm('确认解锁所选帖子？');"  />&nbsp;
								<input class="gapbutton" type="submit" name="topThread" value="置顶"  onclick="checkThreadStatus(this);$('act').value=this.name;return confirm('确认置顶所选帖子？');" />&nbsp;
								<input class="gapbutton" type="submit" name="downThread" value="解除置顶"  onclick="checkThreadStatus(this);$('act').value=this.name;return confirm('确认解除所选帖子的置顶？');" />
							</td>
							<td colspan="3" align="right">
								<c:if test="${!empty requestScope.labelList}">
								<select name="mvLabel" onchange="moveThread(this)">
									<option value="-1">转移所选帖子</option>
									<c:if test="${requestScope.labelId !=-1}"><option value="0">转移到 所有主题</option></c:if>
									<c:forEach var="item" items="${requestScope.labelList}">
										<c:if test="${requestScope.labelId != item.id}"><option value="${item.id}">转移到 ${item.name}</option></c:if>
									</c:forEach>
								</select>
								</c:if>
							</td>
						</tr>
						</c:if>
						</form>
					</table>
				</c:when>
				<c:otherwise>
					<div class="attention">
							<c:if test="${!empty requestScope.label}">本分版</c:if>
							暂时没有<c:if test="${elite==1}">精华</c:if>帖子</div>
				</c:otherwise>
			</c:choose>
			<c:if test="${navCount > 30}">
				<div class="page">
					<c:set var="itemPerPage" value="30" />
					<%@ include file="/inc/pnav.inc" %>
				</div>
			</c:if>
		</c:otherwise>
	</c:choose>
	<c:choose>
		<c:when test="${(requestScope.tribe.access == 2 || requestScope.tribe.access == 3) && (requestScope.isMember > 3)}">
			<div class="attention"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a>&nbsp;友情提示:你还不是该群组成员，可以阅读群组主题但不能发表和回复。你可以<a href="${applicationScope.urlGroup}/RequestTribe.do?id=${requestScope.tribe.id}">申请加入群组</a></div>
		</c:when>
		<c:when test="${(requestScope.tribe.access == 2 || requestScope.tribe.access == 3) && (requestScope.isMember == 3)}">
			<div class="attention">您已经申请加入<a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a>，请等待管理员批准。</div>
		</c:when>
		<c:when test="${(requestScope.isMember <= 1)}">
			<script type="text/javascript">
			var isClicked = false;
			function onReferClick()
			{
				isClicked = true;
				if(isAllowed)
					SetFocus("simplerte");
			}
			function checkPostForm(){
				timeout();
				return gbtextval(${requestScope.tribe.id})&&checkVCode();
			}
			function checkVCode(){
				if(!$('checkcode')||$('checkcode').value==''){
					alert("验证码不能为空");
					$('checkcode').focus();
					return false;
				}
				return true;
			}
			</script>
			<form method=post action="${applicationScope.urlUpload}/Post.do?tribe=${requestScope.tribe.id}" onsubmit="return checkPostForm();" enctype="multipart/form-data" name="editorForm" id="editorForm">
				<p><label for="subject">新主题:</label><input type="text" class="inputtext" id="subject" name="subject" onfocus="initVCode();"  />
					<c:if test="${requestScope.tribe.labelCount>0}"></p>
				<p><label for="label">分版:</label>
						<select name="label" id="label" class="select">
							<option value="0">所有</option>
							<c:forEach var="item" items="${requestScope.labelList}" >
								<option value="${item.id}" <c:if test='${requestScope.label.id==item.id}'>selected="selected" </c:if>>${item.name}</option>
							</c:forEach>
						</select>
					</c:if>
				</p>
				<textarea name="body" id="body" style="display:none" ></textarea>
				<div onmouseover="initVCode();">
					<%@ include file="/inc/editor.inc" %>
				</div>
				<%@ include file="/inc/upFile.inc" %>
				<br>
				 <p>
				  <label class="title">验证码:</label>
				  <script language="javascript">
					function newverifypic(){
						if(!$('validCode')){
							initVCode()
						}else{
							$('validCode').src=$('validCode').src+Math.random();
						}
					}
					function initVCode(){
						if(!$('validCode')){
							$('vCodeSpan').innerHTML='<img src="http://icode.${applicationScope.domain}/getcode.do?t=groupsetting&rnd=<%=System.currentTimeMillis()%>" name="validCode" align="top" id="validCode" />';
						}
					}
				  </script>
              <input name="checkcode" id="checkcode" size="10" maxlength="5" class="inputtext" />
              <span id="vCodeSpan"></span>&nbsp;<a href="javascript:newverifypic();">看不清楚？</a></p>
				<p>
					<input type="hidden" name="tribeId" value="${tribe.id}" />
					<input type="hidden" name="labelId" value="${labelId}" />
					<input type="hidden" name="elite" value="${elite}" />
					<input type="hidden" name="curpage" value="${currentPage}" />
					<input type="hidden" name="tsc" id="tsc" value="${temp_sys_code}" />
					<input type="hidden" name="ak" id="ak" value="${requestScope.ak}" />
					<input type="hidden" name="ac" id="ac" value="${requestScope.ac}" />
					<input type="hidden" name="at" id="at" value="" />
					<input type="submit" name="submit" class="subbutton" id="editorFormBtn" value="发表" />
				</p>
			</form>
		</c:when>
		<c:when test="${(requestScope.tribe.access == 1) && (requestScope.isMember >= 2)}">
			<div class="attention"><a href="${applicationScope.urlGroup}/GetTribe.do?id=${requestScope.tribe.id}&amp;curpage=0">${requestScope.tribe.name}</a> 是私密群组，你可以<a href="${applicationScope.urlGroup}/RequestTribe.do?id=${requestScope.tribe.id}">申请加入群组</a></div>
		</c:when>
	</c:choose>
	</div>
	<div class="more"><c:if test="${isMember<2}"><a href="${urlGroup}/TribeUserThreadList.do?id=${tribe.id}">我的主题</a>&nbsp;|&nbsp;</c:if><a href="#subject">发表新主题</a></div>
</div>
