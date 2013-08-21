<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<div class="side-item message-box">
    <dl>
    	<c:choose>
    		<c:when test="${requestScope.FRIEND_TYPE_UNREAD>0}" >
    			<dt class="friend-new" data-num="${requestScope.FRIEND_TYPE_UNREAD}">好友<sup title="${requestScope.FRIEND_TYPE_UNREAD}条新增好友消息"><!--[if IE]><em><![endif]-->${requestScope.FRIEND_TYPE_UNREAD}<!--[if IE]></em><![endif]--></sup></dt>
 	  		</c:when>
    		<c:otherwise>
    			<dt class="friend" data-num="${requestScope.FRIEND_TYPE_UNREAD}">好友</dt>
    		</c:otherwise>
    	</c:choose>
    	<dd class="friend" style="display:none;"></dd>
        <c:choose>
    		<c:when test="${requestScope.REQUEST_TYPE_UNREAD>0}">
    			<dt class="desire-new" data-num="${requestScope.REQUEST_TYPE_UNREAD}">请求<sup title="${requestScope.REQUEST_TYPE_UNREAD}条新增请求消息"><!--[if IE]><em><![endif]-->${requestScope.REQUEST_TYPE_UNREAD}<!--[if IE]></em><![endif]--></sup></dt>
 	  		</c:when>
    		<c:otherwise>
    			<dt class="desire" data-num="${requestScope.FRIEND_TYPE_UNREAD}">请求</dt>
    		</c:otherwise>
    	</c:choose>
    	 <dd class="desire" style="display:none;"></dd>
        <c:choose>
    		<c:when test="${requestScope.MESSAGE_TYPE_UNREAD>0}" >
    			<dt class="remind-new" data-num="${requestScope.MESSAGE_TYPE_UNREAD}">提醒<sup title="${requestScope.MESSAGE_TYPE_UNREAD}条新增提醒消息"><!--[if IE]><em><![endif]-->${requestScope.MESSAGE_TYPE_UNREAD}<!--[if IE]></em><![endif]--></sup></dt>
 	  		</c:when>
    		<c:otherwise>
    			<dt class="remind" data-num="${requestScope.FRIEND_TYPE_UNREAD}">提醒</dt>
    		</c:otherwise>
    	</c:choose>
    	 <dd class="remind" style="display:none;"></dd>
    </dl>
</div>