package com.xiaonei.tools.monitor;

public class Global {

	/**
	 * 线上正在使用的
	 */
	/**
	 * 取HttpLog来分析的log路径
	 */

	public final static String HTTPLOG_DIR = "/data/XceLogger/log/XceLog/logger/com/xiaonei/profiler/";

	/**
	 * /* HTTPLog分析结束后的存储路径
	 */
	 public final static String HTTPLOG_BACK =
	 "/data/XceLogger/log/XceLog/logger/com/xiaonei/profiler/httplogbak/";

	/**
	 * 日志包括http，db，http404/500的jrobin分析结果jrobin存方的路径不同的日志对应不同的子路径
	 */
	 public final static String JROBIN_RESULT_DIR =
	 "/data/xiaonei-monitor/jrobin-result/";
	
	 public final static String DBLOG_DIR =
	 "/data/XceDBLog/log/XceLog/logger/com/xiaonei/platform/core/opt/";
	
	 public final static String DBLOG_BACK =
	 "/data/XceDBLog/log/XceLog/logger/com/xiaonei/platform/core/opt/dbbak/";
	
	 public final static String HTTPERRLOG_DIR = "/data/httperror/logs/";
	
	 public final static String HTTPERRLOG_BACK =
	 "/data/httperror/logs/httperrbak/";
	
	 public final static String IMLOG_DIR = "/data/httperror/im/";
	
	 public final static String IMLOGONLYDNS_DIR = "/data/httperror/im_1/";
	
	 public final static String IMLOG_BACK = "/data/httperror/im/imbak/";
	
	 public final static String IMLOGONLYDNS_BACK =
	 "/data/httperror/im_1/imbak/";

	/**
	 * 本机开发测试的
	 */
	/* HttpLog 分析路径 */
//	public final static String HTTPLOG_DIR = "/home/lightning/HttpLog/";
//
//	public final static String HTTPLOG_BACK = "/home/lightning/HttpLog/httpbak/";
//
//	public final static String JROBIN_RESULT_DIR = "/home/lightning/JrobinResult/";
//
//	public final static String DBLOG_DIR = "/home/lightning/DBLog/";
//
//	public final static String DBLOG_BACK = "/home/lightning/DBLog/dbbak/";
//
//	public final static String HTTPERRLOG_DIR = "/home/lightning/HttpErrLog/";
//	public final static String HTTPERRLOG_BACK = "/home/lightning/HttpErrLog/httperrbak/";
//
//	//
//	public final static String IMLOG_DIR = "/home/lightning/IMErrLog/im/";
//
//	public final static String IMLOGONLYDNS_DIR = "/home/lightning/IMErrLog/im_1/";
//
//	public final static String IMLOG_BACK = "/home/lightning/IMErrLog/im/imbak/";
//
//	public final static String IMLOGONLYDNS_BACK = "/home/lightning/IMErrLog/im_1/imbak/";

	/**
	 * JRD=JRD_CHILD_DIR_HTTP httplog 分析后的结果存放在JROBIN结果目录中的子目录 千万别改动
	 */
	public final static String JRD_CHILD_DIR_HTTP = "httplog";

	/**
	 * JRD=JRD_CHILD_DIR_DB DBLOG 分析后的结果存放在JROBIN结果目录中的子目录。 千万别改动
	 */
	public final static String JRD_CHILD_DIR_DB = "dblog";

	/**
	 * JRD=JRD_CHILD_DIR_HTTPERR HTTP404/500日志 分析后的结果存放在JROBIN结果目录中的子目录。
	 */
	public final static String JRD_CHILD_DIR_HTTPERR = "httperrlog";

	/**
	 * JRD=JRD_CHILD_DIR_IM IM分析后的结果存放在JROBIN结果目录中的子目录。
	 */
	public final static String JRD_CHILD_DIR_IM = "imlog";

	/**
	 * 对于HTTPLOG分析结果的dsname 为HTTP_OG
	 */
	public final static String DSNAME_HTTP = "HTTP_LOG";

	/**
	 * 对于DBLOG分析结果的dsname 为HTTP_OG
	 */
	public final static String DSNAME_DB = "DB_LOG";

	/**
	 * 对于HTTPERRLOG分析结果的dsname 为HTTP_OG
	 */
	public final static String DSNAME_HTTPERR = "HTTPERR_LOG";

	/**
	 * 对于HTTPERRLOG分析结果的dsname 为HTTP_OG
	 */
	public final static String DSNAME_IM = "IM_LOG";

	/**
	 * 对于HTTPLOG分析结果的dsTYPE 为GAUGE
	 */
	public final static String DSTYPE_HTTP = "GAUGE";

	/**
	 * 对于DBLOG分析结果的dsTYPE 为GAUGE
	 */
	public final static String DSTYPE_DB = "GAUGE";

	/**
	 * 对于HTTPERRLOG分析结果的dsTYPE 为GAUGE
	 */
	public final static String DSTYPE_HTTPERR = "COUNTER";

	/**
	 * 对于IMLOG分析结果的dsTYPE 为GAUGE
	 */
	public final static String DSTYPE_IM = "GAUGE";

}
