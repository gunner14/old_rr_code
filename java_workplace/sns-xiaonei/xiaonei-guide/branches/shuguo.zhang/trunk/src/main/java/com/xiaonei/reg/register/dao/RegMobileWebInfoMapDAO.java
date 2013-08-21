/**
 *
 */
package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.MobileEmailVerify;
import com.xiaonei.reg.register.model.RegMobileWebInfoMapHS;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * tablename:reg_mobile_web_info_map
 * <p/>
 * RegMobileWebInfoMapDAO.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-11 上午11:42:13
 */
public class RegMobileWebInfoMapDAO extends XnDAO {

    private static RegMobileWebInfoMapDAO instance = new RegMobileWebInfoMapDAO();

    public static RegMobileWebInfoMapDAO getInstance() {
        return instance;
    }

    private RegMobileWebInfoMapDAO() {
        super();
    }

    // 25项
    private final static String view_columns = "mobile,down_code,sys_status,sys_ip,u_name,u_basic_control,u_stage,u_userid,u_gender,u_password,hs_school_type,hs_schoolname,hs_student_number,hs_enrollyear,hs_high_school_class,head_largeurl,head_mainurl,head_headurl,head_tinyurl,iv_inviter_id,iv_app_id,iv_inviter_group_name,iv_app_token,iv_invitee_email,hs_schoolid";

    private final static String sqlRegMobileInfoHs_view = "select id," + view_columns
            + " from reg_mobile_web_info_map ";

    private final static String sqlGetRegMobileInfoByCodeHs = sqlRegMobileInfoHs_view
            + " where mobile=? and down_code=?";

    private final static String sqlGetRegMobileInfoLatestHs = sqlRegMobileInfoHs_view
            + " where mobile=? order by id desc limit 1 ";

    // private final static String sqlGetRegMobileInfoByDateHs =
    // sqlRegMobileInfoHs_view
    // +
    // " where mobile=? and DATE_FORMAT(sys_begin_time, '%D %M %Y')=DATE_FORMAT(today(), '%D %M %Y')";
    private final static String sqlGetRegMobileInfoByMobileHs = sqlRegMobileInfoHs_view
            + " where mobile=? ";

    private final static String sqlAddRegMobileInfoHs = "insert into reg_mobile_web_info_map ("
            + view_columns
            + " ,sys_begin_time) values (?, ?,?, ?,?, ?,?, ?, ?,?, ?,?, ?,?, ?,?,?,?,?,?,?,?,?,?,?,now())";

    private final static String sqlUpdateRegMobileInfo = "UPDATE reg_mobile_web_info_map SET u_userid=? ,sys_status=? ,sys_end_time=now() WHERE mobile=? and down_code=?";

