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
  <h3>缓存查询</h3>
  <form action="/cache_query" method="post">
        <p>机器
    <select name="machine">
      <option value= {{ selected_cc }} selected="true">  {{ selected_cc }}  </option>       
      {% for item in machine_items %}
      <option value= {{item}} > {{item}} </option>
      {% end %}
    </select>
    </p>
        <p>开始日期(YYYY-MM-DD-HH):<input type="text" name="begin_time_cache" value={{ begin_time }} ></p>
        <p>结束日期(YYYY-MM-DD-HH):<input type="text" name="end_time_cache" value={{ end_time }} ></p>
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
   			 <td>used_memory_rss</td>
   			 {% for item in items %}
           <td>{{ item['used_memory_rss'] }}</td>
         {% end %}
   		 </tr>
   		 <tr>
   			 <td>total</td>
   			 {% for item in items %}
           <td>{{ item['total'] }}</td>
         {% end %}
   		 </tr>
   		 <tr>
   			 <td>hit</td>
   			 {% for item in items %}
           <td>{{ item['hit'] }}</td>
         {% end %}
   		 </tr>
   		 <tr>
   			 <td>miss</td>
   			 {% for item in items %}
           <td>{{ item['miss'] }}</td>
         {% end %}
   		 </tr>
     </tbody>
   </table>
	</div>
	<img src="{{pic_path_memory}}"/>
	<img src="{{pic_path_command}}"/>
</body>
</html>
