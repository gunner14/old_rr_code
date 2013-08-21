package com.renren.sns.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.xce.XceAdapter;

public class PopUserCityDAO {
	
	private PopUserCityDAO() {
	}

	private static PopUserCityDAO instance = null;

	public static synchronized PopUserCityDAO getInstance() {
		if (instance == null) {
			instance = new PopUserCityDAO();
		}
		return instance;
	}
	
	@SuppressWarnings("unchecked")
	public List<Integer> getUserStarIdsByCityCode(final String cidyCode,final int limit) throws SQLException{
		StringBuffer sb = new StringBuffer();
        sb.append("select id from pop_user_city where city_code = ? order by view_count desc limit ?");        
        OpList op = new OpList(sb.toString(), XceAdapter.DB_POP_USER) {        	

        	@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, cidyCode);
				ps.setInt(2, limit);
			}        	
        	
            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return rs.getInt("id");
            }

        };

        return (List<Integer>)DataAccessMgr.getInstance().queryList(op);
	}
	
}
