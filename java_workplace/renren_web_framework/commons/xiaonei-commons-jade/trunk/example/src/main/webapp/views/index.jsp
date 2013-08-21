<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<html>

<head>

<meta http-equiv="Content-Type" content="UTF-8" />

</head>

<body>

<h2>Hello Rose-Jade Example</h2>

<p>1） 首先在数据库中创建如下表结构：</p>

<pre>
 +------------------+--------------+------+-----+-------------------+-------+
 | Field            | Type         | Null | Key | Default           | Extra |
 +------------------+--------------+------+-----+-------------------+-------+
 | id               | int(11)      | NO   |     |                   |       |
 | user_id          | int(11)      | NO   | PRI |                   |       |
 | name             | varchar(40)  | NO   |     |                   |       |
 | phone_code       | varchar(255) | YES  |     | NULL              |       |
 | create_timestamp | timestamp    | NO   |     | CURRENT_TIMESTAMP |       |
 +------------------+--------------+------+-----+-------------------+-------+
 
 CREATE TABLE contact_info (
     id                   INTEGER NOT NULL PRIMARY KEY,
     user_id              INTEGER NOT NULL,
     name                 VARCHAR(40) NOT NULL,
     phone_code           VARCHAR(255),
     create_timestamp     TIMESTAMP NOT NULL
 );
</pre>

<p>2） 输入 userid 显示通讯录：</p>

<form method="GET" action="example/list" />
	用户 ID：<input type="text" name="userid" value="0" />
	<input type="submit" name="OK" value="显示" />
</form>

<p>3）显示日志：</p>

<form method="GET" action="example/list_log" />
	查询日期：<input type="text" name="timestamp" value="2009-10-23" />
	<input type="submit" name="OK" value="日志" />
</form>

</body>

</html>
