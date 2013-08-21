<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="true">
  <head>
	<%@ include file="/views/jump/admin/inc/header.jsp"%>
    <title>[Management] - [Modify]</title>
    
  </head>
  
  <body>
  <%@ include file="/views/jump/admin/inc/top.jsp"%>
  <form  method="post" action="/admin/jumpmanage">
  <input type="hidden" name="theAction" value="set_save" />
 
    <table width="100%">
    <tr class="td2">
	    <td>id</td>
        <td>ss_id</td>
        <td>rt_id</td>
        <td>game_stat_id</td>
        <td>code_id</td>
        <td>remark</td>
        <td>oper</td>
    </tr>

	<c:forEach var="item" items="${modellist}" begin="0" step="1"
		varStatus="i">
		<c:if test="${item.id == theId}">
			<tr class="td${i.index mod 2 + 3}">
				<td><input type="hidden" name="id" value="${item.id}" />
				<input type="hidden" name="x_id" value="${item.x_id}" class="smallInput" />
				<input type="hidden" name="y_id" value="${item.y_id}" class="smallInput" />
				<input type="hidden" name="to_url" value="${item.to_url}" class="smallInput" />
				${item.id} <br>
				</td>
				<td><input type="text" name="ss_id" value="${item.ss_id}"
					class="smallInput" /><br />
				</td>
				<td><input type="text" name="rt_id" value="${item.rt_id}"
					class="smallInput" /><br />
				</td>
				<td><input type="text" name="game_stat_id"
					value="${item.game_stat_id}" class="smallInput" /><br />
				</td>
				<td><input type="text" name="code_id" value="${item.code_id}"
					class="smallInput" /><br />
				</td>
				<td><input type="text" name="remark" value="${item.remark}"
					class="smallInput" /><br />
				</td>
				<td><input type="submit" name="提交" value="提交" /><a
					href="/admin/jumpmanage">cancel</a></td>
			</tr>
		</c:if>
		<c:if test="${item.id != theId}">
			<tr class="td${i.index mod 2 + 3}">
				<td>${item.id}</td>
				<td>${item.ss_id}</td>
				<td>${item.rt_id}</td>
				<td>${item.game_stat_id}</td>
				<td>${item.code_id}</td>
				<td>${item.remark}</td>
				<td>${item.update_time}</td>
			</tr>
		</c:if>

	</c:forEach>

	<tr>
    	<td>
	    <br></td>
    </tr>
    </table>
  </form>        
	
  </body>
<%@ include file="/views/jump/admin/inc/footer.jsp"%>  
</html>
