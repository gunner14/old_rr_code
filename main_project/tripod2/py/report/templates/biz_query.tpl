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
		table{
			border-collapse: collapse;padding:0;margin:0;border:1px solid #CEE1EE;
		}
		table td, table th{
			background-color: #FFFFFF;
	    border: 1px solid #CEE1EE;
	    padding: 8px;
	    text-align: center;
	    font-size:12px;
	   }
	  table th{
			background-color:#F0F5F8;
	  }
	  table td:hover{
			background-color:#F0F5F8;
			cursor:default;
	  }
	</style>
</head>
<body>
  <h3>业务调用查询</h3>
  <form action="/biz_query" method="post">
    <p>businessId
    <select name="biz_id">
      <option value= {{ selected_biz_id }} selected="true">  {{ selected_biz_id }}  </option>
      {% for item in biz_items %}
      <option value= {{item}} > {{item}} </option>
      {% end %}
    </select>
    </p>

	<p>开始日期(YYYY-MM-DD-HH):<input type="text" name="begin_time_biz" value={{ begin_time }}></p>
	<p>结束日期(YYYY-MM-DD-HH):<input type="text" name="end_time_biz" value={{ end_time }}></p>
	<input type="submit" name="btn1" value="提交"/>
  </form>
  <div>
   <table>
   	 <tbody>
   		 <tr>
   			 <th></th>
   			 {% for item in items %}
           <th>{{ item["time"] }}</th>
         {% end %}
   		 </tr>
   		 <tr>
   			 <td>get次数</td>
   			 {% for item in items %}
           <td>{{ item["get"] }}</td>
         {% end %}
   		 </tr>
   		 <tr>
   			 <td>set次数</td>
   			 {% for item in items %}
           <td>{{ item["set"] }}</td>
         {% end %}
   		 </tr>
   		 <tr>
   			 <td>load次数</td>
   			 {% for item in items %}
           <td>{{ item["load"] }}</td>
         {% end %}
   		 </tr>
   		 <tr>
   			 <td>remove次数</td>
   			 {% for item in items %}
           <td>{{ item["remove"] }}</td>
         {% end %}
   		 </tr>
     </tbody>
   </table>
	</div>
	<img src="{{pic_path}}"/>
</body>
</html>
