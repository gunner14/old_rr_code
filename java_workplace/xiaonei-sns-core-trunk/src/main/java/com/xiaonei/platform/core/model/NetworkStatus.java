package com.xiaonei.platform.core.model;
public class NetworkStatus {
	//network_history.status  -1：当前 0：通过 1：刚填待审核 2：不通过 3：退出 4：资料不完备 5:高中申请信息不完备 6:高中未用注册码 7:海外大学信息不完备状态，即待收信 8:没有我的高中(网络)
	public final static int CURRENT_NETWORK = -1;
	public final static int PASS_NETWORK = 0;
	public final static int WANNA_AUDIT_NETWORK = 1;
	public final static int NO_PASS_NETWORK = 2;
	public final static int EXIT_NETWORK = 3;
	public final static int NOT_COMPLETE_INFO_NETWORK = 4;
	public final static int HS_NOT_COMPLETE_INFO_NETWORK = 5;
	public final static int HS_NOT_VERIFY_CODE = 6;
	public final static int ABOARD_NOT_COMPLETE = 7;
	public final static int NO_MY_HIGHSCHOOL = 8;
	
	public final static int SECOND_THREE_MONTH = 90 * 24 * 60 * 60;  //三个月的秒数
	public final static int START_FOREIGNUNIV_ID = 100000;  //国外大学的ＩＤ起始
	
	public final static int IS_OLD_USER = 1;
	
	public final static int MAX_NETWORK_CNT = 5;
	public final static int HS_NETWORK_CNT = 2;
	public final static int UNIV_NETWORK_CNT = 2;
}
