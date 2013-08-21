package com.xiaonei.platform.core.opt;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.xce.XceAdapter;

/**
 * datasource管理器，通过它可以实现db访问，进行db操作<br>
 * 
 * @author lifs
 */
public class DataAccessMgr implements OperateDataSource {

	private static final String CMD_BATCH = "batch";

	private static final String CMD_SQL = "sql";

	public static final String BIZ_ACCOUNT = XceAdapter.DB_ACCOUNT;

	// 主页源 (小白、启容删)
	// public static final String BIZ_LATEST_LOGIN_FRIENDS =
	// XceAdapter.DB_LATEST_LOGIN_FRIENDS;

	// 活动用库(活动终于有自己的库啦～)
	public static final String BIZ_ACTIVITY = XceAdapter.DB_ACTIVITY;

	/** 审核内容 */
	public static final String BIZ_ADMIN_CONTENT = XceAdapter.DB_AUDIT_CONTENT;

	/** 审核人 */
	public static final String BIZ_ADMIN_USER = XceAdapter.DB_AUDIT_USER;

	public static final String BIZ_ALBUM = XceAdapter.DB_ALBUM;

	public static final String BIZ_ALBUM_INFO = XceAdapter.DB_ALBUM_INFO;

	/** app */
	public static final String BIZ_APP = XceAdapter.DB_APP;

	public static final String BIZ_APP_LOG = XceAdapter.DB_APP_LOG;

	public static final String BIZ_APP_USER = XceAdapter.DB_APP_USER;

	public static final String BIZ_API_PAY = XceAdapter.DB_API_PAY;

	public static final String BIZ_APPS_TYCOON = XceAdapter.DB_TYCOON;

	public static final String BIZ_GODFATHER_INFO = XceAdapter.DB_GODFATHER;

	public static final String BIZ_APP_USER_MENU = XceAdapter.DB_APP_USER_MENU;

	// 以下两个是App留言版和粉丝
	public static final String BIZ_APP_COMMENTS = XceAdapter.DB_APP_COMMENTS;

	public static final String BIZ_APP_FANS = XceAdapter.DB_APP_FANS;

	/** API Connect */
	public static final String BIZ_APP_CONNECT_HASH = XceAdapter.DB_APP_CONNECT;// 散表

	public static final String BIZ_APP_CONNECT = XceAdapter.DB_APP_CONNECT_EMAIL;

	/** 日志散表 */
	public static final String BIZ_BLOG = XceAdapter.DB_BLOG;

	public static final String BIZ_BLOG_INFO = XceAdapter.DB_BLOG_INFO;

	public static final String BIZ_BLOG_STAT = XceAdapter.DB_BLOG_STAT;

	public static final String BIZ_BROWSE = XceAdapter.DB_BROWSE;

	public static final String BIZ_CARPARK = XceAdapter.DB_CARPARK;

	public static final String BIZ_CARPARK_GAME = XceAdapter.DB_CARPARKGAME;

	public static final String BIZ_CARPARK_LOG = XceAdapter.DB_CARPARK_LOG;

	/** 班级散表 */
	public static final String BIZ_CLASS = XceAdapter.DB_CLASS;

	public static final String BIZ_CLASS_INFO = XceAdapter.DB_CLASS_INFO;

	public static final String BIZ_COMMON = XceAdapter.DB_COMMON;

	public static final String BIZ_COUNT = XceAdapter.DB_COUNT;

	// 状态用
	public static final String BIZ_DOING = XceAdapter.DB_DOING;

	// 活动(Event)
	public static final String BIZ_EVENT = XceAdapter.DB_EVENT;

	public static final String BIZ_FRIEND_DIST = XceAdapter.DB_FRIEND_DIST;

	public static final String BIZ_GAOKAO = XceAdapter.DB_GAOKAO;

	/** 礼物 非散表 */
	public static final String BIZ_GIFT_INFO = XceAdapter.DB_GIFT;

	// 将poke、going等表单移出来
	public static final String BIZ_GOING_INFO = XceAdapter.DB_GOING_INFO;

	/** 留言散表 */
	public static final String BIZ_GOSSIP = XceAdapter.DB_GOSSIP;

	public static final String BIZ_GOSSIP_INFO = XceAdapter.DB_GOSSIP_INFO;

    public static final String BIZ_GOSSIP_NEW = XceAdapter.DB_GOSSIP_NEW;

