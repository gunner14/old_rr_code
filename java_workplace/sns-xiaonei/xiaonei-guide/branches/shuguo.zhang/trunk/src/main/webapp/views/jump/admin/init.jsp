<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %> 

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="true">
  <head>
    <%@ include file="/views/jump/admin/inc/header.jsp"%>
    <title>[Management] - [Home]</title>
    
    <script language="javascript">
    function _submit(action,id) {
        document.mainform.theAction.value = action;
        document.mainform.theId.value = id;
        if(action=="del"){
            if(confirm("您确认执行删除操作么？")==false) return; 
        }
        document.mainform.submit();
    }
    </script>
  </head>
  
  <body>
  <%@ include file="/views/jump/admin/inc/top.jsp"%>
<div>
  <form name="mainform" method="post" action="${applicationScope.baseUrl}/admin/jumpmanage">
  <input type="hidden" name="theAction" value="" />
  <input type="hidden" name="theId" value="" />
<table width="100%">
	<tr class="td2">
		<td>id</td>
		<td>ss_id</td>
		<td>rt_id</td>
		<td>game_stat_id</td>
		<td>code_id</td>
		<td>remark</td>
		<td>update_time</td>
		<td>oper</td>
	</tr>
		<c:set var="listcount" value="${fn:length(modellist)}" />
		<c:forEach var="item" items="${modellist}" begin="0" step="1"
			varStatus="i">
			<tr class="td${i.index mod 2 + 3}">
				<td>${item.id}</td>
				<td>${item.ss_id}</td>
				<td>${item.rt_id}</td>
				<td>${item.game_stat_id}</td>
				<td>${item.code_id}</td>
				<td>
				<a target="_blank" href="http://guide.renren.com/jumptogame?ss=${item.ss_id}&rt=${item.rt_id}">go</a>
				${item.remark}</td>
				<td>${item.update_time}</td>
				<td><span style="cursor: pointer; color: blue;"
					onclick="_submit('set',${item.id })">set</span> <span
					style="cursor: pointer; color: red;"
					onclick="_submit('del',${item.id })">del</span></td>
			</tr>
		</c:forEach>
	<tr class="td${(listcount) mod 2 + 3}">
		<td><!--id--></td>
		<td><!--ss_id--></td>
		<td><!--rt_id--></td>
		<td><!--game_stat_id--></td>
		<td><!--code_id--></td>
		<td><!--remark--></td>
		<td><!--update_time--></td>
		<td><span style="cursor: pointer; color: blue;"
			onclick="_submit('add',0)">add</span></td>
	</tr>
</table>
<table>
    <tr>
        <td>
            <span style="cursor:pointer;color:blue" onclick="_submit('init',0)">Refresh</span>
        </td>
    </tr>
    <tr>
        <td>
            <span style="cursor:pointer;color:red" onclick="_submit('exit',0)">Exit</span>
        </td>
    </tr>
    <%if(isManager(host)){%>
    <tr>
        <td>
            <a href="?app=refresh">Reload</a>
        </td>
    </tr>
    <%}%>
</table>
  </form>
</div>
  </body>
<%@ include file="/views/jump/admin/inc/footer.jsp"%>
</html>