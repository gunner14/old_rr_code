<?
date_default_timezone_set('Europe/Berlin');
$date = '';
$days = 5;
if(isset($_GET['date'])){
	$date = $_GET["date"];
}

if(isset($_GET['days'])){
	$days = $_GET["days"];
}
 
if($date == ''){
	$date=date("Y-m-d");
}
?>
<html>
<head>
<style type="text/css"> 
*{font-family:Tahoma, Arial, Helvetica, Sans-serif,"宋体";}
body{
	font-size:12px;
}
table{
	margin:0px 0px;
	font:Georgia 11px;
	font-size:12px;
	color:#333333;
	border-collapse:collapse;/*细线表格代码*/
}
table td{
	border:1px solid #999;/*细线表格线条颜色*/
	height:22px;
}
caption{font-size:12px;font-weight:bold;margin:0 auto;}
tr.t1 td {background-color:#fff;}
tr.t2 td {background-color:#eee;}
tr.t3 td {background-color:#ccc;}
th,tfoot tr td{font-weight:bold;background:#c5c5c5;}
th{line-height:30px;height:30px;}
tfoot tr td{background:#fff;line-height:26px;height:26px;}
thead{border:1px solid #999;}
thead tr td{}
#page a,#page a:visited{width:60px;height:22px;line-height:22px;border:1px black solid;display:block;float:left;margin:0 3px;background:#c9c9c9;
text-decoration:none;}
#page a:hover{background:#c1c1c1;text-decoration:none;}
.grayr {padding:2px;font-size:11px;background:#fff;float:right;}
.grayr a {padding:2px 5px;margin:2px;color:#000;text-decoration:none;;border:1px #c0c0c0 solid;}
.grayr a:hover {color:#000;border:1px orange solid;}
.grayr a:active {color:#000;background: #99ffff}
.grayr span.current {padding:2px 5px;font-weight:bold; margin:2px; color: #303030;background:#fff;border:1px orange solid;}
.grayr span.disabled {padding:2px 5px;margin:2px;color:#797979;background: #c1c1c1;border:1px #c1c1c1 solid;}
</style>
</head>
	<!--
<form>
date:<input name="date" type="text" value="<?php echo $date ?>"/>
<input type="submit" value="提交日期" />
</form>
-->

<table>

	<tr>
		<td>日期</td>
		<td>引导流程总转化率</td>
		<td>加好友比例(进入引导为基数)</td>
		<td>加好友比例(进入GUIDE为基数)</td>
		<td>平均好友申请数</td>
		<td>fillinfo转化率</td>
		<td>preview转化率</td>
		<td>morefriend转化率</td>
		<td>import转化率</td>
		<td>portrait转化率</td>
		<td>非当天转化率</td>
		<td>fillinfo到达人数</td>
		<td>preview到达人数</td>
		<td>morefriend到达人数</td>
		<td>import到达人数</td>
		<td>potrait到达人数</td>
		<td>当天过完引导人数</td>
		<td>fillinfo_加好友人数</td>
		<td>fillinfo_加好友比例</td>
		<td>fillinfo_平均好友申请数</td>
		<td>preview_加好友人数</td>
		<td>preview_加好友比例</td>
		<td>preview_平均好友申请数</td>
		<td>morefriend_加好友人数</td>
		<td>morefriend_加好友比例</td>
		<td>morefriend_平均好友申请数</td>		
	</tr>
		<?
		
		for($cnt = 0;$cnt <= $days ;$cnt++){
			echo "<tr>";
			q_one_day($date,$shows,$sql_head,$sqls);
			$time = strtotime($date) - 60*60*24;
	   		$date = date('Y-m-d',$time);
	   		echo "</tr>";
		}
		
		?>
</table>




</html>
	
	
<?
function q_one_day($date){
	$sql_head = "select count(*) from guide_log where user_type='AAF' and fillinfo_arrive_isornot = 1 and insert_time >= '$date  00:00:00' and insert_time < '$date  23:59:59' ";
	$sqls = array(
		$sql_head."and fillinfo_arrive_isornot = 1", //到达11
		$sql_head."and fillinfo_arrive_isornot = 1 and fillinfo_action_addfriend = 1", //到达11又加好友
		"select sum(fillinfo_action_addfriend_count) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1", //11加人总数
		$sql_head."and preview_result_findfrind_sum >= 1 " ,//到达preview
		$sql_head."and preview_result_findfrind_sum >= 1 and morefriend_arrive_isornot =  1" ,//到达preview
		"select sum(preview_result_findfrind_sum) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1", //21推人总数
		$sql_head."and morefriend_result_findfrind_sum >= 1 " ,//到达mf
		$sql_head."and importmsn_arrive_isornot = 1 " ,//到达im
		$sql_head."and portrait_arrive_isornot = 1 " ,//到达po
		$sql_head."and leadend_arrive_isornot = 1 " ,//引导过完
		"select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1) and (preview_result_findfrind_sum>=1 or morefriend_result_findfrind_sum>=1 or importmsn_arrive_isornot=1 or portrait_arrive_isornot=1)",
		//11
		"select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and preview_result_findfrind_sum >= 1) and ( morefriend_result_findfrind_sum>=1 or importmsn_arrive_isornot=1 or portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
		"select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and morefriend_result_findfrind_sum>=1) and ( importmsn_arrive_isornot=1 or portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
		"select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and portrait_arrive_isornot=1) and ( leadend_arrive_isornot=1);",
		"select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and importmsn_arrive_isornot=1) and (  portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
		$sql_head."and leadend_arrive_isornot = 1 and last_active_time <= '$date 23:59:59'" ,//当天引导过完
		"select sum(preview_action_addfriend_count) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1", //preview_action_addfriend_count 加人总数
		"select sum(morefriend_action_addfriend_count) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1", //preview_action_addfriend_count 加人总数
		"select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and preview_action_addfriend_count>=1)",
		"select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and morefriend_action_addfriend_count>=1)",
		//20
		"select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and (morefriend_action_addfriend_count>=1 or fillinfo_action_addfriend_count>=1 or preview_action_addfriend_count>=1));",
						
		"select avg(TIMESTAMPDIFF(SECOND,insert_time, last_active_time)) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and insert_time>0 and last_active_time>0 and fillinfo_arrive_isornot=1 and fillinfo_arrive_isornot and  leadend_arrive_isornot=1 and user_type='AAF' limit 10;",
	);

	$data = array();
	for($i=0;$i<count($sqls);$i++){ 
		$rst = do_query($sqls[$i]);
		$data[$i] = $rst;
	}
	
	$rst = do_query($sqls[$i]);
	$data[$i] = $rst;
	
	$shows = array(
		"Date"=> $date,
		"引导流程总转化率"=> my_round(($data[9] / $data[0]),3),
		"进入引导为基数"=> my_round(($data[20] / $data[0]),3),
		"进入GUIDE为基数"=> my_round(($data[20] / $data[9]),3),
		"平均好友申请数"=> round((($data[2]+$data[16]+$data[17]) / $data[20]),3),
		"fillinfo转化率"=>my_round(($data[10] / $data[0]),3),
		"preview转化率"=>my_round(($data[11] / $data[3]),3),
		"morefriend转化率"=>my_round(($data[12] / $data[6]),3),
		"portrait转化率"=>my_round(($data[13] / $data[8]),3),
		"import转化率"=>my_round(($data[14] / $data[7]),3),
		"非当天转化率"=> my_round((($data[9] -$data[15])/ $data[0]),3),
		"fillinfo到达人数" =>$data[0],
		"preview到达人数"=> $data[3] ,
		"morefriend到达人数"=> $data[6],
		"import到达人数"=> $data[7],
		"portrait到达人数"=> $data[8],
		"当天过完引导人数"=> $data[15],
		"fillinfo_加好友人数"=>$data[1],
		"fillinfo_加好友比例"=> my_round(($data[1] / $data[0]),3),
		"fillinfo_平均好友申请数"=> round(($data[2] / $data[1]),3),
		"preview_加好友人数"=> $data[18],
		"preview_加好友比例"=> my_round(($data[18] / $data[3]),3),
		"preview_平均好友申请数"=> round(($data[16] / $data[18]),3),
		"morefriend_加好友人数"=> $data[19],
		"morefriend_加好友比例"=> my_round(($data[19] / $data[6]),3),
		"morefriend_平均好友申请数"=> round(($data[17] / $data[19]),3),		
	);

	foreach ($shows as $k=>$v){
		echo "<td>".$v."</td>";
	}
	
}

function my_round($data,$d){
	return  round($data*100,2)."%";
}
function do_query($sql){
		$mysql_server_name="10.3.18.45"; //数据库服务器名称
        $mysql_username="myuser"; // 连接数据库用户名
        $mysql_password="12345"; // 连接数据库密码
        $mysql_database="inshion_db"; // 数据库的名字

        // 连接到数据库
        $conn=mysql_connect($mysql_server_name, $mysql_username,
        $mysql_password);

        // 从表中提取信息的sql语句
        $strsql=$sql;//"select * from guide_log limit 10";
		
		
        // 执行sql查询
        $result=mysql_db_query($mysql_database, $strsql, $conn);
        // 获取查询结果
        $row=mysql_fetch_row($result);

        if(mysql_num_rows($result) == 0){//判断
            $ctt = "-1";
        }
        else{
			//$ctt = $row[3]." -- by [".$row[2]."@".$row[1]."]";
			$ctt = $row[0];
			//echo $ctt;
		}
		return $ctt;
        
        // 释放资源
		mysql_free_result($result);
		// 关闭连接
		mysql_close();
}
?>
