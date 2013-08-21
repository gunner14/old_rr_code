package com.xiaonei.reg.guide.root.configs;

public interface IStepStatusDefinition {
	/** 完善资料 2 */
	public static final int STEP_FILLINFO 					= Integer.valueOf("10", 2); // 完善资料
	/** 上传头像 4 */
	public static final int STEP_UPLOAD_HEAD 				= Integer.valueOf("100", 2); // 上传头像
	/** 导入MSN 8 */
	public static final int STEP_IMPORT_MSN 				= Integer.valueOf("1000", 2); // 导入MSN
	/** 正式开始页 16 */
	public static final int STEP_PREVIEW 					= Integer.valueOf("10000", 2); // 正式开始页
	/** guide页 32 */
	public static final int STEP_GUIDE 						= Integer.valueOf("100000", 2);
	/** 手机注册，fillinfo 64 */
	public static final int STEP_FILLINFO_MOBILE 			= Integer.valueOf("1000000", 2);
	/** 手机注册，邮箱验证页 128 */
	public static final int STEP_VERIFY_MOBILE 				= Integer.valueOf("10000000", 2);
	//inshion 2009-9-4 本来Flash引导中要多加一步的，后来去掉，就一直没用了
	//2009-11-21 12:56:03
	public static final int STEP_LANDING_PAGE				= Integer.valueOf("100000000", 2); //256
	/** 推好友的好友512*/
	public static final int STEP_MOREFRIEND 				= Integer.valueOf("1000000000", 2); //512
	public static final int STEP_SEARCH_FRIEND				= Integer.valueOf("10000000000", 2); //1024
	//public static final int STEP_FILLINFO_STAGE			= Integer.valueOf("100000000000", 2); //2048
	/** 引导完成标志（不含guide页） 4096 */
	public static final int STEP_COMPLETE_GUIDE 			= Integer.valueOf("1000000000000", 2);
	/** 4399完善资料 8192 */
	public static final int STEP_4399_FILLINFO 				= Integer.valueOf("10000000000000", 2);
	/** 手机注册,fillinfo 完善基本资料 16384 */
	public static final int STEP_FILLINFO_STAGE_MOBILE 		= Integer.valueOf("100000000000000", 2);
	/** 狗等APP 32768 */
	public static final int STEP_APP						= Integer.valueOf("1000000000000000", 2);
	/** FLASH流程入口 65536 */
	public static final int STEP_FLASH_APP_OPEN 			= Integer.valueOf("10000000000000000", 2);
	
	public static final String STR_STEP_FILLINFO = "STEP_FILLINFO";
	public static final String STR_STEP_UPLOAD_HEAD = "STEP_UPLOAD_HEAD";
	public static final String STR_STEP_IMPORT_MSN = "STEP_IMPORT_MSN";
	public static final String STR_STEP_PREVIEW = "STEP_PREVIEW";
	public static final String STR_STEP_GUIDE = "STEP_GUIDE";
	public static final String STR_STEP_FILLINFO_MOBILE = "STEP_FILLINFO_MOBILE";
	public static final String STR_STEP_VERIFY_MOBILE = "STEP_VERIFY_MOBILE";
	public static final String STR_STEP_COMPLETE_GUIDE = "STEP_COMPLETE_GUIDE";
	public static final String STR_STEP_4399_FILLINFO = "STEP_4399_FILLINFO";
	public static final String STR_STEP_FILLINFO_STAGE_MOBILE = "STEP_FILLINFO_STAGE_MOBILE";
	public static final String STR_STEP_APP = "STEP_APP";
	public static final String STR_STEP_FLASH_APP_OPEN = "STEP_FLASH_APP_OPEN";
	
	
}