    public static final String BIZ_GOSSIP_INFO_NEW = XceAdapter.DB_GOSSIP_INFO_NEW;

	// public static final String BIZ_TRIBE = XceAdapter.DB_TRIBE;
	/** 群组散表 */
	public static final String BIZ_GROUP = XceAdapter.DB_GROUP;

	public static final String BIZ_GROUP_INFO = XceAdapter.DB_GROUP_INFO;

	public static final String BIZ_GROUP_MEMBER = XceAdapter.DB_GROUP_MEMBER;

	public static final String BIZ_GUIDE = XceAdapter.DB_GUIDE;

	public static final String BIZ_IDSEQUENCE = XceAdapter.DB_IDSEQUENCE;

	// 存放即时mail
	public static final String BIZ_IMMEDIATE_EMAIL = XceAdapter.DB_EMAIL;

	public static final String BIZ_IMMSG = XceAdapter.DB_IMMSG;

	/* 了若指掌 */
	public static final String BIZ_KNOWABC_INFO = XceAdapter.DB_KNOWABC_INFO;

	public static final String BIZ_KNOWABC_NEWS = XceAdapter.DB_KNOWABC_NEWS;

	public static final String BIZ_KNOWABC_SUB = XceAdapter.DB_KNOWABC_SUB;

	public static final String BIZ_KNOWABC_INVESTIGATION = XceAdapter.DB_KNOWABC_INVESTIGATION;

	public static final String BIZ_LOGGING = XceAdapter.DB_LOGGING;

	/* 蛊惑仔 */
	public static final String BIZ_MOBWAR_INFO = XceAdapter.DB_MOBWAR;

	public static final String BIZ_MOBWAR_NEWS = XceAdapter.DB_MOBWAR_NEWS;

	public static final String BIZ_MOBWAR_FIGHT = XceAdapter.DB_MOBWAR_FIGHT;

	public static final String BIZ_MOBWAR_REL = XceAdapter.DB_MOBWAR_REL;

	// public static final String BIZ_MOBWAR_LIST = XceAdapter.DB_MOBWAR_LIST;

	// for wap
	public static final String BIZ_MOBILE_STAT = XceAdapter.DB_MOBILE_STAT;

	public static final String BIZ_MAIL_MMS = XceAdapter.DB_MAIL_MMS;

	public static final String BIZ_WAP_PAY = XceAdapter.DB_WAP_PAY;

	public static final String BIZ_WAP_REGISTER = XceAdapter.DB_WAP_REGISTER;

	/* mini city */
	public static final String BIZ_MINICITY_INFO = XceAdapter.DB_MINICITY;

	/** market 非散表 */
	public static final String BIZ_MARKET_INFO = XceAdapter.DB_MARKET;

	// public static final String BIZ_ADMIN_ = XceAdapter.DB_ADMIN;

	public static final String BIZ_MEMBER_GROUP = XceAdapter.DB_MEMBER_GROUP;

	/** 站内信散表 */
	public static final String BIZ_MESSAGE = XceAdapter.DB_MESSAGE;

	public static final String BIZ_MESSAGE_INFO = XceAdapter.DB_MESSAGE_INFO;

	public static final String BIZ_MESSAGE_SESSION = XceAdapter.DB_MESSAGE_SESSION;

	/** 影评 */
	public static final String BIZ_MOVIE_INFO = XceAdapter.DB_LOGGING;

	public static final String BIZ_NOTIFY = XceAdapter.DB_MESSAGE_NOTIFY;

	public static final String BIZ_NOTIFY_CONFIG = XceAdapter.DB_NOTIFY_CONFIG;

	/**
	 * 请求中心
	 */
	public static final String BIZ_REQUEST_CENTER = XceAdapter.DB_REQUEST_CENTER;

	public static final String BIZ_REQUEST_CENTER_APP = XceAdapter.DB_REQUEST_CENTER_APP;

	public static final String BIZ_REQUEST_CENTER_SYSTEM = XceAdapter.DB_REQUEST_CENTER_SYSTEM;

	// 紧急审核专用(如台独，藏独)
	public static final String BIZ_POLICE = XceAdapter.DB_POLICE;

	public static final String BIZ_POP_USER = XceAdapter.DB_POP_USER;

	public static final String BIZ_RELATION = XceAdapter.DB_RELATION;

