<%@ page pageEncoding="UTF-8" %>
<div class="personIcon">
	<table>
		<tr>
			<td><a href="${applicationScope.urlMain}/profile.do?portal=${StatPortal}&id=${usr.id}"><img src="${usr.tiny_Url}" alt="${usr.name}" /></a></td>
		</tr>
	</table>
</div>
<div class="personName"><a href="${applicationScope.urlMain}/profile.do?portal=${StatPortal}&id=${usr.id}">${usr.name}</a></div>
