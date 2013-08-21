<%@page pageEncoding="utf-8"%><!doctype html>
<html>
<head>
<meta charset="utf-8" />
<title>豆瓣资料导入</title>
</head>
<body>
<h1>豆瓣资料导入</h1>
<form action="http://wiki.renren.com/adminwiki/set-wiki-info" method="post">wikiId:<input name="wikiId" value="${wikiId }" /><br />
<textarea name="info" style="width: 600px; height: 400px;">${info }</textarea><br />
<input type="submit" value="确定" /></form>
</body>
</html>