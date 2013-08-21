package com.xiaonei.reg.jump.data.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.jump.data.model.MlinkRegToGameMapModel;


/**
 * MlinkRegToGameMapDAO.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 创建文件 
 */
public class MlinkRegToGameMapDAO {

    public static MlinkRegToGameMapDAO getInstance() {
        return instance;
    }

    private MlinkRegToGameMapDAO() {
        super();
    }

    private static MlinkRegToGameMapDAO instance = new MlinkRegToGameMapDAO();

    private static final String TABLE_NAME = "mlink_reg_to_game_map";

    protected MlinkRegToGameMapModel parseToModel(ResultSet rs) throws SQLException {
        MlinkRegToGameMapModel model = new MlinkRegToGameMapModel();
        model.setId(rs.getInt("id"));
        model.setSs_id(rs.getInt("ss_id"));
        model.setRt_id(rs.getInt("rt_id"));
        model.setX_id(rs.getInt("x_id"));
        model.setY_id(rs.getInt("y_id"));
        model.setGame_stat_id(rs.getInt("game_stat_id"));
        model.setCode_id(rs.getString("code_id"));
        model.setTo_url(rs.getString("to_url"));
        model.setRemark(rs.getString("remark"));
        model.setInsert_time(rs.getString("insert_time"));
        model.setUpdate_time(rs.getString("update_time"));
        return model;
    }
    
    private static final String sqlSelect = "select id,ss_id,rt_id,x_id,y_id,game_stat_id,code_id,to_url,remark,insert_time,update_time from "+TABLE_NAME+"";
    @SuppressWarnings("unchecked")
    public List<MlinkRegToGameMapModel> select()throws SQLException {
        OpList op = new OpList(sqlSelect, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
            }
            
            @Override
            public MlinkRegToGameMapModel parse(ResultSet rs) throws SQLException {
                return parseToModel(rs);
            }
        };
        return (List<MlinkRegToGameMapModel>)DataAccessMgr.getInstance().queryList(op);
    }
    
    private static final String sqlSelectBySsIdAndRtId = "select id,ss_id,rt_id,x_id,y_id,game_stat_id,code_id,to_url,remark,insert_time,update_time from "+TABLE_NAME+" where ss_id = ? and rt_id = ?";
    public MlinkRegToGameMapModel selectBySsIdAndRtId(final int ss_id,final int rt_id)throws SQLException {
        OpUniq op = new OpUniq(sqlSelectBySsIdAndRtId, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, ss_id);
                ps.setInt(2, rt_id);
            }
            
            @Override
            public MlinkRegToGameMapModel parse(ResultSet rs) throws SQLException {
                return parseToModel(rs);
            }
        };
        return (MlinkRegToGameMapModel)DataAccessMgr.getInstance().queryUnique(op);
    }
    
    private static final String sqlnsert = "insert into "+TABLE_NAME+" (ss_id,rt_id,x_id,y_id,game_stat_id,code_id,to_url,remark,insert_time) values (?,?,?,?,?,?,?,?,now()) ";
    public int insert(final MlinkRegToGameMapModel model) throws SQLException{
        OpUpdate op = new OpUpdate(sqlnsert, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1,model.getSs_id());
                ps.setInt(2,model.getRt_id());
                ps.setInt(3,model.getX_id());
                ps.setInt(4,model.getY_id());
                ps.setInt(5,model.getGame_stat_id());
                ps.setString(6,model.getCode_id());
                ps.setString(7,model.getTo_url());
                ps.setString(8,model.getRemark());
            }
        };
        
        return DataAccessMgr.getInstance().update(op);
    }

    private static final String sqUpdate = "update mlink_reg_to_game_map set ss_id=?,rt_id=?,x_id=?,y_id=?,game_stat_id=?,code_id=?,to_url=?,remark=? where id = ?";
    public int update(final MlinkRegToGameMapModel model) throws SQLException{
        OpUpdate op = new OpUpdate(sqUpdate, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1,model.getSs_id());
                ps.setInt(2,model.getRt_id());
                ps.setInt(3,model.getX_id());
                ps.setInt(4,model.getY_id());
                ps.setInt(5,model.getGame_stat_id());
                ps.setString(6,model.getCode_id());
                ps.setString(7,model.getTo_url());
                ps.setString(8,model.getRemark());
                ps.setInt(9,model.getId());
            }
        };
        
        return DataAccessMgr.getInstance().update(op);
    }

    private static final String sqDelete = "delete from mlink_reg_to_game_map where id = ?";
    public int delete(final MlinkRegToGameMapModel model) throws SQLException{
        OpUpdate op = new OpUpdate(sqDelete, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1,model.getId());
            }
        };
        
        return DataAccessMgr.getInstance().update(op);
    }
}
