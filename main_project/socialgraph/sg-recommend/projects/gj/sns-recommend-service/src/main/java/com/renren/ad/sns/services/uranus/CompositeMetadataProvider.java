package com.renren.ad.sns.services.uranus;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.ResultSetExtractor;

import com.google.code.morphia.Datastore;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.User;
import com.renren.ad.sns.util.FileWatchdog;
import com.xiaonei.xce.userapicache.UserApiCache;
import com.xiaonei.xce.userapicache.UserApiCacheAdapter;

public class CompositeMetadataProvider implements IDataProvider {
	protected static final Log LOG = LogFactory
			.getLog(CompositeMetadataProvider.class);

	private final static int U2C_LEN = 20;

	private final static int SG_LEN = 7;

	private final static int U2U_LEN = 6;

	FileWatchdog userDataWatchdog;

	FileWatchdog goodsDataWatchdog;

	FileWatchdog u2uDataWatchdog;
	
	FileWatchdog u2categoryDataWatchdog;

	UserApiCacheAdapter adapter = UserApiCacheAdapter.getInstance();

	protected Map<Long, UserMetadata> users = new HashMap<Long, UserMetadata>();

	protected Map<Long, int[]> staticGoods = new HashMap<Long, int[]>();

	protected Map<Long, int[]> u2cs = new HashMap<Long, int[]>();
	
	protected Map<Long, int[]> u2us = new HashMap<Long, int[]>();
	
	private JdbcTemplate jdbcTemplate;

	private Datastore datastore;

	private String dataPath;
	
	/* object lock */
	protected final Object usersLock = new byte[0];
	protected final Object staticGoodsLock = new byte[0];
	protected final Object u2csLock = new byte[0];
	protected final Object u2usLock = new byte[0];

