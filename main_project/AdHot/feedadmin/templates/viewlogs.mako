<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<link href="/css/new-style.css" rel="stylesheet" type="text/css"></link>
<script src="/jquery.js" type="text/javascript"></script>
<title>字段编辑 - 编辑LOG查看</title>
<script type="text/javascript">
var g_user_right = ${user_right};

if (g_user_right <= 0) {
  var f = window.location.href;
  window.location = 'https://passport.no.opi-corp.com/login.php?forward=' + escape(f);
}
</script>
<script type="text/javascript">
$(document).ready(function(){
  $('#logDiv').load('/viewlogs/${stype}.txt');
});
</script>
</head>
<body>
<center><h1>查看最近修改记录</h1><br />
<table border="1">
<thead>
  <tr>
      <th>时间</th>
      <th>用户</th>
      <th>操作记录</th>
      <th>版本号</th>
      <th>模板号</th>
      <th>View</th>
  </tr>
</thead>
<tbody id="logDiv"></tbody>
<tfoot>
    <tr>
      <td colspan='6' align='center'>对于log有疑问请联系renren.feed@renren-inc.com</td>
    </tr>
</tfoot>
</table>
</center>
</body>
</html>