	public static final String BIZ_RELATION_INFO = XceAdapter.DB_RELATION_INFO;

    public static final String BIZ_RELATION_BLOCK = XceAdapter.DB_RELATION_BLOCK;

	// 好友描述申请的综述表
	public static final String BIZ_REQUEST_FRIEND_MEMO = XceAdapter.DB_REQUEST_FRIEND_MEMO;

	/** 搜索用的 */
	public static final String BIZ_SEARCH = XceAdapter.DB_SEARCH;

	public static final String BIZ_SELF_MESSAGE = XceAdapter.DB_SELF_MESSAGE;

	public static final String BIZ_SHARE = XceAdapter.DB_SHARE;

	public static final String BIZ_STAT = XceAdapter.DB_STAT;

	public static final String BIZ_USER_APP = XceAdapter.DB_USER_APP;

	public static final String BIZ_USER_COMMON = XceAdapter.DB_USER_COMMON;

	public static final String BIZ_USER_CONFIG = XceAdapter.DB_USER_CONFIG;

	public static final String BIZ_USER_DESC = XceAdapter.DB_USER_DESC;

	public static final String BIZ_USER_MSN = XceAdapter.DB_USER_MSN;

	public static final String BIZ_USER_SCORE = XceAdapter.DB_USER_SCORE;

	public static final String BIZ_USER_SCORE_INFO = XceAdapter.DB_USER_SCORE_INFO;

	public static final String BIZ_USER_VIEW_COUNT = XceAdapter.DB_USER_VIEW_COUNT;

	public static final String BIZ_USER_NETWORK = XceAdapter.DB_USER_NETWORK;

	public static final String BIZ_VAMPIRE = XceAdapter.DB_VAMPIRE;

	/** 做邮件打开和回点统计的数据源 */
	public static final String BIZ_EMAIL_STAT = XceAdapter.DB_EMAIL_STAT;

	public static final String BIZ_INVITE_LETTER = XceAdapter.DB_INVITE_LETTER;

	public static final String BIZ_OP_LETTER = XceAdapter.DB_OP_LETTER;

	public static final String BIZ_ACTIVATION_LETTER = XceAdapter.DB_ACTIVATION_LETTER;

    public static final String BIZ_USER_TIME_EMAIL = XceAdapter.DB_USER_TIME_EMAIL;

	/** 统计好友申请历史记录的 */
	public static final String BIZ_FRIEND_REQUEST = XceAdapter.DB_FRIEND_REQUEST;

	/** 注册邀请 */
	public static final String BIZ_REG_INVITE = XceAdapter.DB_REG_INVITE;

	/** 注册邀请,开心从天津倒过来的数据供合并使用 是原开心的注册邀请 */
	public static final String BIZ_KX_STAT = XceAdapter.DB_KX_STAT;

	public static final String BIZ_KX_REG_INVITE = XceAdapter.DB_KX_REG_INVITE;

    public static final String BIZ_INVITE_URL_PARAS = XceAdapter.DB_INVITE_URL_PARAS;

    public static final String BIZ_KX_INVITE_URL_PARAS = XceAdapter.DB_KX_INVITE_URL_PARAS;

	/** PAGE用到的数据源 */
	public static final String BIZ_PAGE = XceAdapter.DB_PAGE;

	public static final String BIZ_PAGE_FANS = XceAdapter.DB_PAGE_FANS;

	public static final String BIZ_FANS_PAGES = XceAdapter.DB_FANS_PAGES;

	public static final String BIZ_PAGE_STAT = XceAdapter.DB_USER_PAGE_STAT; 

	public static final String BIZ_PAGE_SCORE = XceAdapter.DB_PAGE_SCORE; 

	/** follow用到的数据源 */
	public static final String BIZ_FOLLOW = XceAdapter.DB_FOLLOW;

	/** wap游戏用到的数据源 */
	public static final String BIZ_MOBILE_GAME = XceAdapter.DB_MOBILE_GAME;

	/** wap游戏航海用到的数据源 */
	public static final String BIZ_VOYAGE = XceAdapter.DB_VOYAGE;

	/** 常志昂在用：跑马甲的 */
	public static final String BIZ_NEWBIE_PROFILE = XceAdapter.DB_NEWBIE_PROFILE;

	/** 安全中心 */
	public static final String BIZ_SAFE_CENTER = XceAdapter.DB_SAFE_CENTER;