	public void init() {
		userDataWatchdog = new FileWatchdog(dataPath + File.separator + "users.dat") {
			@Override
			protected void doOnChange() {
				LOG.info("loading new user data ...");
				Scanner cin;
				String line=null;
				try {
					cin = new Scanner(new FileInputStream(this.filename));
					Map<Long, UserMetadata> tmpu = new HashMap<Long, UserMetadata>();
					while (cin.hasNext()) {
						try{
							line = cin.nextLine();
							UserMetadata user = new UserMetadata(line);
							tmpu.put(user.getUid(), user);
						}catch(Exception e)
						{
							LOG.error("UserMetadata Error in line:"+line,e);
						}
					}
					cin.close();
					if (tmpu.size() > 0) {
					    synchronized(usersLock){
					        users = tmpu;
					    }
					}
				} catch (FileNotFoundException e) {
					LOG.error(e, e);
				} catch (Exception e) {
					LOG.error("Parse U2C Exception : " + line, e);
				}
				LOG.info("loaded new user data with " + users.size() + " user");
			}
		};
		userDataWatchdog.start();
		goodsDataWatchdog = new FileWatchdog(dataPath + File.separator + "goods.dat") {
			@Override
			protected void doOnChange() {
				LOG.info("loading new goods data ...");
				Scanner cin;
				try {
					cin = new Scanner(new FileInputStream(this.filename));
					Map<Long, int[]> tmpg = new HashMap<Long, int[]>();
					while (cin.hasNext()) {
						String line = cin.nextLine();
						try {
							String[] tmps = line.split(",");
							if (tmps.length >= SG_LEN) {
								int[] values = new int[SG_LEN - 1];
								long gid = Long.parseLong(tmps[0]);
								for (int i = 1; i < SG_LEN; i++) {
									values[i - 1] = Integer.parseInt(tmps[i]);
								}
								tmpg.put(gid, values);
							}
						} catch (Exception e) {
							LOG.error("Parse U2C Exception : " + line, e);
						}
					}
					cin.close();
					if (tmpg.size() > 0) {
					    synchronized(staticGoodsLock){
					        staticGoods = tmpg;
					    }
					}
				} catch (FileNotFoundException e) {
					LOG.error("load new goods.dat error!", e);
				}
				LOG.info("loaded new goods data with " + staticGoods.size() + " goods");
			}
		};
		goodsDataWatchdog.start(); 
		
		u2uDataWatchdog = new FileWatchdog(dataPath + File.separator + "u2us.dat") {
			@Override
			protected void doOnChange() {
				LOG.info("loading new user2user data ...");
				Scanner cin;
				try {
					cin = new Scanner(new FileInputStream(this.filename));
					Map<Long, int[]> tmpu2u = new HashMap<Long, int[]>();
					while (cin.hasNext()) {
						String line = cin.nextLine();
						String[] tmps = line.split(",");
						if (tmps.length == U2U_LEN+2) {
							long uid = Long.parseLong(tmps[0]);
							long ownerid = Long.parseLong(tmps[1]);
							long key = uid * Integer.MAX_VALUE + ownerid;
							int[] value = new int[U2U_LEN];
							for (int i = 0; i < U2U_LEN; i++) {
								value[i] = Integer.parseInt(tmps[i + 2]);
							}
							tmpu2u.put(key, value);
						}
					}
					cin.close();
					if (tmpu2u.size() > 0) {
					    synchronized(u2usLock){
					        u2us = tmpu2u;
					    }
					}
				} catch (FileNotFoundException e) {
					LOG.error(e, e);
				}
				LOG.info("loaded new user2user data with " + u2us.size() + " items");
			}
		};
		u2uDataWatchdog.start(); 

		u2categoryDataWatchdog = new FileWatchdog(dataPath + File.separator + "u2c.dat") {
			@Override
			protected void doOnChange() {
				LOG.info("loading new user2category data ...");
				Scanner cin;
				try {
					cin = new Scanner(new FileInputStream(this.filename));
					Map<Long, int[]> tmpu2c = new HashMap<Long, int[]>();
					while (cin.hasNext()) {
						String line = cin.nextLine();
						String[] tmps = line.split(",");
						if (tmps.length == U2C_LEN) {
							long uid = Long.parseLong(tmps[0]);
							long category = Long.parseLong(tmps[1]);
							long key = category * Integer.MAX_VALUE + uid;
							int[] value = new int[U2C_LEN - 2];
							for (int i = 0; i < U2C_LEN - 2; i++) {
								value[i] = Integer.parseInt(tmps[i + 2]);
							}
							tmpu2c.put(key, value);
						}
					}
					cin.close();
					if (tmpu2c.size() > 0) {
					    synchronized(u2csLock){
					        u2cs = tmpu2c;
					    }
					}
				} catch (FileNotFoundException e) {
					LOG.error(e, e);
				}
				LOG.info("loaded new user2category data with " + u2cs.size() + " items");
			}
		};
		u2categoryDataWatchdog.start(); 
	}

	@Override
	public UserMetadata getUserMetaData(final long uid) {
		UserMetadata data = null;
		if (users.containsKey(uid)) {
			data = users.get(uid);
		} else { //Get user metadata
			Number [] numbers = user(uid);
			data = new UserMetadata(uid, numbers);
			users.put(uid, data);
		}		
		
		return data;
	}

