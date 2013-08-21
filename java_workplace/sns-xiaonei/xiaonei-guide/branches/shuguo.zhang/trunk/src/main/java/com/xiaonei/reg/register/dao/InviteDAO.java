package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.Invite;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.xce.XceAdapter;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;

public class InviteDAO extends XnDAO {
	public static final Logger log = Logger.getLogger("regLog");
	/**
	 * 这个是邀请DAO，与之相关联的表是invite_list 字段及其含义： 1.owner_id:发送邀请的人的ID
	 * 2.owner_name:发送邀请的人的名称 3.account:被邀请的人的帐户（Email） 4.flag:邀请的类型
	 * <p>
	 * 0：仅保存的邀请链接 1：已经成功发送的邀请链接 2：成功发送并且已经点击邀请链接返回的邀请链接 3:
	 * 用户在页面将邀请记录设置为不可见以后将不在页面上显示//add by 康伟
	 * </p>
	 * 5.tinyurl： 6.from_account:发送邀请的人的帐户（目前是Email帐户） 7.headurl：头像连接
	 * 8.action_type 具体参照 com.dodoyo.invite.logic.importer.ImportStrategyFactory
	 */
	private static InviteDAO instance = new InviteDAO();

	public static InviteDAO getInstance() {
		return instance;
	}

	private InviteDAO() {
		super();
	}

	/**
	 * 获得对数据库的链接
	 * 
	 * @param isWrite
	 * @return
	 * @throws java.sql.SQLException
	 */
	public static Connection getConn(boolean isWrite)
			throws java.sql.SQLException {
		if (isWrite) {
			return XceAdapter.getInstance().getWriteConnection(
					XceAdapter.DB_STAT);
		} else {
			return XceAdapter.getInstance().getReadConnection(
					XceAdapter.DB_STAT);
		}
	}

	// 保存邀请
	private final static String sqlInsert = "insert ignore into invite_list (owner_id, owner_name, account, flag, tinyurl, from_account, headurl) values (?,?,?,?,?,?,?)";

	@Deprecated
	// Deprecated by 康伟 只是为了适应老的MSN邀请，新的邀请请使用saveWithActionType
	public int save(final int owner_id, final String owner_name,
			final String account, final int flag, final String tinyurl,
			final String from_account, final String headurl)
			throws SQLException {
		OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, owner_id);
				ps.setString(2, owner_name);
				ps.setString(3, account);
				ps.setInt(4, flag);
				ps.setString(5, tinyurl);
				ps.setString(6, from_account);
				ps.setString(7, headurl);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	// 保存邀请 add by 康伟 适应新的需求：记录是邀请或者查找 是MSN或者WEBMAIL或者其他方式，具体情况参见类注释
	private final static String sqlInsertWithActionType = "insert ignore into invite_list (owner_id, owner_name, account, flag, tinyurl, from_account, headurl,action_type,app_id, nickname) values (?,?,?,?,?,?,?,?,?,?)";

	public int saveWithActionType(final int owner_id, final String owner_name,
			final String account, final int flag, final String tinyurl,
			final String from_account, final String headurl,
			final int action_type, final int appId, final String nickname)
			throws SQLException {
		OpUpdate op = new OpUpdate(sqlInsertWithActionType,
				DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, owner_id);
				ps.setString(2, owner_name);
				ps.setString(3, account);
				ps.setInt(4, flag);
				ps.setString(5, tinyurl);
				ps.setString(6, from_account);
				ps.setString(7, headurl);
				ps.setInt(8, action_type);
				ps.setInt(9, appId);
				ps.setString(10, nickname);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	private final static String sqlUpdateStatusWithNickName = "UPDATE invite_list SET flag=? ,nickname=? WHERE id=?";

	public int updateInviteStatusWithNickname(final int id, final int status,
			final String nickname) throws SQLException {

		OpUpdate op = new OpUpdate(sqlUpdateStatusWithNickName,
				DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, status);
				ps.setString(2, nickname);
				ps.setInt(3, id);
			}
		};

