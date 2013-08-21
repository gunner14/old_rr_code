<%@ page contentType="text/html;charset=UTF-8" %>
<div class="list_item">
	<div class="container" id="${item.menuId}">		
		<a class="edit" href="${item.menuExtraUrl}">${item.menuExtraName}</a>
		<a class="link_title edit-title" href="${item.menuUrl}" style="background-image:url(${item.logoImgPath})">${item.menuName}</a>
	</div>
</div>