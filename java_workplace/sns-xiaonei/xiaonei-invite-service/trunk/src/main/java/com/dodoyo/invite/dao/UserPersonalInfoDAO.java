package com.dodoyo.invite.dao;

import com.dodoyo.invite.model.PersonalLinkView;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import mop.hi.oce.domain.model.UserPersonalInfo;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class UserPersonalInfoDAO extends XnDAO {

//    private static UserPersonalInfoDAO instance = new UserPersonalInfoDAO();

    /*private static UserPersonalInfoDAO getInstance() {
        if (instance == null) {
            synchronized (UserPersonalInfoDAO.class) {
                if (instance == null)
                    instance = new UserPersonalInfoDAO();
            }
        }
        return instance;
    }*/

    private UserPersonalInfoDAO() {
        super();
    }

    private final static String sqlGetPersonalLinkByLink = "select id, link, linkstatus from user_personalinfo where link = ?";

    public PersonalLinkView getPersonalLinkByLink(final String link)
            throws SQLException {
        OpUniq op = new OpUniq(sqlGetPersonalLinkByLink,
                DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, link.toLowerCase());
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                PersonalLinkView plv = new PersonalLinkView();
                plv.setId(rs.getInt("id"));
                plv.setLink(rs.getString("link"));
                plv.setStatus(rs.getInt("linkstatus"));
                return plv;
            }

        };
        return (PersonalLinkView) DataAccessMgr.getInstance().queryUnique(op);
    }

    /**
     * fusong.li@opi-corp.com add
     */
    private final static String sqlGetPersonalLinkByLinkSource = "select id, link, linkstatus from user_personalinfo where link = ? and source= ? ";

    public PersonalLinkView getPersonalLinkByLinkSource(final String link,
                                                        final int source) throws SQLException {
        OpUniq op = new OpUniq(sqlGetPersonalLinkByLinkSource,
                DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, link.toLowerCase());
                ps.setInt(2, source);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                PersonalLinkView plv = new PersonalLinkView();
                plv.setId(rs.getInt("id"));
                plv.setLink(rs.getString("link"));
                plv.setStatus(rs.getInt("linkstatus"));
                return plv;
            }

        };
        return (PersonalLinkView) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlGetPersonalLinkByLinkNot5q = "select id, link, linkstatus from user_personalinfo where link = ? and source != ? ";

    public PersonalLinkView getPersonalLinkByLinkNot5q(final String link,
                                                       final int source) throws SQLException {
        OpUniq op = new OpUniq(sqlGetPersonalLinkByLinkNot5q,
                DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, link.toLowerCase());
                ps.setInt(2, source);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                PersonalLinkView plv = new PersonalLinkView();
                plv.setId(rs.getInt("id"));
                plv.setLink(rs.getString("link"));
                plv.setStatus(rs.getInt("linkstatus"));
                return plv;
            }

        };
        return (PersonalLinkView) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlGetPersonalLinkByUser = "select id, link, linkstatus from user_personalinfo where id = ?";

    public PersonalLinkView getPersonalLinkByUser(final int userId)
            throws SQLException {
        OpUniq op = new OpUniq(sqlGetPersonalLinkByUser,
                DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                PersonalLinkView plv = new PersonalLinkView();
                plv.setId(rs.getInt("id"));
                plv.setLink(rs.getString("link"));
                plv.setStatus(rs.getInt("linkstatus"));
                return plv;
            }

        };
        return (PersonalLinkView) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlUserInfoExist = "select id from user_personalinfo where id = ?";

    public boolean userInfoExist(final int userId) throws SQLException {
        OpUniq op = new OpUniq(sqlUserInfoExist, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }

        };
        return DataAccessMgr.getInstance().queryExist(op);
    }

    private final static String sqlGetSpaceAll = "select space_all from user_personalinfo where id = ?";

    public int getSpaceAll(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetSpaceAll, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlInsertInviteCode = "insert into user_personalinfo (id, invitecode, source) values (?, ?, 0)";

    public int insertInviteCode(final int id, final String invitecode)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertInviteCode,
                DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
                ps.setString(2, invitecode);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    /*
      * private final static String sqlInsertLastIpLong =
      * "insert into user_personalinfo (id, lastip_long, source) values (?, ?, 0)"
      * ;
      *
      * public int insertLastIpLong(final int id, final long iplong) throws
      * SQLException { OpUpdate op = new OpUpdate(sqlInsertLastIpLong) {
      *
      * @Override public void setParam(PreparedStatement ps) throws SQLException
      * { ps.setInt(1, id); ps.setLong(2, iplong); }
      *
      * }; return update(op); }
      */
    private final static String sqlInsertLink = "insert into user_personalinfo (id, link, linkstatus,source) values (?, ?, ?, ?)";

    public int insertLink(final int id, final String link, final int state,
                          final int source) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertLink, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
                ps.setString(2, link);
                ps.setInt(3, state);
                ps.setInt(4, source);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlUpdateInviteCode = "update user_personalinfo set invitecode = ? where id = ?";

    public int updateInviteCode(final int id, final String invitecode)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateInviteCode,
                DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, invitecode);
                ps.setInt(2, id);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    /*
      * private final static String sqlUpdateLastIpLong =
      * "update user_personalinfo set lastip_long = ? where id = ?";
      *
      * public int updateLastIpLong(final int id, final long iplong) throws
      * SQLException { OpUpdate op = new OpUpdate(sqlUpdateLastIpLong) {
      *
      * @Override public void setParam(PreparedStatement ps) throws SQLException
      * { ps.setLong(1, iplong); ps.setInt(2, id); }
      *
      * }; try { AdapterFactory.getUserPersonalInfoAdapter().updateLastIpLong(id,
      * iplong); } catch (Exception e) { this.getLog().error("ICE:", e); } return
      * update(op); }
      */
    private final static String sqlUpdateLinkState = "update user_personalinfo set linkstatus = ? where id = ?";

    public int updateLinkState(final int id, final int state)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateLinkState, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, state);
                ps.setInt(2, id);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlUpdateLink = "update user_personalinfo set link = ?, linkstatus = ? where id = ?";

    public int updateLink(final int id, final String link, final int state)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateLink, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, link);
                ps.setInt(2, state);
                ps.setInt(3, id);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    public void save(UserPersonalInfo info) {
	}
}