    /**
     * @param status
     * @param mobile
     * @param down_code
     * @return
     * @throws SQLException
     */
    public int insertRegMobileInfo(final int status, final String mobile, final String down_code)
            throws SQLException {
        final String sqlInsertRegMobileCode = "insert into reg_mobile_web_info_map  (sys_status ,mobile,down_code,sys_end_time) values (?,?,?,now())";
        OpUpdate op = new OpUpdate(sqlInsertRegMobileCode, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setLong(1, status);
                ps.setString(2, mobile);
                ps.setString(3, down_code);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    public int delRegMobileInfo(final int id) throws SQLException {
        String sqlDelRegMobileInfo = "delete from reg_mobile_web_info_map where id=?";
        OpUpdate op = new OpUpdate(sqlDelRegMobileInfo, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setLong(1, id);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    /**
     * 更新
     * 
     * @param userid
     * @param status
     * @param moblie
     * @param down_code
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 下午04:55:53
     */
    public int updateRegMobileInfo(final int userid, final int status, final String mobile,
            final String down_code) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateRegMobileInfo, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setLong(1, userid);
                ps.setLong(2, status);
                ps.setString(3, mobile);
                ps.setString(4, down_code);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    /**
     * 更新status
     * 
     * @param status
     * @param mobile
     * @param down_code
     * @return
     * @throws SQLException
     */
    public int updateStatus(final int status, final String mobile, final String down_code)
            throws SQLException {
        final String sqlUpdateStatus = "UPDATE reg_mobile_web_info_map SET sys_status=? ,sys_end_time=now() WHERE mobile=? and down_code=?";
        OpUpdate op = new OpUpdate(sqlUpdateStatus, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setLong(1, status);
                ps.setString(2, mobile);
                ps.setString(3, down_code);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    /**
     * 获得高中用户信息，通过mobile number 和down code
     * 
     * @param mobile
     * @param downCode
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 下午12:06:12
     */
    public RegMobileWebInfoMapHS getRegMobileInfoByMobileAndCode_hs(final String mobile,
            final String downCode) throws SQLException {
        OpUniq op = new OpUniq(sqlGetRegMobileInfoByCodeHs, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, mobile);
                ps.setString(2, downCode);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseRegMobileInfo_hs(rs);
            }

        };
        return (RegMobileWebInfoMapHS) DataAccessMgr.getInstance().queryUnique(op);
    }

    /**
     * 获得高中用户信息，通过mobile number
     * 
     * @param mobile
     * @return RegMobileWebInfoMapHS
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 下午12:13:17
     */
    public RegMobileWebInfoMapHS getRegMobileInfoByMobile_hs(final String mobile)
            throws SQLException {
        OpUniq op = new OpUniq(sqlGetRegMobileInfoByMobileHs, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, mobile);
            }

            @Override
            public RegMobileWebInfoMapHS parse(ResultSet rs) throws SQLException {
                return parseRegMobileInfo_hs(rs);
            }

        };
        return (RegMobileWebInfoMapHS) DataAccessMgr.getInstance().queryUnique(op);
    }

    /**
     * 获得用户信息，通过mobile number
     * 
     * @param mobile
     * @return RegMobileWebInfoMapHS
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 下午12:13:17
     */
    public String getDownCode(final String mobile) throws SQLException {
        final String sqlGetDownCode = "select down_code from reg_mobile_web_info_map  where mobile=?  ";
        OpUniq op = new OpUniq(sqlGetDownCode, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, mobile);
            }
            
            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return rs.getString("down_code");
            }
        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);
    }

    /**
     * 最新高中用户信息，通过mobile number
     * 
     * @param mobile
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 下午12:13:17
     */
    public RegMobileWebInfoMapHS getRegMobileInfoLatest_hs(final String mobile) throws SQLException {
        OpUniq op = new OpUniq(sqlGetRegMobileInfoLatestHs, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, mobile);
            }

            @Override
            public RegMobileWebInfoMapHS parse(ResultSet rs) throws SQLException {
                return parseRegMobileInfo_hs(rs);
            }

        };
        return (RegMobileWebInfoMapHS) DataAccessMgr.getInstance().queryUnique(op);
    }
    
    public static void main(String[] aa){
        System.out.println(sqlGetRegMobileInfoLatestHs);
    }
    
    /**
     * 保存高中用户信息 mobile
     * 
     * @param rm
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 下午03:44:17
     */
    public int AddRegMobileInfo_hs(final RegMobileWebInfoMapHS rm) throws SQLException {
        OpUpdate op = new OpUpdate(sqlAddRegMobileInfoHs, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                /*
                 * "mobile,down_code,sys_status,sys_ip,u_name,u_basic_control,
                 * u_stage
                 * ,u_userid,u_gender,u_password,hs_school_type,hs_schoolname,
                 * hs_student_number
                 * ,hs_enrollyear,hs_high_school_class,head_largeurl,
                 * head_mainurl
                 * ,head_headurl,head_tinyurl,iv_inviter_id,iv_app_id,
                 * iv_inviter_group_name
                 * ,iv_app_token,iv_invitee_email,hs_schoolid"
                 */
                ps.setString(1, rm.getMobile());
                ps.setString(2, rm.getDown_code());
                ps.setInt(3, rm.getSys_status());
                ps.setString(4, rm.getSys_ip());
                ps.setString(5, rm.getU_name());
                ps.setInt(6, rm.getU_basic_control());
                ps.setLong(7, rm.getU_stage());
                ps.setLong(8, rm.getU_userid());
                ps.setString(9, rm.getU_gender());
                ps.setString(10, rm.getU_password());
                ps.setLong(11, rm.getHs_school_type());
                ps.setString(12, rm.getHs_schoolname());
                ps.setString(13, rm.getHs_student_number());
                ps.setLong(14, rm.getHs_enrollyear());
                ps.setString(15, rm.getHs_high_school_class());
                ps.setString(16, rm.getHead_largeurl());
                ps.setString(17, rm.getHead_mainurl());
                ps.setString(18, rm.getHead_headurl());
                ps.setString(19, rm.getHead_tinyurl());
                ps.setLong(20, rm.getIv_inviter_id());
                ps.setLong(21, rm.getIv_app_id());
                ps.setString(22, rm.getIv_inviter_group_name());
                ps.setString(23, rm.getIv_app_token());
                ps.setString(24, rm.getIv_invitee_email());
                ps.setLong(25, rm.getHs_schoolid());
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    /**
     * 封装高中对象
     * 
     * @param rs
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 上午11:49:25
     */
    private RegMobileWebInfoMapHS parseRegMobileInfo_hs(ResultSet rs) throws SQLException {

        RegMobileWebInfoMapHS rm = new RegMobileWebInfoMapHS(rs.getInt("id"), rs
                .getString("down_code"), rs.getString("head_headurl"), rs
                .getString("head_largeurl"), rs.getString("head_mainurl"), rs
                .getString("head_tinyurl"), rs.getInt("hs_enrollyear"), rs
                .getString("hs_high_school_class"), rs.getInt("hs_school_type"), rs
                .getInt("hs_schoolid"), rs.getString("hs_schoolname"), rs
                .getString("hs_student_number"), rs.getInt("iv_app_id"), rs
                .getString("iv_app_token"), rs.getString("iv_invitee_email"), rs
                .getString("iv_inviter_group_name"), rs.getInt("iv_inviter_id"), rs
                .getString("mobile"), rs.getString("sys_ip"), rs.getInt("sys_status"), rs
                .getInt("u_basic_control"), rs.getString("u_gender"), rs.getString("u_name"), rs
                .getString("u_password"), rs.getInt("u_stage"), rs.getInt("u_userid"));
        return rm;
    }

    /**
     * 更新验证邮件
     * 
     * @param mobile
     * @param email
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-8 - 下午03:44:25
     */
    public int updateEmail(final String mobile, final String email) throws SQLException {
        final String sqlUpdateEmail = "insert INTO mobile_email_verify set mobile=?,email=?,status="
                + MobileEmailVerify.VERIFY_NEW
                + ",insert_date=now() ON DUPLICATE KEY UPDATE email=?,status="
                + MobileEmailVerify.VERIFY_UPDATE + ";";
        OpUpdate op = new OpUpdate(sqlUpdateEmail, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, mobile);
                ps.setString(2, email);
                ps.setString(3, email);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }
}
