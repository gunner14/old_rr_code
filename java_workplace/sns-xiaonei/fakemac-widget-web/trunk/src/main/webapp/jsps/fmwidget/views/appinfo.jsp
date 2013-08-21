<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
hostid:${hostid}
<br />
[应用信息]<br />
<form action="/demo/app/set">
<table>
<tr>
    <td>
        <input type="hidden" name="id" value="${curdict.id}"/>
    </td>
</tr>
<tr>
    <td>
                  应用号:${curdict.app_id}</td><td><input type="text" name="app_id" value="${curdict.app_id}"/>
    </td>
</tr>
<tr>
    <td>
                  应用名:<font color=red>${curdict.app_name}</font></td><td><input type="text" name="app_name" value="${curdict.app_name}"/>
    </td>
</tr>
<tr>
    <td>
                  应用描:<font color=green>${curdict.app_description}</font></td><td><input type="text" name="app_description" value="${curdict.app_description}"/>
    </td>
</tr>
<tr>
    <td>
                  开发者:${curdict.app_owner}</td><td><input type="text" name="app_owner" value="${curdict.app_owner}"/>
    </td>
</tr>
</table>
<input type="submit" value="修改之"/>
</form>
<a href="/demo/home">返回</a>