		return DataAccessMgr.getInstance().update(op);
	}

	private final static String sqlUpdateStatus = "UPDATE invite_list SET `flag`=? WHERE `id`=?";

	public int updateInviteStatus(final int id, final int status)
			throws SQLException {
		OpUpdate op = new OpUpdate(sqlUpdateStatus, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, status);
				ps.setInt(2, id);
			}
		};

		return DataAccessMgr.getInstance().update(op);
	}

	private final static String sqlUpdateStatusForUserId = "update invite_list set flag = ?, invitee_user_id  = ? where id = ?";

	public int updateInviteStatusForUserId(final int id,
			final int invitee_user_id, final int status) throws SQLException {
		OpUpdate op = new OpUpdate(sqlUpdateStatusForUserId,
				DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, status);
				ps.setInt(2, invitee_user_id);
				ps.setInt(3, id);
			}
		};

		return DataAccessMgr.getInstance().update(op);
	}

	/**
	 * 更新状态位，多加了一个字段hostid
	 * 
	 * @param id
	 *            当前用户id
	 * @param hostid
	 * @param status
	 * @return
	 * @throws SQLException
	 */
	public int updateInviteStatus(final int id, final int hostid,
			final int status) throws SQLException {
		OpUpdate op = new OpUpdate(sqlUpdateStatus, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, status);
				ps.setInt(2, hostid);
				ps.setInt(3, id);
			}
		};

		return DataAccessMgr.getInstance().update(op);
	}

	/**
	 * 查看是否有重复数据 康伟添加
	 */
	private final static String sqlCheckSelect = "SELECT * FROM `invite_list` WHERE `owner_id`=? AND `account`=? LIMIT 0, 1";

	public InviteInfo checkSelect(final int owner_id, final String account)
			throws SQLException {
		OpUniq op = new OpUniq(sqlCheckSelect, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, owner_id);
				ps.setString(2, account);
			}

			@Override
			public InviteInfo parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (InviteInfo) DataAccessMgr.getInstance().queryUnique(op);
	}

	private final static String sqlGetOneNewlyInviteInfo = "SELECT * FROM `invite_list` WHERE `owner_id`=? AND `account`=? AND `flag`=1 ORDER BY id DESC LIMIT 0, 1";

	public InviteInfo getInviteInfo(final int inviteId,
			final String invitedEmail) throws SQLException {
		OpUniq op = new OpUniq(sqlGetOneNewlyInviteInfo, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviteId);
				ps.setString(2, invitedEmail);
			}

			@Override
			public InviteInfo parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (InviteInfo) DataAccessMgr.getInstance().queryUnique(op);
	}

	/**
	 * 根据发送邀请者的ID读取flag=1的被邀请的用户
	 * 
	 * @owner_Id 邀请者的ID
	 * @offset 查询数据库记录的起始未知
	 * @limit 查询数据库的结束
	 * @author seraph
	 */
	private final static String sqlGetInviteInfoByOwnerId = "SELECT * FROM `invite_list` WHERE `owner_id`=? AND `flag`=? LIMIT ?,?";

	@SuppressWarnings("unchecked")
	public List<InviteInfo> getInviteInfoByOwnerId(final int owner_id,
			final int flag, final int offset, final int limit)
			throws SQLException {
		OpList op = new OpList(sqlGetInviteInfoByOwnerId,
				DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, owner_id);
				ps.setInt(2, flag);
				ps.setInt(3, offset);
				ps.setInt(4, limit);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (List<InviteInfo>) DataAccessMgr.getInstance().queryList(op);
	}

	private final static String sqlGetInviteInfoById = "SELECT * FROM `invite_list` WHERE `id`=? AND `flag`=1 ORDER BY id DESC LIMIT 0, 1";

	public InviteInfo getInviteInfoById(final int id) throws SQLException {
		OpUniq op = new OpUniq(sqlGetInviteInfoById, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}

			@Override
			public InviteInfo parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (InviteInfo) DataAccessMgr.getInstance().queryUnique(op);
	}

	private final static String sqlGetInviteCount = "SELECT COUNT(id) FROM `invite_list` WHERE `account`=? AND `flag`=1";

	public int getInviteCount(final String email) throws SQLException {
		OpUniq op = new OpUniq(sqlGetInviteCount, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, email);
			}
		};
		return DataAccessMgr.getInstance().queryInt(op);
	}

	/**
	 * 根据被邀请的Email读取数据库表的记录
	 * 
	 * @email 被邀请的Email
	 */
	private final static String sqlGetInviteUserByEmail = "SELECT * FROM `invite_list` WHERE `account`=? AND `flag`=1";

	@SuppressWarnings("unchecked")
	public List<InviteInfo> getInviteUsers(final String email)
			throws SQLException {

		OpList op = new OpList(sqlGetInviteUserByEmail, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, email);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (ArrayList<InviteInfo>) DataAccessMgr.getInstance()
				.queryList(op);
	}

	private final static String sqlGetNotInviteUserByEmail = "SELECT * FROM `invite_list` WHERE `account`=? AND `flag`=0";

	@SuppressWarnings("unchecked")
	public List<InviteInfo> getNotInviteUsers(final String email)
			throws SQLException {

		OpList op = new OpList(sqlGetNotInviteUserByEmail,
				DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, email);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (ArrayList<InviteInfo>) DataAccessMgr.getInstance()
				.queryList(op);
	}

	// private final static String sqlGetMSNFriendsByEmail =
	// "SELECT * FROM `invite_list` WHERE `owner_id`=? AND `flag`=0 LIMIT 0, 10";
	// //康伟注释
	private final static String sqlGetMSNFriendsByEmail = "SELECT * FROM `invite_list` WHERE `owner_id`=? LIMIT 0, 10";

	@SuppressWarnings("unchecked")
	public List<InviteInfo> getMSNFriends(final int userId) throws SQLException {

		OpList op = new OpList(sqlGetMSNFriendsByEmail, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (ArrayList<InviteInfo>) DataAccessMgr.getInstance()
				.queryList(op);
	}

	/**
	 * 下面的描述将以"寻找好友"这个名称统称"寻找好友"以及"邀请好友"两个功能，就是说，当提及"寻找好友"的时候，其实是在说这两个功能.
	 * 因为它们只是在概念上有区别
	 * <p/>
	 * <b>本接口实现这样一个功能：查询谁通过寻找好友寻找过我</b>
	 * <p/>
	 * 假设有这样一个好友关系：A<b>用户</b>的MSN里有一个<b>好友</b>B,B并没有注册在系统中
	 * 当A用户使用了寻找好友功能，并且允许系统记录了他的好友关系，系统就会知道B的EMAIL地址。当B以后注册到系统的时候，通过本接口可以将
	 * <b>用户</b>A推荐给<b>用户</b>B
	 * 
	 * @param email
	 *            B用户的电子邮件地址
	 * @return List<InviteInfo> 曾经使用寻找好友功能寻找过B的用户信息集合
	 * @throws SQLException
	 */
	private final static String sqlInvitedUserGetInvitoer = "SELECT * FROM `invite_list` WHERE `account`=?";

	@SuppressWarnings("unchecked")
	public List<InviteInfo> invitedGetInvitoer(final String email)
			throws SQLException {

		OpList op = new OpList(sqlInvitedUserGetInvitoer,
				DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, email);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (ArrayList<InviteInfo>) DataAccessMgr.getInstance()
				.queryList(op);
	}

	// private final static String sqlInviteduserGetInvitoer =
	// "select * from invite_list where account = ? and flag = ?";
	/**
	 * 根据邀请标志位返回邀请者信息
	 * 
	 * @param email
	 *            邀请者的email
	 * @param flag
	 *            标志位 @see invite_list 表结构中flag意义
	 * @return InviteInfo对象,发信者的信息
	 * @throws SQLException
	 *             查询异常
	 */
	@SuppressWarnings("unchecked")
	public List<InviteInfo> invitedGetInvitoer(final String email,
			final int flag) throws SQLException {

		OpList op = new OpList(sqlInvitedUserGetInvitoer,
				DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, email);
				ps.setInt(2, flag);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView(rs);
			}
		};
		return (ArrayList<InviteInfo>) DataAccessMgr.getInstance()
				.queryList(op);
	}

	/**
	 * 读取invite_list_2中的总记录条数
	 */

	private static final String INSERT_INTO_FRIEND_GROUP = "insert into invite_list_group (hostid, groupid, guestemail, groupname) values (?,?,?,?)";

	/**
	 * 为好友分组添加的保存信息
	 * 
	 * @param hostId
	 *            用户的id
	 * @param groupId
	 *            分组的id
	 * @param friendMail
	 *            被邀请者的email
	 * @return 数据库插入结果
	 * @throws SQLException
	 */
	public int saveFriendGroup(final int hostId, final int groupid,
			final String friendMail, final String groupname)
			throws SQLException {
		OpUpdate op = new OpUpdate(INSERT_INTO_FRIEND_GROUP,
				DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, hostId);
				ps.setInt(2, groupid);
				ps.setString(3, friendMail);
				ps.setString(4, groupname);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	private static final String sql_1 = "select * from invite_list_2 where flag = 1 limit 1000";

	/**
	 * 每次读取1000条记录
	 * 
	 * @return
	 * @throws SQLException
	 */
	@SuppressWarnings("unchecked")
	public List<InviteInfo> getMail() throws SQLException {
		OpList op = new OpList(sql_1, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
			}

			@Override
			public InviteInfo parse(ResultSet rs) throws SQLException {
				return parseInviteInfoView_2(rs);
			}
		};
		log.info("------------------------>批量选取成功");
		return (ArrayList<InviteInfo>) DataAccessMgr.getInstance()
				.queryList(op);
	}

	private InviteInfo parseInviteInfoView(ResultSet rs) throws SQLException {
		InviteInfo invite = new InviteInfo();
		invite.setId(rs.getInt("id"));
		invite.setOwner_id(rs.getInt("owner_id"));
		invite.setOwner_name(rs.getString("owner_name"));
		invite.setTinyurl(rs.getString("tinyurl"));
		invite.setAccount(rs.getString("account"));
		invite.setFlag(rs.getInt("flag"));
		invite.setFrom_account(rs.getString("from_account"));
		invite.setNickname(rs.getString("nickname"));
		invite.setApp_id(rs.getInt("app_id"));
		return invite;
	}

	private InviteInfo parseInviteInfoView_2(ResultSet rs) throws SQLException {
		InviteInfo invite = new InviteInfo();
		invite.setId(rs.getInt("id"));
		invite.setOwner_id(rs.getInt("owner_id"));
		invite.setOwner_name(rs.getString("owner_name"));
		invite.setTinyurl(rs.getString("tinyurl"));
		invite.setAccount(rs.getString("account"));
		invite.setFlag(rs.getInt("flag"));
		invite.setFrom_account(rs.getString("from_account"));
		invite.setNickname(rs.getString("nickname"));
		invite.setApp_id(rs.getInt("app_id"));
		invite.setInvite_time(rs.getDate("invite_time"));
		return invite;
	}

	private static final String SELECT_FRIEND_GROUP = "select groupname from invite_list_group where hostid = ? and guestemail = ?";

	/**
	 * 得到邀请时使用的分组
	 * 
	 * @param invterId
	 *            邀请人id
	 * @param inviteeMail
	 *            被邀请人邮件
	 * @return
	 * @throws SQLException
	 */
	public String getFriendGroup(final int invterId, final String inviteeMail)
			throws SQLException {
		OpUniq op = new OpUniq(SELECT_FRIEND_GROUP, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, invterId);
				ps.setString(2, inviteeMail);
			}

			@Override
			public String parse(ResultSet rs) throws SQLException {
				return rs.getString("groupname");
			}
		};
		return (String) DataAccessMgr.getInstance().queryUnique(op);
	}

	public Invite get(final int userId) throws SQLException {
		String sqlGet = "select * from invite_code where userid = ?";
		OpUniq op = new OpUniq(sqlGet, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseInvite(rs);
			}

		};
		return (Invite) DataAccessMgr.getInstance().queryUnique(op);
	}

	private Invite parseInvite(ResultSet rs) throws SQLException {
		Invite invite = new Invite();
		invite.setUserId(rs.getInt("userid"));
		invite.setInvitecode(rs.getString("invite_code"));
		return invite;
	}
}