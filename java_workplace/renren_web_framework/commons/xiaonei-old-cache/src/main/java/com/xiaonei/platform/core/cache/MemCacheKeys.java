package com.xiaonei.platform.core.cache;

public class MemCacheKeys {

	public static final String blog_new_userId_is_ = "blog_new_userId_is_";
	public static final String blog_auto_save = "blog_auto_save_";
	public static final String album = "album_";
	public static final String photo = "photo_";
	public static final String photo_edit_list = "photo_edit_list_";
	public static final String photo_edit = "photo_edit_";
	public static final String home = "home_";
	public static final String home_friends = "home_friends_";
	public static final String home_tribe = "home_tribe_";
	public static final String guest_tribe = "guest_tribe_";
	public static MemCacheKeys pool_default = new MemCacheKeys("default");
	public static MemCacheKeys pool_icode = new MemCacheKeys("pool_icode");
	public static MemCacheKeys pool_expday = new MemCacheKeys("default");
	public static MemCacheKeys pool_photo_upload = new MemCacheKeys("pool_photo_upload");
	public static MemCacheKeys pool_user_space = new MemCacheKeys("default");
	public static MemCacheKeys pool_guide = new MemCacheKeys("default");
	public static MemCacheKeys pool_tribe = new MemCacheKeys("default");
	public static MemCacheKeys pool_rand_code = new MemCacheKeys("pool_rand_code");
	public static MemCacheKeys pool_profile = new MemCacheKeys("default");
	public static MemCacheKeys pool_score = new MemCacheKeys("pool_score");

	public static MemCacheKeys pool_xnml_ref = new MemCacheKeys("pool_xnml_ref");
	public static MemCacheKeys pool_tyrant = new MemCacheKeys("tyrant");
	public static MemCacheKeys pool_apps = new MemCacheKeys("pool_apps");
	public static MemCacheKeys pool_tyrant_read = new MemCacheKeys("tyrant_read");
	public static MemCacheKeys pool_user_recent_apps = new MemCacheKeys("pool_user_recent_apps");
	public static MemCacheKeys pool_status_count = new MemCacheKeys("pool_status_count");
	public static MemCacheKeys pool_status = new MemCacheKeys("pool_status");
	public static MemCacheKeys pool_antirobot = new MemCacheKeys("pool_antirobot");
	public static MemCacheKeys pool_wap = new MemCacheKeys("pool_wap");
	public static MemCacheKeys pool_app_user = new MemCacheKeys("pool_app_user");
	public static MemCacheKeys pool_vip = new MemCacheKeys("pool_vip");
	public static MemCacheKeys pool_page = new MemCacheKeys("pool_page");
	public static MemCacheKeys pool_request_center = new MemCacheKeys("pool_request_center");
	public static MemCacheKeys pool_test = new MemCacheKeys("pool_test");
	public static MemCacheKeys pool_photo_test = new MemCacheKeys("pool_photo_test");
	
	public static final String exp_dialy = "exp_dialy_";
	public static final String user_space = "user_space_dis_";
	public static final String user_space_desc = "user_space_desc_";
	public static final String key_spm_user = "user_spm_";
	public static final String key_guide = "guide_";
	public static final String key_space_share = "share_sp_";
	public static final String key_going = "user_going_";
	public static final String key_tips = "tips_";
	public static final String key_icode_all = "icode_all_";
	public static final String key_icode_user = "icode_user_";

	public static final String ad_tribe_request = "ad_tribe_request";
	public static final String tribe_recent_update = "tribe_recent_update";
	public static final String meida_reports = "meida_reports_";

	public static final String key_total_menu = "total_menu";
	public static final String gossip_queue_on_home_ = "gossip_queue_on_home_";
	public static final String blog_queue_on_home_ = "blog_queue_on_home_";

	public static long second = 1;
	public static long minute = second * 60;
	public static long hour = minute * 60;
	public static long day = hour * 24;
	public static long month = day * 30;
	public static long year = month * 12;

	private String pool_name;

	protected MemCacheKeys(String poolName) {
		this.pool_name = poolName;
	}

	public String getPoolName() {
		return pool_name;
	}

}