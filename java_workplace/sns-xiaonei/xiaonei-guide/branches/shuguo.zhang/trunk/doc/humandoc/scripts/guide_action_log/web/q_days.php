<?
date_default_timezone_set('Europe/Berlin');
$date = '';
if(isset($_GET['date'])){
	$date = $_GET["date"];
}

if($date == ''){
	$date=date("Y-m-d");
}

?>
<html>
<head>
<link href="qstyle.css" rel="stylesheet" type="text/css" />
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
<!-- form>
date:<input name="date" type="text" value="<?php echo $date ?>"/>
<input type="submit" value="提交日期" />
</form -->
Guide Log Daily Report
<?
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
  "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and preview_result_findfrind_sum >= 1) and ( morefriend_result_findfrind_sum>=1 or importmsn_arrive_isornot=1 or portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
  "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and morefriend_result_findfrind_sum>=1) and ( importmsn_arrive_isornot=1 or portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
  "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and portrait_arrive_isornot=1) and ( leadend_arrive_isornot=1);",
  "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and importmsn_arrive_isornot=1) and (  portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
  $sql_head."and leadend_arrive_isornot = 1 and last_active_time <= '$date 23:59:59'" ,//当天引导过完
  "select sum(preview_action_addfriend_count) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1", //preview_action_addfriend_count 加人总数
   "select sum(morefriend_action_addfriend_count) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1", //preview_action_addfriend_count 加人总数
   "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and preview_action_addfriend_count>=1)",
   "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and morefriend_action_addfriend_count>=1)",
  "select  avg(TIMESTAMPDIFF(SECOND,insert_time, last_active_time)) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and insert_time>0 and last_active_time>0 and fillinfo_arrive_isornot=1 and fillinfo_arrive_isornot and  leadend_arrive_isornot=1 and user_type='AAF' limit 10;",
);

$data = array();
$data_desc = array(
"到达fillinfo", //0
"在fillinfo加好友", //1
"在fillinfo加好友总量", //2
"到达preview", //3
"到过preview且到过morefriend", //4
"在preview推人总量", //5
"到达morefriend", //6
"到达import", //7
"到达potrait", //8
"引导过完", //9
"fillinfo转化人数", //10
"preview转化人数", //11
"morefriend转化人数", //12
"portrait转化人数", //13
"import转化人数", //14
"当天过完引导人数", //15
"preview加人总量", //16
"morefriend加人总量", //17
"preview加人", //18
"morefriend加人", //19
"", //
);

$shows = array(
"过完引导比率"=> round(($data[9] / $data[0]),3),
"fillinfo转化率"=>round(($data[10] / $data[0]),3),
"preview转化率"=>round(($data[11] / $data[3]),3),
"morefriend转化率"=>round(($data[12] / $data[6]),3),
"portrait转化率"=>round(($data[13] / $data[8]),3),
"import转化率"=>round(($data[14] / $data[7]),3),
"非当天转化率"=> round((($data[9] -$data[15])/ $data[0]),3),
"p11_到达" =>$data[0],
"p11_加好友人数"=> $data[1],
"p11_加好友比例"=> round(($data[1] / $data[0]),3),
"p11_平均好友申请数"=> round(($data[2] / $data[1]),3),
"p21_到达"=> $data[3] ,
"p21_加好友人数"=> $data[18],
"p21_加好友比例"=> round(($data[18] / $data[3]),3),
"p21_平均好友申请数"=> round(($data[16] / $data[18]),3),
"p22_到达"=> $data[6],
"p22_加好友人数"=> $data[19],
"p22_加好友比例"=> round(($data[19] / $data[3]),3),
"p22_平均好友申请数"=> round(($data[17] / $data[19]),3),
);


?>

<?
$table_2 = "";
echo "<hr />";

   //$date = "2006-01-01";

  	echo "<table style=margin-left:5px;margin-right:5px><tr>";
	for($i=0;$i<count($sqls);$i++){ 
		echo "<td>$data_desc[$i]</td>";
	}
	for($cnt = 0;$cnt <= 5 ;$cnt++){
		$table_2 = $table_2.q_one_day($date,$shows,$sql_head,$sqls);
		$time = strtotime($date) - 60*60*24;
   		$date = date('Y-m-d',$time);
	}
	echo "</tr></table>";


echo "<hr />";
echo "<table style=margin-left:5px;margin-right:5px>";
	echo "<tr>";
	foreach ($shows as $k=>$v){
		//$ret = $ret."<tr>";
		echo "<td>$k</td>";
		//$ret = $ret."</tr>";
	}
	echo "</tr>";
echo $table_2;
echo "<table>";


?>
	