	@Override
	public GoodsMetadata getGoodsMetaData(final long uid, final long gid) {
		GoodsMetadata data = null;
		// get static goods
		int category = 0;
		int owner_id = 0;
		int[] sg = null;
		if (staticGoods.containsKey(gid)) {
			sg = staticGoods.get(gid);
		} else {
			sg = sg(gid);
			staticGoods.put(gid, sg);
		}
		category = sg[1];
		owner_id = sg[0];
		// get user owner
		long key = uid * Integer.MAX_VALUE + owner_id;
		int[] u2u = null;
		if( u2us.containsKey(key)) {
			u2u = u2us.get(key);
		}else{
			u2u = (int[]) jdbcTemplate.query("select is_following,is_follower,visit_count,like_count,"
					+ "comment_count, click_count "
					+ " from u2u_actions where user_id = ? and owner_id = ?",
					new Object[] { new Long(uid), new Long(owner_id) },
					new ResultSetExtractor() {
						@Override
						public int[] extractData(ResultSet rs) throws SQLException,
								DataAccessException {
							int[] u2u = new int[U2U_LEN];
							if( rs.next()) {
								try{
									for (int i = 0; i < U2U_LEN; i++) {
										u2u[i] = rs.getInt(i + 1);
									}
								}catch(Exception e){
									LOG.error("rowmapper error!",e);
								}
							}
							return u2u;
						}

					});			
		}

		// get user 2 category
		int[] u2c = null;
		key = 1l*  category * Integer.MAX_VALUE + uid;
		if (u2cs.containsKey(key)) {
			u2c = u2cs.get(key);
		} else {
			u2c = new int[GoodsMetadata.GG_UC];
			for (int i = 0; i < GoodsMetadata.GG_UC; i++) {
				u2c[i] = 0;
			}
		}

		int[] now = new int[2];
		now[0] = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
		now[1] = Calendar.getInstance().get(Calendar.DAY_OF_WEEK);
		
		data = new GoodsMetadata(sg, u2u, u2c, now);
		return data;
	}

	@Override
	public List<GoodsMetadata> getGoodsMetaData(final long uid,
			final List<Long> gids) {
		List<GoodsMetadata> goods = new ArrayList<GoodsMetadata>();
		for (long gid : gids) {
			goods.add(getGoodsMetaData(uid, gid));
		}
		return goods;
	}

	private int[] sg(final long gid) {
		int[] res = null;
		try {
			Goods goods = datastore
					.find(Goods.class, "goodsId", gid)
					.retrievedFields(true, "goodsId", "ownerId", "categoryId",
							"price", "shareCount", "likeCount", "tagList")
					.get();
			if (goods != null) {
				res = new int[GoodsMetadata.GG_SG];
				res[0] = new Long(goods.getOwnerId()).intValue();
				res[1] = goods.getCategoryId();
				res[2] = goods.getPrice();
				res[3] = goods.getShareCount();
				res[4] = goods.getShareCount();
				res[5] = goods.getLikeCount();
			}
		} catch (Exception e) {
			LOG.error(e, e);
		}
		return res;
	}

	private Number[] user(final long uid) {
		Number [] numbers = new Number [UserMetadata.UU_SU];
		try {
			if( uid > 0 ) {
			User user = datastore
					.find(User.class, "userId", uid)
					.retrievedFields(true, "userAbsInfo.followCount",
							"userAbsInfo.fansCount").get();
			UserApiCache userCache = adapter.getUserApiCache(new Long(uid)
					.intValue());
			numbers[0] = uid;
			int age = Calendar.getInstance().get(Calendar.YEAR)
					- userCache.getBirthyear();
			numbers[1] = age;
			int male = userCache.getGender().contains("男") ? 1 : 0;
			numbers[2] = male;
			int area = 8600;
			numbers[3] = area;
			int stage = userCache.getStage();
			numbers[4] = stage(stage);
			int following_count = user.getUserAbsInfo().getFollowCount();
			numbers[5] = following_count;
			int follower_count = user.getUserAbsInfo().getFansCount();
			numbers[6] = follower_count;
			}else{
			    numbers[0] =  0 ;
			    numbers[1] =  20 ;
			    numbers[2] =  0;
			    numbers[3] =  8600 ;
			    numbers[4] =  4 ;
			    numbers[5] =  0 ;
			    numbers[6] =  0 ;
			}	
		} catch (Exception e) {
			LOG.error("Get Static User Info Error With Uid = " + uid, e);
		}
		return numbers;
	}

	private Number stage(int stage) {
		stage /= 10;
		return stage==9?0:stage==3?4:stage; 
	}
	
	public void setJdbcTemplate(JdbcTemplate jdbcTemplate) {
		this.jdbcTemplate = jdbcTemplate;
	}

	public void setDatastore(Datastore datastore) {
		this.datastore = datastore;
	}

	public void setDataPath(String dataPath) {
		this.dataPath = dataPath;
	}
}

