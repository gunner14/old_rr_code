package com.renren.sns.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.renren.sns.guide.model.GuideMiddleSchool;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.xce.XceAdapter;

public class PopUserMiddleSchoolDAO {

	private PopUserMiddleSchoolDAO() {
	}

	private static PopUserMiddleSchoolDAO instance = null;

	public static synchronized PopUserMiddleSchoolDAO getInstance() {
		if (instance == null) {
			instance = new PopUserMiddleSchoolDAO();
		}
		return instance;
	}
	
	@SuppressWarnings("unchecked")
    public List<Integer> getPopUserMiddleSchoolList(final int highSchoolId,final int enrollYear,final int limit) throws SQLException {

        StringBuffer sb = new StringBuffer();
        sb.append("select id from pop_user_middle_school where middle_school_id = ? order by abs( ? - enroll_year) , view_count desc limit ?");        
        OpList op = new OpList(sb.toString(), XceAdapter.DB_POP_USER) {        	

        	@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, highSchoolId);
				ps.setInt(2, enrollYear);
				ps.setInt(3, limit);
			}        	
        	
            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return rs.getInt("id");
            }

        };

        return DataAccessMgr.getInstance().queryList(op);
    }
    
    @SuppressWarnings("unchecked")
    public List<GuideMiddleSchool> getMiddleSchoolIdsByCityCode(final String cityCode,final int popUserCount) throws SQLException {

        StringBuffer sb = new StringBuffer();
        sb.append("select middle_school_id,middle_school_name from pop_user_middle_school_map where city_code = ? and pop_user_count > ?");        
        OpList op = new OpList(sb.toString(), XceAdapter.DB_POP_USER) {        	

        	@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, cityCode);
				ps.setInt(2, popUserCount);
			}        	
        	
            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseGuideMiddleSchool(rs);
            }

        };

        return (List<GuideMiddleSchool>)DataAccessMgr.getInstance().queryList(op);
    }
    
    public static GuideMiddleSchool parseGuideMiddleSchool(ResultSet rs) throws SQLException {
    	GuideMiddleSchool gms = new GuideMiddleSchool();
    	gms.setMiddleSchoolId(rs.getInt("middle_school_id"));
    	gms.setMiddleSchoolName(rs.getString("middle_school_name"));
    	return gms;
    }
}