</html>	
<?
function q_one_day($date,$shows,$sql_head,$sqls){
	 
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
  "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and preview_result_findfrind_sum >= 1) and ( morefriend_result_findfrind_sum>=1 or importmsn_arrive_isornot=1 or portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
  "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and morefriend_result_findfrind_sum>=1) and ( importmsn_arrive_isornot=1 or portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
  "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and portrait_arrive_isornot=1) and ( leadend_arrive_isornot=1);",
  "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot=1 and importmsn_arrive_isornot=1) and (  portrait_arrive_isornot=1 or leadend_arrive_isornot=1);",
  $sql_head."and leadend_arrive_isornot = 1 and last_active_time <= '$date 23:59:59'" ,//当天引导过完
  "select sum(preview_action_addfriend_count) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1", //preview_action_addfriend_count 加人总数
   "select sum(morefriend_action_addfriend_count) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and fillinfo_arrive_isornot = 1", //preview_action_addfriend_count 加人总数
   "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and preview_action_addfriend_count>=1)",
   "select count(distinct user_id)  from guide_log where ( user_type = 'AAF' and insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and morefriend_action_addfriend_count>=1)",
  "select  avg(TIMESTAMPDIFF(SECOND,insert_time, last_active_time)) from guide_log where insert_time >= '$date 00:00:00' and insert_time < '$date 23:59:59' and insert_time>0 and last_active_time>0 and fillinfo_arrive_isornot=1 and fillinfo_arrive_isornot and  leadend_arrive_isornot=1 and user_type='AAF' limit 10;",
);
 
	 	$data = array();
			echo "<tr><td>$date</td>";
			for($i=0;$i<count($sqls);$i++){ 
				$rst = do_query($sqls[$i]);
				$data[$i] = $rst;
				echo " <td>".$rst."</td>";
			}
			echo "</tr>";
			
			$shows = array(
"过完引导比率"=> round(($data[9] / $data[0]),3),
"fillinfo转化率"=>round(($data[10] / $data[0]),3),
"preview转化率"=>round(($data[11] / $data[3]),3),
"morefriend转化率"=>round(($data[12] / $data[6]),3),
"portrait转化率"=>round(($data[13] / $data[8]),3),
"import转化率"=>round(($data[14] / $data[7]),3),
"非当天转化率"=> round((($data[9] -$data[15])/ $data[0]),3),
"p11_到达" =>$data[0],
"p11_加好友人数"=> $data[1],
"p11_加好友比例"=> round(($data[1] / $data[0]),3),
"p11_平均好友申请数"=> round(($data[2] / $data[1]),3),
"p21_到达"=> $data[3] ,
"p21_加好友人数"=> $data[18],
"p21_加好友比例"=> round(($data[18] / $data[3]),3),
"p21_平均好友申请数"=> round(($data[16] / $data[18]),3),
"p22_到达"=> $data[6],
"p22_加好友人数"=> $data[19],
"p22_加好友比例"=> round(($data[19] / $data[3]),3),
"p22_平均好友申请数"=> round(($data[17] / $data[19]),3),
);


			
			//echo "<table style=margin-left:5px;margin-right:5px>";
			$ret = "";
			$ret = $ret."<tr>";
			foreach ($shows as $k=>$v){
				$ret = $ret."<td>".$v."</td>";
				
			}
			$ret = $ret."</tr>";
			return $ret;
			//echo "</table>";


}

function show_static_table(){
	echo "<table style=margin-left:5px;margin-right:5px>
<tr><th>列名</th><th>表示</th></tr>
<tr><td> A</td><td>user_id</td><tr>
</tr><tr><td> B</td><td>nsert_time</td><tr>
</tr><tr><td> C</td><td>user_type</td><tr>
</tr><tr><td> D</td><td>fillinfo_arrive_isornot</td><tr>
</tr><tr><td> E</td><td> filinfo_arrive_fillinfo_memo</td><tr>
</tr><tr><td> F</td><td> filinfo_action_addfriend</td><tr>
</tr><tr><td> G</td><td>fillinfo_action_addfriend_count</td><tr>
</tr><tr><td> H</td><td>preview_arrive_isornot</td><tr>
</tr><tr><td> I</td><td> preview_result_findfrind_memo</td><tr>
</tr><tr><td> J</td><td> preview_result_findfrind_sum</td><tr>
</tr><tr><td> K</td><td>preview_result_findfrind_good</td><tr>
</tr><tr><td> L</td><td>morefriend_arrive_isornot</td><tr>
</tr><tr><td> M</td><td>morefriend_result_findfrind_memo</td><tr>
</tr><tr><td> N</td><td>morefriend_result_findfrind_sum</td><tr>
</tr><tr><td> O</td><td>morefriend_result_findfrind_good</td><tr>
</tr><tr><td> P</td><td>mportmsn_arrive_isornot</td><tr>
</tr><tr><td> Q</td><td>portrait_arrive_isornot</td><tr>
</tr><tr><td> R</td><td>leadend_arrive_isornot</td><tr>
</tr><tr><td> S</td><td>ast_active_time</tr>
</table>
";
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
