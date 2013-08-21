package com.xiaonei.reg.usertrace.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.guide.pojo.RecommendContent;


public class GuideRecommendDAO {
    private static GuideRecommendDAO _instance = new GuideRecommendDAO();

    public static GuideRecommendDAO getInstance() {
        return _instance;
    }

    private GuideRecommendDAO() {
    }

    public int insert_guide_action_log(final int user_id,final int page_id,final String action,final String oper) throws SQLException {
        final String sqlInsert = "insert into guide_action_log (user_id,page_id,action,oper) values (?,?,?,?)";
        final OpUpdate op = new OpUpdate(sqlInsert, "reg_invite") {
            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, user_id);
                ps.setInt(2, page_id);
                ps.setString(3, action);
                ps.setString(4, oper);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    public int insert_guide_log_record(final int user_id,final int sub_step,final String source,final int step) throws SQLException {
        final String sqlInsert = "insert into guide_log_record (user_id,sub_step,source,step) values (?,?,?,?)";
        final OpUpdate op = new OpUpdate(sqlInsert, "reg_invite") {
            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, user_id);
                ps.setInt(2, sub_step);
                ps.setString(3, source);
                ps.setInt(4, step);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }


    public int insert_newflow_log_record(final int user_id,final int sub_step,final String source,final int step,final String pageid) throws SQLException {
        final String sqlInsert = "insert into newflow_log_record (user_id,sub_step,source,step,pageid) values (?,?,?,?,?)";
        final OpUpdate op = new OpUpdate(sqlInsert, "reg_invite") {
            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, user_id);
                ps.setInt(2, sub_step);
                ps.setString(3, source);
                ps.setInt(4, step);
                ps.setString(5, pageid);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
    
    
    public int intiRecommendContent(final Long id,final String nickname,final String interest) throws SQLException {
        final String sqlInsert = "insert into guide_recommend_content (content_business_id,content_type,hits" +
                ",show_count,user_keep_rate,nickname,interest) values (?,?,?,?,?,?,?)";
        final OpUpdate op = new OpUpdate(sqlInsert, "reg_invite") {
            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setLong(1, id);
                ps.setInt(2, 1);
                ps.setInt(3, 1);
                ps.setInt(4, 1);
                ps.setInt(5, 100);
                ps.setString(6, nickname);
                ps.setString(7, interest);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }


   /* //以下是推荐用的方法
    public int insertGuideRecommendCountViewLog(final int user_id,final int type,final int recommend_content_id) throws SQLException {
        final String sqlInsert = "insert into guide_recommend_content_view_log (user_id,type,recommend_content_id) values (?,?,?)";
        final OpUpdate op = new OpUpdate(sqlInsert, "count") {
            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, user_id);
                ps.setInt(2, type);
                ps.setInt(3, recommend_content_id);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

   


    @SuppressWarnings("unchecked")
    public List<RecommendContent> selectRecommendContent(final int userid,final int limit) throws SQLException {
        final String sqlGetFrom = "select id,content_business_id,content_type," +
                "hits/show_count as ep from guide_recommend_content" +
                " where id not in (select distinct recommend_content_id from " +
                "guide_recommend_content_view_log where user_id=?) order by ep limit "+limit;
        final OpList op = new OpList(sqlGetFrom, "count") {
            @Override
            public RecommendContent parse(final ResultSet rs) throws SQLException {
                final RecommendContent recommendContent=new RecommendContent();
                recommendContent.setId(rs.getInt("id"));
                recommendContent.setContent_business_id(rs.getInt("content_business_id"));
                recommendContent.setContent_type(rs.getInt("content_type"));
                return recommendContent;
            }


            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, userid);
            }

        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    public int delete_guide_recommend_count_view_log(final int type) throws SQLException {
        final String sqlInsert = "delete from guide_recommend_count_view_log where type=?";
        OpUpdate op = new OpUpdate(sqlInsert, "count") {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, type);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    @SuppressWarnings("unchecked")
    public List<RecommendContent> selectRecommendContentByEp(final int userid) throws SQLException {
        final String sqlGetFrom = "select id,content_business_id,content_type," +
                "hits/show_count as ep from guide_recommend_content" +
                " where id not in (select distinct recommend_content_id from " +
                "guide_recommend_content_view_log where user_id=? and type=3) order by ep";
        final OpList op = new OpList(sqlGetFrom, "count") {
            @Override
            public RecommendContent parse(final ResultSet rs) throws SQLException {
                final RecommendContent recommendContent=new RecommendContent();
                recommendContent.setId(rs.getInt("id"));
                recommendContent.setContent_business_id(rs.getInt("content_business_id"));
                recommendContent.setContent_type(rs.getInt("content_type"));
                return recommendContent;
            }


            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, userid);
            }

        };
        return DataAccessMgr.getInstance().queryList(op);
    }



    public int updateGuideRecommendhits(final int id) throws SQLException {
        final String sqlInsert = "update guide_recommend_content set hits=hits+1 where id=?";
        final OpUpdate op = new OpUpdate(sqlInsert, "count") {
            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    public int updateGuideRecommendView(final int id) throws SQLException {
        final String sqlInsert = "update guide_recommend_content set show_count=show_count+1 where id=?";
        final OpUpdate op = new OpUpdate(sqlInsert, "count") {
            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }*/

}