	/** 手机客户端申请通讯录备份数据表 */
	public static final String BIZ_MOBILE_CLIENT = XceAdapter.DB_MOBILE_CLIENT;

	/** 热点分享 UGC */
	public static final String BIZ_SHARE_INFO = XceAdapter.DB_SHARE_INFO;

	/** SNS部门在用，用户个人信息中的个人爱好 */
	public static final String BIZ_USER_FOND = XceAdapter.DB_USER_FOND;// 用户个人信息中的个人爱好

    /** 公共主页VIDEO_COMMENT */
    public static final String BIZ_PAGE_VIDEO_COMMENT = XceAdapter.DB_PAGE_VIDEO_COMMENT;// 

    /** 人人网发召回邮件 */
    public static final String BIZ_RROP_LETTER = XceAdapter.DB_RROP_LETTER;// 
    
    /** 查询活跃用户的最后登录时间，用于发mail */
    public static final String BIZ_USER_TIME = XceAdapter.DB_USER_TIME;// 

    /** Ugc专用id sequence */
    public static final String BIZ_IDSEQ_UGC = XceAdapter.DB_IDSEQ_UGC;

    /** 高中项目数据库 */
    public static final String BIZ_HIGH_SCHOOL_PAGE = XceAdapter.DB_HIGH_SCHOOL_PAGE;

    /** 手机农场数据库 */
    public static final String BIZ_MOBILE_FARM = XceAdapter.DB_MOBILE_FARM;

    /** 积分系统新增 */
    public static final String BIZ_USER_SCORE_LOG = XceAdapter.DB_USER_SCORE_LOG;

    public static final String BIZ_USER_SCORE_TOTAL = XceAdapter.DB_USER_SCORE_TOTAL;

    public static final String BIZ_USER_SCORE_TOTAL_INFO = XceAdapter.DB_USER_SCORE_TOTAL_INFO;

    public static final String BIZ_USER_SCORE_AWARD_NEW = XceAdapter.DB_USER_SCORE_AWARD_NEW;

    /** 公共主页Page徽章系统 */
    public static final String BIZ_PAGE_BIZ = XceAdapter.DB_PAGE_BIZ;

    /** SEO 项目新增 */
    public static final String BIZ_SEO_COMPANY = XceAdapter.DB_SEO_COMPANY;

    public static final String BIZ_PASSPORT_WAP = XceAdapter.DB_PASSPORT_WAP;

    /** 相册圈人数据源 */
    public static final String BIZ_PAGE_RING = XceAdapter.DB_PAGE_RING;

    public static final String BIZ_UGC_EVENT = XceAdapter.DB_UGC_EVENT;

    public static final String BIZ_UGC_EVENT_INFO = XceAdapter.DB_UGC_EVENT_INFO;

    /** 游戏后台 */
    public static final String BIZ_GAME_CENTER = XceAdapter.DB_GAME_CENTER;

    public static final String BIZ_FINANCE_STATS = XceAdapter.DB_FINANCE_STATS;

    /** 安全中心窗项目 */
    public static final String BIZ_SAFECENTER_CHUANG = XceAdapter.DB_SAFECENTER_CHUANG;

    public static final String BIZ_FRIENDFINDER_USERLOGIN = XceAdapter.DB_FRIENDFINDER_USERLOGIN;

    /** 世界杯活动 */
    public static final String BIZ_WORLD_CUP_USER = XceAdapter.DB_WORLD_CUP_USER;

    public static final String BIZ_WORLD_CUP_COUPLE = XceAdapter.DB_WORLD_CUP_COUPLE;

    public static final String BIZ_USER_PASSPORT_INVITE = XceAdapter.DB_USER_PASSPORT_INVITE;

    /** 雪碧活动 */
    public static final String BIZ_XUEBI = XceAdapter.DB_XUEBI;

    public static final String BIZ_HOTORNOT = XceAdapter.DB_HOTORNOT;

    /** ASP 俱乐部 */
    public static final String BIZ_ASP_ORG = XceAdapter.DB_ASP_ORG;

	private static DataAccessMgr instance = new DataAccessMgr();

	public static DataAccessMgr getInstance() {
		return instance;
	}

	/**
	 * logger
	 */
	protected Log logger = LogFactory.getLog(this.getClass());

