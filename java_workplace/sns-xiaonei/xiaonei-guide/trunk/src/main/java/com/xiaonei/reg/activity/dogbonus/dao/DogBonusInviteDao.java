package com.xiaonei.reg.activity.dogbonus.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusInviteModel;

public class DogBonusInviteDao {
	public static DogBonusInviteDao getInstance() {
		return instance;
	}

	private DogBonusInviteDao() {
		super();
	}
	
	private static DogBonusInviteDao instance = new DogBonusInviteDao();
	
	private static final String sqlUpdateByInviterId = "UPDATE dog_bonus_invite set accept_flag = 1 where inviter_id = ?";
	public int updateByInviterid(final int inviterid) throws SQLException{

		OpUpdate op = new OpUpdate(sqlUpdateByInviterId, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviterid);
			}
		};
		return  DataAccessMgr.getInstance().update(op);
	}
	
	private static final String sqlUpdateByInviteeId = "UPDATE dog_bonus_invite set accept_flag = 1 where invitee_id = ?";
	public int updateByInviteeid(final int inviteeid) throws SQLException{

		OpUpdate op = new OpUpdate(sqlUpdateByInviteeId, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviteeid);
			}
		};
		return  DataAccessMgr.getInstance().update(op);
	}
	
	//TODO update不用now OK!
	private static final String sqlUpdateByInviterAndInviteeId = "UPDATE dog_bonus_invite set accept_flag = 1 where inviter_id = ? and invitee_id = ?";
	public int acceptByInviteridAndInviteeid(final int inviterid,final int inviteeid) throws SQLException{

		OpUpdate op = new OpUpdate(sqlUpdateByInviterAndInviteeId, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviterid);
				ps.setInt(2, inviteeid);
			}
		};
		return  DataAccessMgr.getInstance().update(op);
	}
	
	//TODO insert now OK!
	private static final String sqlInsertBatch = "INSERT INTO dog_bonus_invite ( inviter_id, invitee_id,invite_time) VALUES ( ?, ?, now() )";
	public int[] batchInsert(final List<DogBonusInviteModel> infos) throws SQLException{
		OpUpdate op = new OpUpdate(sqlInsertBatch, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
            	for (DogBonusInviteModel info : infos) {
					ps.setInt(1, info.getInviter_id());
					ps.setInt(2, info.getInvitee_id());
					ps.addBatch();
				}
                
            }
        };
		return  DataAccessMgr.getInstance().batchUpdate(op);
	}
	
	//TODO update不用now OK!
	private static final String sqlUpdateFlagBatch = "UPDATE dog_bonus_invite set accept_flag = 1 where id = ? and invitee_id = ?";
	public int[] batchUpdateFlag(final List<Integer> infos,final int inviteeid) throws SQLException{
		OpUpdate op = new OpUpdate(sqlUpdateFlagBatch, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
            	for (Integer info : infos) {
					ps.setInt(1, info);
					ps.setInt(2, inviteeid);
					ps.addBatch();
				}
                
            }
        };
		return  DataAccessMgr.getInstance().batchUpdate(op);
	}
	
	
	private static final String sqlSelectByInviteeIdForLink = "SELECT id, inviter_id, invitee_id, invite_time, accept_flag, accept_time FROM dog_bonus_invite where invitee_id = ? and accept_flag = 0 limit 6";
	@SuppressWarnings("unchecked")
	public List<DogBonusInviteModel> selectByInviteeIdForLink(final int inviteeid) throws SQLException {
		OpList op = new OpList(sqlSelectByInviteeIdForLink, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviteeid);
			}
			@Override
			public DogBonusInviteModel parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
		};
		return (List<DogBonusInviteModel>) DataAccessMgr.getInstance().queryList(op);
	}
	
	private static final String sqlSelectByInviterIdForLink = "SELECT id, inviter_id, invitee_id, invite_time, accept_flag, accept_time FROM dog_bonus_invite where inviter_id = ? and accept_flag = 0 limit 6";
	@SuppressWarnings("unchecked")
	public List<DogBonusInviteModel> selectByInviterIdForLink(final int inviterid) throws SQLException {
		OpList op = new OpList(sqlSelectByInviterIdForLink, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviterid);
			}
			@Override
			public DogBonusInviteModel parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
		};
		return (List<DogBonusInviteModel>) DataAccessMgr.getInstance().queryList(op);
	}
	
	
	private static final String sqlSelectByInviterId = "SELECT id, inviter_id, invitee_id, invite_time, accept_flag, accept_time FROM dog_bonus_invite where inviter_id = ?";
	@SuppressWarnings("unchecked")
	public List<DogBonusInviteModel> selectByInviterId(final int inviterid) throws SQLException {
		OpList op = new OpList(sqlSelectByInviterId, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviterid);
			}
			@Override
			public DogBonusInviteModel parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
		};
		return (List<DogBonusInviteModel>) DataAccessMgr.getInstance().queryList(op);
	}

	protected DogBonusInviteModel parseToModel(ResultSet rs) throws SQLException {
		DogBonusInviteModel model = new DogBonusInviteModel();
		model.setId(rs.getInt("id"));
		model.setInviter_id(rs.getInt("inviter_id"));
		model.setInvitee_id(rs.getInt("invitee_id"));
		model.setInvite_time(rs.getString("invite_time"));
		model.setAccept_flag(rs.getInt("accept_flag"));
		model.setAccept_time(rs.getString("accept_time"));
        return model;
	}
	//SELECT id, uid, act_type, act_amount, act_time from recall_bonus_account_record where uid = ? and act_type = 1 and datediff(now(), act_time) = 0 limit 1
	private static final String sqlSelectByInviterIdAndToday = "SELECT id, inviter_id, invitee_id, invite_time, accept_flag, accept_time FROM dog_bonus_invite where inviter_id = ? and datediff(now(), invite_time)=0 ";
	@SuppressWarnings("unchecked")
	public List<DogBonusInviteModel> selectByInviterIdAndToday(final int inviterid) throws SQLException {
		OpList op = new OpList(sqlSelectByInviterIdAndToday, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviterid);
			}
			@Override
			public DogBonusInviteModel parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
		};
		return (List<DogBonusInviteModel>) DataAccessMgr.getInstance().queryList(op);
	}

	private static final String sqlSelectByCanLinkInviterId = "SELECT distinct invitee_id FROM dog_bonus_invite where accept_flag = 0 limit ?";
	@SuppressWarnings("unchecked")
	public List<Integer> selectAllCanLinkInviteeId(int host,	final int limit) throws SQLException {
		OpList op = new OpList(sqlSelectByCanLinkInviterId, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, limit);
			}
			@Override
			public Integer parse(ResultSet rs) throws SQLException {
				return parseToInviteeInteger(rs);
			}
		};
		return (List<Integer>) DataAccessMgr.getInstance().queryList(op);
	}

	protected Integer parseToInviteeInteger(ResultSet rs) throws SQLException {
		return rs.getInt("invitee_id");
	}

	private static final String sqlSelectById = "SELECT id, inviter_id, invitee_id, invite_time, accept_flag, accept_time FROM dog_bonus_invite where id = ? limit 1";
	public DogBonusInviteModel selectById(final Integer id) throws SQLException {
		OpUniq op = new OpUniq(sqlSelectById, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}
			@Override
			public DogBonusInviteModel parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
		};
		return (DogBonusInviteModel) DataAccessMgr.getInstance().queryUnique(op);
	}
	
}
