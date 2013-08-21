package com.xiaonei.platform.storage;

public class Constants {

	public static final String STATE_SPACE = "Space";
	
	public static final String STATE_LOAD = "Load";

	public static final String STATE_TIME = "Time";

	public static int RANK_LEVEL_IDLE = 0;

	public static int RANK_LEVEL_NORMAL = 1;

	public static int RANK_LEVEL_WARN = 2;

	public static int RANK_LEVEL_FATAL = 3;

	public static int RANK_LEVEL_COUNT = 4;

	public static int TRAP_INTERVAL = 60*3;
	
	public static int NODE_ACTIVE_THRESHOLD = 60*8;
	
	public static double THRESHOLD_SPACEUSED = 0.90;
	
	public static double THRESHOLD_LOAD = 10;
	
	public static final String TABLE_STORAGE_NODE = "storage_node";//node 的数据库表名
		
	public static int NODE_STATUS_DISABLE = 0;// node 状态无效
	
	public static int NODE_STATUS_WRITABLE = 2;//node 状态能写
	
	public static int NODE_STATUS_READABLE = 1;//node 状态能读
	
	public static int NODE_STATUS_WRITABLE_READABLE=3; //node 状态即读又能写
	
	public static int NODE_TYPE_NORMAL = 1;
	
	public static int NODE_TYPE_HEAD = 2;
	
	public static int NODE_TYPE_CLASS = 3 ;
}
