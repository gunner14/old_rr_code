package com.renren.ad.sns.services.uranus;

import java.util.HashMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class GoodsMetadata extends Metadata {

	private static final Log LOG = LogFactory.getLog(GoodsMetadata.class);

	public static final int FIELD_SIZE = 32;

	public final static int GG_SG = 6;

	public final static int GG_UU = 12;

	public final static int GG_UC = 30;

	static final String SG_OWNER_ID = "sg_owner_id";
	static final String SG_CATEGORY_ID = "sg_category_id";
	static final String SG_PRICE = "sg_price";
	static final String SG_FORWD_COUNT = "sg_forwd_count";
	static final String SG_COMMENT_COUNT = "sg_comment_count";
	static final String SG_LIKE_COUNT = "sg_like_count";
	static final String U2U_IS_FOLLOWING = "u2u_is_following";
	static final String U2U_IS_FOLLOWER = "u2u_is_follower";
	static final String U2U_OWNER_VISIT = "u2u_owner_visit";
	static final String U2U_OWNER_LIKE = "u2u_owner_like";
	static final String U2U_OWNER_COMMENT = "u2u_owner_comment";
	static final String U2U_OWNER_CLICK = "u2u_owner_click";
	static final String U2C_CATEGORY_VISIT_DAY = "u2c_category_visit_day";
	static final String U2C_CATEGORY_VISIT_WEEK = "u2c_category_visit_week";
	static final String U2C_CATEGORY_VISIT_OTHER = "u2c_category_visit_other";
	static final String U2C_CATEGORY_VISIT_ALL = "u2c_category_visit_all";
	static final String U2C_CATEGORY_LIKE_DAY = "u2c_category_like_day";
	static final String U2C_CATEGORY_LIKE_WEEK = "u2c_category_like_week";
	static final String U2C_CATEGORY_LIKE_OTHER = "u2c_category_like_other";
	static final String U2C_CATEGORY_LIKE_ALL = "u2c_category_like_all";
	static final String U2C_CATEGORY_COMMENT_DAY = "u2c_category_comment_day";
	static final String U2C_CATEGORY_COMMENT_WEEK = "u2c_category_comment_week";
	static final String U2C_CATEGORY_COMMENT_OTHER = "u2c_category_comment_other";
	static final String U2C_CATEGORY_COMMENT_ALL = "u2c_category_comment_all";
	static final String U2C_CATEGORY_CLICK_DAY = "u2c_category_click_day";
	static final String U2C_CATEGORY_CLICK_WEEK = "u2c_category_click_week";
	static final String U2C_CATEGORY_CLICK_OTHER = "u2c_category_click_other";
	static final String U2C_CATEGORY_CLICK_ALL = "u2c_category_click_all";
	static final String U2C_MONITARY_CATEGORY_PUBLISH_WEEK = "u2c_monitary_category_publish_week";
	static final String U2C_MONITARY_ALL_PUBLISH_WEEK = "u2c_monitary_all_publish_week";
	static final String NOW_HOUR = "now_hour";
	static final String NOW_DAY = "now_day";

	static {
		fields = new HashMap<String, Integer>();
		fields.put(SG_OWNER_ID, 0);
		fields.put(SG_CATEGORY_ID, 1);
		fields.put(SG_PRICE, 2);
		fields.put(SG_FORWD_COUNT, 3);
		fields.put(SG_COMMENT_COUNT, 4);
		fields.put(SG_LIKE_COUNT, 5);
		fields.put(U2U_IS_FOLLOWING, 6);
		fields.put(U2U_IS_FOLLOWER, 7);
		fields.put(U2U_OWNER_VISIT, 8);
		fields.put(U2U_OWNER_LIKE, 9);
		fields.put(U2U_OWNER_COMMENT, 10);
		fields.put(U2U_OWNER_CLICK, 11);
		fields.put(U2C_CATEGORY_VISIT_DAY, 12);
		fields.put(U2C_CATEGORY_VISIT_WEEK, 13);
		fields.put(U2C_CATEGORY_VISIT_OTHER, 14);
		fields.put(U2C_CATEGORY_VISIT_ALL, 15);
		fields.put(U2C_CATEGORY_LIKE_DAY, 16);
		fields.put(U2C_CATEGORY_LIKE_WEEK, 17);
		fields.put(U2C_CATEGORY_LIKE_OTHER, 18);
		fields.put(U2C_CATEGORY_LIKE_ALL, 19);
		fields.put(U2C_CATEGORY_COMMENT_DAY, 20);
		fields.put(U2C_CATEGORY_COMMENT_WEEK, 21);
		fields.put(U2C_CATEGORY_COMMENT_OTHER, 22);
		fields.put(U2C_CATEGORY_COMMENT_ALL, 23);
		fields.put(U2C_CATEGORY_CLICK_DAY, 24);
		fields.put(U2C_CATEGORY_CLICK_WEEK, 25);
		fields.put(U2C_CATEGORY_CLICK_OTHER, 26);
		fields.put(U2C_CATEGORY_CLICK_ALL, 27);
		fields.put(U2C_MONITARY_CATEGORY_PUBLISH_WEEK, 28);
		fields.put(U2C_MONITARY_ALL_PUBLISH_WEEK, 29);
		fields.put(NOW_HOUR, 30);
		fields.put(NOW_DAY, 31);

		fieldsIdx = new String[] { "sg_owner_id", "sg_category_id", "sg_price",
				"sg_forwd_count", "sg_comment_count", "sg_like_count",
				"u2u_is_following", "u2u_is_follower", "u2u_owner_visit",
				"u2u_owner_like", "u2u_owner_comment", "u2u_owner_click",
				"u2c_category_visit_day", "u2c_category_visit_week",
				"u2c_category_visit_other", "u2c_category_visit_all",
				"u2c_category_like_day", "u2c_category_like_week",
				"u2c_category_like_other", "u2c_category_like_all",
				"u2c_category_comment_day", "u2c_category_comment_week",
				"u2c_category_comment_other", "u2c_category_comment_all",
				"u2c_category_click_day", "u2c_category_click_week",
				"u2c_category_click_other", "u2c_category_click_all",
				"u2c_monitary_category_publish_week",
				"u2c_monitary_all_publish_week", "now_hour", "now_day" };
	}

	private long gid = 0;

	private GoodsMetadata() {
		super();
		contents = new Number[fields.size()];
	}

	public GoodsMetadata(long gid, Number... numbers) {
		this();

		this.gid = gid;
		setContent(numbers);
	}

	public GoodsMetadata(long gid, String content) {
		this(gid, Metadata.string2Contents(content));
	}

	public GoodsMetadata(Number[] sg, Number[] uu, Number[] uc, Number[] now) {
		this();
		if (sg != null) {
			for (int idx = 0, i = 0; idx < GG_SG && i < sg.length;) {
				contents[idx++] = sg[i++];
			}
		}
		if (uu != null) {
			for (int idx = GG_SG, i = 0; idx < GG_UU && i < uu.length;) {
				contents[idx++] = uu[i++];
			}
		}
		if (uc != null) {
			for (int idx = GG_UU, i = 0; idx < GG_UC && i < uc.length;) {
				contents[idx++] = uc[i++];
			}
		}
		if (now != null) {
			for (int idx = GG_UC, i = 0; idx < contents.length
					&& i < now.length;) {
				contents[idx++] = now[i++];
			}
		}
		this.gid = 0;
	}

	public GoodsMetadata(int[] sg, int[] uu, int[] uc, int[] now) {
		this();
		if (sg != null) {
			for (int idx = 0, i = 0; idx < GG_SG && i < sg.length;) {
				contents[idx++] = sg[i++];
			}
		}
		if (uu != null) {
			for (int idx = GG_SG, i = 0; idx < GG_UU && i < uu.length;) {
				contents[idx++] = uu[i++];
			}
		}
		if (uc != null) {
			for (int idx = GG_UU, i = 0; idx < GG_UC && i < uc.length;) {
				contents[idx++] = uc[i++];
			}
		}
		if (now != null) {
			for (int idx = GG_UC, i = 0; idx < contents.length
					&& i < now.length;) {
				contents[idx++] = now[i++];
			}
		}
		this.gid = 0;
	}

	public long getGid() {
		return gid;
	}
	
	/**
	 * set the values and padding the blank field
	 * @param numbers
	 */
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
}
