<html>
<head>
  <meta charset="utf-8" />	
	<script type="text/javascript" src="static/js/calendar.js"></script>
	<script type="text/javascript" src="static/js/jquery.js"></script>
  <title>通用Cache Report Beta</title>
  <style>
		body{
			font-size:9pt;
			font-family:Arial;
		}
		*{
		text-align:left;
		}		
		input {border:1px solid #ABABAB}
	</style>
</head>
<body>
  <h3>业务调用查询</h3>
  <form action="/biz_query" method="post">
    <p>businessId
    <select name="biz_id">
      {% for item in biz_items %}
      <option value= {{item}} > {{item}} </option>
      {% end %}
    </select>
    </p>
    <p>开始日期(YYYY-MM-DD-HH):<input type="text" name="begin_time_biz"></p>
    <p>结束日期(YYYY-MM-DD-HH):<input type="text" name="end_time_biz"></p>
    <input type="submit" name="btn1" value="提交"/>
  </form>
  <h3>Redis缓存查询</h3>
  <form action="/cache_query" method="post">
    <p>机器
    <select name="machine">
      {% for item in machine_items %}
      <option value= {{item}} > {{item}} </option>
      {% end %}
    </select>
    </p>
    <p>开始日期(YYYY-MM-DD-HH):<input type="text" name="begin_time_cache"></p>
    <p>结束日期(YYYY-MM-DD-HH):<input type="text" name="end_time_cache"></p>
    <input type="submit" name="btn1" value="提交"/>
  </form>
	
</body>
</html>
