<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<link href="/css/new-style.css" rel="stylesheet" type="text/css"></link>
<script src="/jquery.js" type="text/javascript"></script>
<title>模板白名单管理</title>
<script type="text/javascript">
var g_user_right = ${user_right};
var g_stype = ${stype};

if (g_user_right <= 0) {
  var f = window.location.href;
  window.location = 'https://passport.no.opi-corp.com/login.php?forward=' + escape(f);
}
</script>
</head>
<body>
<center><h1><font size="+2">该类型所有白名单</font></h1><br />
<p style="color:red;"><font size="+1">提示：所有的模版都加载到了测试机中了，线上模版和预发布模版是不可以编辑的，<br />如果你想测试一个模板，只需要将你的人人帐号id设置为该模板的白名单，<br />如果你不设置，那将采用默认的模板（预发布模板）</font></p>
<table border="1">
<thead>
  <tr>
      <th>类型号</th>
      <th>版本号</th>
      <th>模板号</th>
      <th>用户ID</th>
      <th>删除</th>
  </tr>
</thead>
<tbody id="whitelist-table"><tr><td colspan="5" align="center">类型白名单如下</td></tr></tbody>
<tfoot>
    <tr>
      <td colspan='6' align='center'>有任何疑问请联系renren.feed@renren-inc.com</td>
    </tr>
</tfoot>
</table>
</center>
</body>
<script>
$(document).ready(
  function() {
    $.ajax('/get-whitelist?stype='+g_stype, {
      'success' : function(text){
        var tv = $.parseJSON(text);
        for(var i = 0; i < tv.length; ++i) {
          var o = tv[i];
          var html = '<tr><td>' + (o.stype) + '</td>'
                   + '<td>' + (o.version) + '</td>'
                   + '<td>' + (o.tpl_id) + '</td>'
                   + '<td>' + (o.whiteid) + '</td>'
                   + '<td><a id="remove_whiteid" href="#nogo" >删除</a></td>';
           html += '</tr>';
           var node = $(html);
           $('#remove_whiteid', node).click(
             function() {
               return function() {
                 if (!confirm('确定要删除白名单 ' + o.whiteid + ' 吗?')) {
                   return;
                 }
                 $.ajax('/del-whiteid?stype='+(o.stype)+'&version='+(o.version)+'&tpl_id='+(o.tpl_id)+'&whiteid='+(o.whiteid), {
                   success : function(text) {
                     alert(text);
                   },
                   error : function() {
                     alert('删除白名单' + (o.whiteid) + ' 失败');
                   }
                 });
               };
             }()
           );
           $('#whitelist-table').append(node);
        }
      },
      'error' : function(){
        alert('加载出错');
      }
    });
  }
);
</script>

</html>