	private DataAccessMgr() {
		super();
	}

	public void closeConnection(final Connection con) {
		try {
			if (con != null) {
				con.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void closeResultSet(final ResultSet rs) {
		try {
			if (rs != null) {
				rs.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * @param rs
	 * @param st
	 * @param conn
	 */
	public void closeRSC(final ResultSet rs, final Statement st,
			final Connection conn) {
		if (rs != null) {
			try {
				rs.close();
			} catch (SQLException e) {
				logger.error("res close", e);
			}
		}
		if (st != null) {
			try {
				st.close();
			} catch (SQLException e) {
				logger.error("prestatement  close", e);
			}
		}
		if (conn != null) {
			try {
				conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
	}

	public void closeStatement(final Statement st) {
		try {
			if (st != null) {
				st.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 在调用getConn方法的时候没有指定r，或w，或者是没有得到一个connection对象 会抛出异常
	 * 
	 * @param o
	 * @param op
	 * @return
	 * @throws SQLException
	 */
	private Connection getConn(final char o, final Op op) throws SQLException {
		if (op.getBizName() == null || op.getBizName().length() == 0)
			throw new SQLException("Op中 BizName 不能为null");

		Connection conn = null;
		if (o == 'r') {
			if (op.getTableSuffix() < 0) {
				conn = getConnection(op.getBizName(), true);
			} else {
				conn = getConnection(op.getBizName(), op.getTableSuffix(), true);
			}
		} else if (o == 'w') {
			if (op.getTableSuffix() < 0) {
				conn = getConnection(op.getBizName(), false);
			} else {
				conn = getConnection(op.getBizName(), op.getTableSuffix(),
						false);
			}
		} else
			throw new SQLException("没有指定是 r 还是 w");
		if (conn == null) {
			throw new SQLException("couldn't retrive sql connection");
		}
		return conn;
	}

	/**
	 * 没有散表的情况下，要得到一个连接，可以调用此方法
	 * 
	 * @param bizName
	 *            业务名
	 * @param isReadConnection
	 *            是否获取读连接
	 * @return
	 * @throws SQLException
	 */
	public Connection getConnection(final String bizName,
			final boolean isReadConnection) throws SQLException {
		if (bizName == null || bizName.length() == 0)
			throw new SQLException("BizName 不能为null");
		Connection conn = null;
		if (isReadConnection) {
			try {
				conn = XceAdapter.getInstance().getReadConnection(bizName);
			} catch (Exception e) {
				// logger.error("-----取["+bizName+"]Conn时出异常,e|"+e+" \n
				// error_desc|"+e.getMessage());
				e.printStackTrace(System.err);
				throw new SQLException();
			}
		} else {
			try {
				conn = XceAdapter.getInstance().getWriteConnection(bizName);
			} catch (Exception e) {
				// logger.error("-----取["+bizName+"]Conn时出异常,e|"+e+" \n
				// error_desc|"+e.getMessage());
				e.printStackTrace(System.err);
				throw new SQLException();
			}
		}

		return conn;
	}

	/**
	 * 散表的情况下，要得到一个连接，可以调用此方法
	 * 
	 * @param bizName
	 *            业务名
	 * @param tableSuffix
	 *            散表名的后缀 ，如gossip_3，传入 3
	 * @param isReadConnection
	 *            是否获取读连接
	 * @return
	 * @throws SQLException
	 */
	public Connection getConnection(final String bizName,
			final int tableSuffix, final boolean isReadConnection)
			throws SQLException {
		if (bizName == null || bizName.length() == 0)
			throw new SQLException("BizName 不能为null");
		if (tableSuffix < 0)
			if (!bizName.equals(XceAdapter.DB_IDSEQUENCE))
				throw new SQLException("tableSuffix 不能为null");
		Connection conn = null;
		String tablePattern = getTablePattern(bizName, tableSuffix);
		if (isReadConnection) {
			conn = XceAdapter.getInstance().getReadConnection(bizName,
					tablePattern);
		} else {
			conn = XceAdapter.getInstance().getWriteConnection(bizName,
					tablePattern);
		}
		return conn;
	}

	/**
	 * 获得id，这个方法比较特殊，其数据源是固定的，jdbc/id
	 */
	public long getQueryId(final OpUniq op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long result = 0;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				if (rs.next()) {
					result = rs.getLong(1);
				}
				if (rs.next()) {
					logger
							.error("Non Unique Result Error: wrong sql syntax or database not consistence!");
				}
				return result;
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	private String getTablePattern(final String bizName, final int tableSuffix) {
		return bizName + "_" + tableSuffix;
	}

	public boolean insert(final OpUpdate op) throws SQLException {
		// 数据源
		PreparedStatement ps = null;
		ResultSet rs = null;
		Connection conn = null;
		long begin = 0L;
		if (ConnCounterMgr.enter(op.getBizName())) {
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				conn = getConn('w', op);
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				int count = ps.executeUpdate();
				if (count > 0) {
					if (ps != null) {
						ps.close();
					}
					return true;
				} else
					return false;

			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public int[] batchUpdate(final OpUpdate op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			// 数据源
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				conn = getConn('w', op);
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				return ps.executeBatch();
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	/**
	 * 慎用此方法
	 */
	public void insertBatchReturnFirstSqlId(final List<OpBatchUpdate> ops)
			throws SQLException {
		if (ops == null || ops.size() == 0)
			throw new SQLException(
					" ----- the opbatchupdate list can't null -------------");
		OpBatchUpdate firstOp = ops.get(0);
		if (firstOp.bizName == null || firstOp.bizName.trim().length() == 0)
			throw new SQLException(
					" ----- the bizName of the first opbatchupdate object can't null -------------");

		if (ConnCounterMgr.enter(firstOp.getBizName())) {
			// 数据源
			PreparedStatement ps = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				conn = getConn('w', firstOp);
				for (OpBatchUpdate opb : ops) {
					ps = conn.prepareStatement(opb.getSql());
					opb.setParam(ps);
					ps.executeUpdate();
					if (ps != null) {
						ps.close();
					}
				}
			} finally {
				ConnCounterMgr.outer(firstOp.getBizName());
				closeRSC(null, ps, conn);
				batchEnd(begin, "excutebatch sql");// 在log 中将不分析
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(firstOp.getBizName()));
		}
	}

	public int insertReturnId(final OpUpdate op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			// 数据源
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				conn = getConn('w', op);
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				// op.log(logger);
				int count = ps.executeUpdate();
				if (count > 0) {
					if (ps != null) {
						ps.close();
					}
					ps = conn.prepareStatement("select last_insert_id();");
					rs = ps.executeQuery();
					if (rs.next())
						return rs.getInt(1);
					else
						return 0;
				} else
					return 0;
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public boolean queryExist(final OpUniq op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				if (rs.next())
					return true;
				return false;
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	/**
	 * 获得id，这个方法比较特殊，其数据源是固定的，jdbc/id
	 */
	public int queryId(final OpUniq op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			int result = 0;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				if (rs.next()) {
					result = rs.getInt(1);
				}
				if (rs.next()) {
					logger
							.error("Non Unique Result Error: wrong sql syntax or database not consistence!");
				}
				return result;
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public int queryInt(final OpUniq op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {

			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			int result = 0;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());

				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				if (rs.next()) {
					result = rs.getInt(1);
				} else
					return 0;
				if (rs.next()) {
					logger
							.error("Non Unique Result Error: wrong sql syntax or database not consistence!");
				}
				return result;
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public int queryInt(final OpUniq op, final boolean master)
			throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			int result = 0;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				if (master) {
					conn = getConn('w', op);
				} else {
					conn = getConn('r', op);
				}
				ps = conn.prepareStatement(op.getSql());

				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				if (rs.next()) {
					result = rs.getInt(1);
				} else
					return 0;
				if (rs.next()) {
					logger
							.error("Non Unique Result Error: wrong sql syntax or database not consistence!");
				}
				return result;
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public List queryList(final OpList op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {

			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());

				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				while (rs.next()) {
					op.add(op.parse(rs));
				}
				return op.getResult();
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public List queryList(final OpList op, final boolean master)
			throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {

			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				if (master) {
					conn = getConn('w', op);
				} else {
					conn = getConn('r', op);
				}
				ps = conn.prepareStatement(op.getSql());

				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				while (rs.next()) {
					op.add(op.parse(rs));
				}
				return op.getResult();
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public long queryLong(final OpUniq op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long result = 0;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());

				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				if (rs.next()) {
					result = rs.getLong(1);
				} else
					return 0;
				if (rs.next()) {
					logger
							.error("Non Unique Result Error: wrong sql syntax or database not consistence!");
				}
				return result;
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public Map<String, Object> queryMap(final OpMap op,
			final String keyFieldName) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());

				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				while (rs.next()) {
					op.add(rs.getString(keyFieldName), op.parse(rs));
				}
				return op.getResult();
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public Map<String, Object> queryMap(final OpMap op,
			final String[] keyFieldNames) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {

			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());

				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				int len = keyFieldNames.length;
				String key = "";
				while (rs.next()) {
					for (int i = 0; i < len - 1; i++)
						if (i == len - 1) {
							key += rs.getString(keyFieldNames[i]);
						} else {
							key += rs.getString(keyFieldNames[i]) + ",";
						}
					op.add(key, op.parse(rs));
				}
				return op.getResult();
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public Object queryUnique(final OpUniq op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				conn = getConn('r', op);
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				if (rs.next()) {
					op.add(op.parse(rs));
				}
				if (rs.next()) {
					logger.error("----------【error sql】----------------");
					logger
							.error("Non Unique Result Error: wrong sql syntax or database not consistence!");
					logger.error("wrong sql is:" + op.getSql());
					logger.error("wrong ps is:" + ps);
				}
				return op.getResult();
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	public Object queryUnique(final OpUniq op, final boolean master)
			throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {

			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				if (master) {
					conn = getConn('w', op);
				} else {
					conn = getConn('r', op);
				}
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				// op.log(logger);
				rs = ps.executeQuery();
				if (rs.next()) {
					op.add(op.parse(rs));
				}
				if (rs.next()) {
					logger.error("----------【error sql】----------------");
					logger
							.error("Non Unique Result Error: wrong sql syntax or database not consistence!");
					logger.error("wrong sql is:" + op.getSql());
					logger.error("wrong ps is:" + ps);
				}
				return op.getResult();
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}

	private long sqlBegin() {
		// 记录sql执行的开始时间
		if (logger.isDebugEnabled())
			return System.currentTimeMillis();
		return 0;
	}

	/**
	 * @param info
	 *            需要在输出的内容
	 * @param begin是BatchSql执行的开始时间
	 *            单位是毫秒
	 */

	protected void batchEnd(final long begin, final String info) {
		log(CMD_BATCH, begin, info);
	}

	/**
	 * @param op
	 *            是一个op对象
	 * @param begin
	 *            begin是BatchSql执行的开始时间 单位是毫秒
	 */
	protected void sqlEnd(final long begin, final Op op) {
		log(CMD_SQL, begin, op.getBizName(), op.getSql());
	}

	/**
	 * @param cmd
	 *            标识是处理单条SQL还是批处理 标识符分别为 sql和batch
	 * @param begin是Sql执行的开始时间
	 *            单位是毫秒
	 * @param infos是要打印出来的内容对于batch而言
	 *            ，为空
	 */
	protected void log(final String cmd, final long begin, String... infos) {
		if (begin > 0 && logger.isDebugEnabled()) {
			long end = System.currentTimeMillis();
			long cost = end - begin;
			StringBuilder logBuilder = new StringBuilder();
			logBuilder.append("|");
			logBuilder.append(cmd.trim());

			logBuilder.append("|");
			logBuilder.append(cost);

			logBuilder.append("|");
			logBuilder.append(end);

			for (String str : infos) {
				logBuilder.append("|");
				logBuilder.append(str.trim());
			}
			logBuilder.append("|");
			logger.debug(logBuilder.toString());
		}
	}

	public int update(final OpUpdate op) throws SQLException {
		if (ConnCounterMgr.enter(op.getBizName())) {
			PreparedStatement ps = null;
			ResultSet rs = null;
			Connection conn = null;
			long begin = 0L;
			try {
				// 记录sql执行的开始时间
				begin = sqlBegin();
				// 从不同的数据源获得connection
				conn = getConn('w', op);
				ps = conn.prepareStatement(op.getSql());
				op.setParam(ps);
				// op.log(logger);
				return ps.executeUpdate();
			} finally {
				ConnCounterMgr.outer(op.getBizName());
				closeRSC(rs, ps, conn);
				sqlEnd(begin, op);
			}
		} else {
			throw new SQLException("数据库堵塞异常:\t"
					+ ConnCounterMgr.get(op.getBizName()));
		}
	}
}
