<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<link href="${applicationScope.urlSmallStatic}/csspro/old/base.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />

<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/util.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/base.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/xn/base.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/prototype.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/xn/chat.js?ver=$revxxx$"></script>
<style type="text/css">*{ font-size:12px; background-color:transparent}</style>

<body>

<div id="onlinemystate" style="display:none;">
	<div class="m-chat-window-IE6 notifications">
		<div class="chat-head">
			<div class="head-btn">
				<a href="#nogo" class="minimize" onfocus="this.blur();" onclick="hiddstate();" title="隐藏"></a>
			</div>
			<div class="head-name">提醒</div>
		</div>
		<div class="chat-conv" id='im_chatconv_ie6' style="height:170px;overflow-y:auto;overflow-x:hidden;">
		</div>
	</div>
</div>


<div id="onlinestatecont" class="onlinestatecont"> 
 <h3><span class="float-right"><a class="closebtn" href="#nogo" onclick="hiddstate();"></a></span>在线好友</h3>
</div>

<div id="xnimdownload" class="xnimdownload">
	<h3>
		<span class="float-right"><a class="closebtn" href="#ongo" onclick="hiddstate();" onfocus="this.blur()"></a></span>

		校内通
	</h3>
	
    <div class="notebox">
    	<a href="http://gadget.talk.${applicationScope.domain}/imhome.do?c=pop1d" target="_blank">
    		<strong>校内通</strong>
    	</a>-
    	<a href="http://gadget.talk.${applicationScope.domain}/imhome.do?c=pop2d" target="_blank">${domainIdentified["siteName"]}聊天软件</a>

    	<a href="http://gadget.talk.${applicationScope.domain}/imdownload.do?d=pop1" target="_blank"><img src="${applicationScope.urlSmallStatic}/img/im/imdownload.gif" /></a>

    	<a href="http://gadget.talk.${applicationScope.domain}/imdownload.do?d=pop2" target="_blank">下载</a>
    </div>
	
	<ul>
    	<li>* 与好友畅快聊天，不丢聊天记录</li>
        <li>* 新鲜事实时收，好友动向尽掌握</li>
        <li>* 精美新皮肤，为中国2008加油</li>
    </ul>

</div>


<c:if test="${imLeavewordCount>0}">
  <div id='leaveword' class="noticecounte" >
        <a class="msgnb" href="#nogo" onclick='talkto(event, ${visiter.id}, 0, "${visiter.eamil}", "");hiddenleaveword();report(3);'>
                <span class="number">${requestScope.imLeavewordCount}</span>
        </a>
  </div>
</c:if>

<div class="hide" id="noticecounte" onclick="showconl(0);"><a class="msgnb" href="#nogo"><span class="number">0</span></a></div>

<div id="onlinestatebar" class="onlinestatebar">
	<div class="statebar_m3">
    	<a href="http://gadget.talk.${applicationScope.domain}/imhome.do?c=pop5" target="_blank"></a>    	    	
    </div>

 <div class="statebar_m2">
  <a href="#nogo" onfocus="this.blur()" onclick="showconl(0);"></a>
   
 </div>
 <div class="statebar_m1">
  <a href="#nogo" onfocus="this.blur()" onclick="getOnLineFriends();">在线好友 <strong id='onlineFriendsCount'>(${requestScope.onlineFriendsCount})</strong></a></div>
</div>
</body>

