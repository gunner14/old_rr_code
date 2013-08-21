package com.xiaonei.xnmonitor.utils;

public class Global {

	public final static String HTTP_DSNAME = "HTTP_LOG";// http分析以后的DSName

	 public final static String JROBIN_RESULT_DIR =
	 "/data/xiaonei-monitor/jrobin-result/";//http分析以后的rrd文件的存放位置

	// public final static String JROBIN_RESULT_DIR =
	// "/home/lightning/Logs/jrobin/";//http分析以后的rrd文件的存放位置

	//	
	// public final static String HTTP_DSNAME="HTTP_LOG";//http分析以后的DSName
	// public final static String
	// HTTP_JROBIN_PATH="/home/lightning/Logs2/jrobin_testSkip/";//http分析以后的rrd文件的存放位置

//	public final static String JROBIN_RESULT_DIR = "/home/lightning/JrobinResult/";

	/**
	 * JRD=JROBIN_RESULT_DIR httplog 分析后的结果存放在JROBIN结果目录中的子目录
	 */
	public final static String JRD_CHILD_DIR_HTTP = "httplog";

	/**
	 * JRD=JROBIN_RESULT_DIR DBLOG 分析后的结果存放在JROBIN结果目录中的子目录。
	 */
	public final static String JRD_CHILD_DIR_DB = "dblog";

	/**
	 * JRD=JROBIN_RESULT_DIR HTTP404/500日志 分析后的结果存放在JROBIN结果目录中的子目录。
	 */
	public final static String JRD_CHILD_DIR_HTTPERR = "httperrlog";

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
