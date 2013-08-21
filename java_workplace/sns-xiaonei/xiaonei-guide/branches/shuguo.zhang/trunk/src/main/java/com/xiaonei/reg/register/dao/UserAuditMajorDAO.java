/**
 *
 */
package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.UserAuditMajor;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * UserAuditMajorDAO
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-11-21 - 下午02:46:25
 */
public class UserAuditMajorDAO extends XnDAO {

    public static UserAuditMajorDAO getInstance() {
        return instance;
    }

    private UserAuditMajorDAO() {
        super();
    }

    private static UserAuditMajorDAO instance = new UserAuditMajorDAO();

    /**
     * 获得审核状态
     *
     * @param id
     * @return
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2008-11-21 - 下午06:31:19
     */
    public int getAuditStatus(final int id) throws SQLException {
        final String sqlGetAuditStatus = "select status from user_audit_major where id = ?";
        OpUniq op = new OpUniq(sqlGetAuditStatus, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }
        };
        int status = DataAccessMgr.getInstance().queryInt(op);
        return status;
    }

    /**
     * 通过审核表id获得审核记录
     *
     * @param id
     * @return
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2008-11-21 - 下午06:30:36
     */
    public UserAuditMajor getAudit(final int id) throws SQLException {
        final String sqlGetAudit = "select * from user_audit_major where id = ?";
        OpUniq op = new OpUniq(sqlGetAudit, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public UserAuditMajor parse(ResultSet rs) throws SQLException {
                return parseUserAuditMajor(rs);
            }

            private UserAuditMajor parseUserAuditMajor(ResultSet rs)
                    throws SQLException {
                int id = rs.getInt("id");
                int userid = rs.getInt("userid");
                String name = rs.getString("name");
                int univ_id = rs.getInt("univ_id");
                String univ_name = rs.getString("univ_name");
                int enrollyear = rs.getInt("enrollyear");
                String headurl = rs.getString("headurl");
                String mainurl = rs.getString("mainurl");
                String largeurl = rs.getString("largeurl");
                String workplaces = rs.getString("workplaces");
                String ip = rs.getString("ip");
                int status = rs.getInt("status");
                int audit_user = rs.getInt("audit_user");
                int invite_userid = rs.getInt("invite_userid");
                String gender = rs.getString("gender");

                String inviter_group_name = rs.getString("inviter_group_name");
                int app_id = rs.getInt("app_id");
                String app_token = rs.getString("app_token");
                String invite_type = rs.getString("invite_type");
                int basic_control = rs.getInt("basic_control");
                String passwod = rs.getString("password");//
                String email = rs.getString("email");//
                String tinyurl = rs.getString("tinyurl");//
                String home_city_code = rs.getString("home_city_code");//
                String home_city_name = rs.getString("home_city_name");//
                String home_province = rs.getString("home_province");//
                String workplace_id = rs.getString("workplace_id");//
                String invitee_email = rs.getString("invitee_email");//

                int stage = rs.getInt("stage");
                UserAuditMajor uam = new UserAuditMajor(id, userid, name,
                        univ_id, univ_name, enrollyear, headurl, mainurl,
                        largeurl, tinyurl, workplace_id, workplaces, ip,
                        status, audit_user, invite_userid, invitee_email,
                        gender, inviter_group_name, app_id, app_token,
                        invite_type, basic_control, home_city_code,
                        home_city_name, home_province, passwod, email, stage);
                return uam;
            }

        };
        return (UserAuditMajor) DataAccessMgr.getInstance().queryUnique(op);
    }

    /**
     * 保存到审核表中
     *
     * @param uam
     * @return id
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2008-11-21 - 下午06:29:39
     */
    public int save(final UserAuditMajor uam) throws SQLException {
        final String sqlInsert = "insert into user_audit_major set userid=?,name=?,password=?,email=?,gender=?,univ_id=?,univ_name=?,enrollyear=?,headurl=?,mainurl=?,largeurl=?,tinyurl=?,home_city_code=?,home_city_name=?,home_province=?,workplace_id=?,workplaces=?,ip=?,status=?,audit_user=?,invite_userid=?,invitee_email=?,inviter_group_name=?,app_id=?,app_token=?,invite_type=?,basic_control=?,begin_time=now(),stage=?";
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, uam.getUserid());
                ps.setString(2, uam.getName());
                ps.setString(3, uam.getPassword());
                ps.setString(4, uam.getEmail());
                ps.setString(5, uam.getGender());
                ps.setLong(6, uam.getUniv_id());
                ps.setString(7, uam.getUniv_name());
                ps.setLong(8, uam.getEnrollyear());
                ps.setString(9, uam.getHeadurl());
                ps.setString(10, uam.getMainurl());
                ps.setString(11, uam.getLargeurl());
                ps.setString(12, uam.getTinyeurl());
                ps.setString(13, uam.getHome_city_code());
                ps.setString(14, uam.getHome_city_name());
                ps.setString(15, uam.getHome_province());
                ps.setString(16, uam.getWorkplace_id());
                ps.setString(17, uam.getWorkplaces());
                ps.setString(18, uam.getIp());
                ps.setLong(19, UserAuditMajor.AUDIT_STATUS_NEW);
                ps.setLong(20, uam.getAudit_user());
                ps.setLong(21, uam.getInvite_userid());
                ps.setString(22, uam.getInvitee_email());
                ps.setString(23, uam.getInviter_group_name());
                ps.setLong(24, uam.getApp_id());
                ps.setString(25, uam.getApp_token());
                ps.setString(26, uam.getInvite_type());
                ps.setLong(27, uam.getBasic_control());
                ps.setLong(28, uam.getStage());

            }
        };
        return DataAccessMgr.getInstance().insertReturnId(op);
    }

    /**
     * 重新审核时更新属性
     *
     * @param uam
     * @return
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2008-11-24 - 上午11:35:15
     */
    public int updateAudit(final UserAuditMajor uam) throws SQLException {
        final String sqlUpdate = "update user_audit_major set userid=?,name=?,password=?,email=?,gender=?,univ_id=?,univ_name=?,enrollyear=?,headurl=?,mainurl=?,largeurl=?,tinyurl=?,home_city_code=?,home_city_name=?,home_province=?,workplace_id=?,workplaces=?,ip=?,status=?,audit_user=?,invite_userid=?,invitee_email=?,inviter_group_name=?,app_id=?,app_token=?,invite_type=?,basic_control=?,begin_time=now(),end_time=now() where id=?";
        OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, uam.getUserid());
                ps.setString(2, uam.getName());
                ps.setString(3, uam.getPassword());
                ps.setString(4, uam.getEmail());
                ps.setString(5, uam.getGender());
                ps.setLong(6, uam.getUniv_id());
                ps.setString(7, uam.getUniv_name());
                ps.setLong(8, uam.getEnrollyear());
                ps.setString(9, uam.getHeadurl());
                ps.setString(10, uam.getMainurl());
                ps.setString(11, uam.getLargeurl());
                ps.setString(12, uam.getTinyeurl());
                ps.setString(13, uam.getHome_city_code());
                ps.setString(14, uam.getHome_city_name());
                ps.setString(15, uam.getHome_province());
                ps.setString(16, uam.getWorkplace_id());
                ps.setString(17, uam.getWorkplaces());
                ps.setString(18, uam.getIp());
                ps.setLong(19, UserAuditMajor.AUDIT_STATUS_NEW);
                ps.setLong(20, uam.getAudit_user());
                ps.setLong(21, uam.getInvite_userid());
                ps.setString(22, uam.getInvitee_email());
                ps.setString(23, uam.getInviter_group_name());
                ps.setLong(24, uam.getApp_id());
                ps.setString(25, uam.getApp_token());
                ps.setString(26, uam.getInvite_type());
                ps.setLong(27, uam.getBasic_control());
                ps.setLong(28, uam.getId());
			}
		};
		return DataAccessMgr.getInstance().update(op);

	}
}
