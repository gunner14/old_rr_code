package com.renren.ad.sns.services.uranus;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class UserMetadata extends Metadata {
	private static final Log LOG = LogFactory.getLog(UserMetadata.class);
	public static final int FIELD_SIZE = 19;

	public final static int UU_SU = 7;

	public final static int UU_RFM = 19;
	
	static final String SU_UID = "su_uid";
	static final String SU_AGE = "su_age";
	static final String SU_MALE = "su_male";
	static final String SU_AREA = "su_area";
	static final String SU_STAGE = "su_stage";
	static final String SU_FOLLOWING_COUNT = "su_following_count";
	static final String SU_FOLLOWER_COUNT = "su_follower_count";
	static final String RFM_LOGIN_COUNT_DAY = "rfm_login_count_day";
	static final String RFM_LOGIN_COUNT_WEEK = "rfm_login_count_week";
	static final String RFM_MONITARY_VISIT_DAY = "rfm_monitary_visit_day";
	static final String RFM_MONITARY_VISIT_WEEK = "rfm_monitary_visit_week";
	static final String RFM_MONITARY_LIKE_DAY = "rfm_monitary_like_day";
	static final String RFM_MONITARY_LIKE_WEEK = "rfm_monitary_like_week";
	static final String RFM_MONITARY_COMMENT_DAY = "rfm_monitary_comment_day";
	static final String RFM_MONITARY_COMMENT_WEEK = "rfm_monitary_comment_week";
	static final String RFM_MONITARY_CLICK_DAY = "rfm_monitary_click_day";
	static final String RFM_MONITARY_CLICK_WEEK = "rfm_monitary_click_week";
	static final String RFM_MONITARY_PUBLISH_DAY = "rfm_monitary_publish_day";
	static final String RFM_MONITARY_PUBLISH_WEEK = "rfm_monitary_publish_week";

	static {
		fields = new HashMap<String, Integer>();
		fields.put(SU_UID,0);
		fields.put(SU_AGE,1);
		fields.put(SU_MALE,2);
		fields.put(SU_AREA,3);
		fields.put(SU_STAGE,4);
		fields.put(SU_FOLLOWING_COUNT,5);
		fields.put(SU_FOLLOWER_COUNT,6);
		fields.put(RFM_LOGIN_COUNT_DAY,7);
		fields.put(RFM_LOGIN_COUNT_WEEK,8);
		fields.put(RFM_MONITARY_VISIT_DAY,9);
		fields.put(RFM_MONITARY_VISIT_WEEK,10);
		fields.put(RFM_MONITARY_LIKE_DAY,11);
		fields.put(RFM_MONITARY_LIKE_WEEK,12);
		fields.put(RFM_MONITARY_COMMENT_DAY,13);
		fields.put(RFM_MONITARY_COMMENT_WEEK,14);
		fields.put(RFM_MONITARY_CLICK_DAY,15);
		fields.put(RFM_MONITARY_CLICK_WEEK,16);
		fields.put(RFM_MONITARY_PUBLISH_DAY,17);
		fields.put(RFM_MONITARY_PUBLISH_WEEK,18);

		fieldsIdx = new String[] { "su_uid", "su_age", "su_male", "su_area",
				"su_stage", "su_following_count", "su_follower_count",
				"rfm_login_count_day", "rfm_login_count_week",
				"rfm_monitary_visit_day", "rfm_monitary_visit_week",
				"rfm_monitary_like_day", "rfm_monitary_like_week",
				"rfm_monitary_comment_day", "rfm_monitary_comment_week",
				"rfm_monitary_click_day", "rfm_monitary_click_week",
				"rfm_monitary_publish_day", "rfm_monitary_publish_week" };
	}

	private long uid = 0;

	public UserMetadata() {
		super();
		contents = new Number[fields.size()];
	}

	public UserMetadata(long uid, Number... numbers) {
		this();
		setContent(numbers);
		this.uid = uid;
	}

	public UserMetadata(long uid, List<Number> numbers) {
		this();
		setContent(numbers.toArray(new Number[0]));
		this.uid = uid;
	}

	public UserMetadata(long uid, String content) {
		this(uid, Metadata.string2Contents(content));
	}

	public UserMetadata(String content) {
		this();
		Number[] numbers = Metadata.string2Contents(content);
		if (numbers != null && numbers.length > 0) {
			this.uid = numbers[0].longValue();
			setContent(numbers);
		}
	}

	public long getUid() {
		return this.uid;
	}

	private void setContent(Number... numbers) {
		if (numbers != null) {
			int len = Math.min(numbers.length, contents.length);
			for (int i = 0; i < len; i++) {
				contents[i] = numbers[i];
			}
			for (int i = len; i < contents.length; i++) {
				contents[i] = 0;
			}
		}
	}

	public static void main(String[] args) throws InterruptedException,
			FileNotFoundException {
		Scanner cin = new Scanner(new FileInputStream("users.dat"));
		Map<Long, UserMetadata> data = new HashMap<Long, UserMetadata>();
		while (cin.hasNext()) {
			String line = cin.nextLine();
			UserMetadata user = new UserMetadata(line);
			data.put(user.getUid(), user);
		}
		cin.close();
		System.out.println(data.size());
		Thread.sleep(600000);
	}
}
