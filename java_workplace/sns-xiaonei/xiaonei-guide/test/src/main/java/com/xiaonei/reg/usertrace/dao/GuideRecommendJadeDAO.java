package com.xiaonei.reg.usertrace.dao;

import java.sql.SQLException;
import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.reg.guide.pojo.GuideZ3Pojo;
import com.xiaonei.reg.guide.pojo.PagePojo;
import com.xiaonei.reg.guide.pojo.RecommendContent;

@DAO(catalog = "reg_invite")
public interface GuideRecommendJadeDAO {

	@SQL("insert into guide_recommend_content_view_log (user_id,type,recommend_content_id,frompage,subtype) values  (:1, :2, :3, :4, :5)")
	public int insertGuideRecommendCountViewLog(int user_id, int type, int recommend_content_id, String frompage, String subtype)throws SQLException;

	@SQL("update guide_recommend_content set hits=hits+1 where id=:1")
	public int updateGuideRecommendhits(int id) throws SQLException;

	@SQL("update guide_recommend_content set show_count=show_count+1 where id=:1")
	public int updateGuideRecommendView(int id) throws SQLException;

	@SQL("select id,content_business_id,content_type, hits/show_count as ep,nickname,interest from guide_recommend_content where content_type=:1 order by ep")
	public List<RecommendContent> selectGuideRecommendContent(int contentType) throws SQLException;

	@SQL("select distinct recommend_content_id from guide_recommend_content_view_log where user_id=:1")
	public List<Integer> selectRecommendContentViewLog(int userid) throws SQLException;

	@SQL("select distinct recommend_content_id from guide_recommend_content_view_log where user_id=:1 and type=3")
	public List<Integer> selectRecommendContentViewLogCon(int userid) throws SQLException;

	@SQL("delete from  guide_recommend_content")
	public int deleteGuideContentdContent() throws SQLException;

	/*
	 * ----------------------------------------------guide Z3 DAO----------------------------------------------------------
	 */
	@SQL("select tag_value,tag_order,sex_flag,tag_img,foucs_count from guide_flow_z3 where sex_flag=:1 order by tag_order")
	public List<GuideZ3Pojo> selectGuideZ3List(int sexFlag) throws SQLException;

	@SQL("update guide_flow_z3 set foucs_count=foucs_count+1 where tag_value=:1 and sex_flag=:2")
	public int updateGuideZ3AddFoucsCount(String tagValue, int sexFlag) throws SQLException;

	@SQL("insert into guide_flow_z3 (tag_value,tag_order,sex_flag,tag_img,foucs_count) values (:1, :2, :3, :4, :5)")
	public int insertGuideZ3Content(String tag_value, int tag_order, int sex_flag, String tag_img, int foucs_count) throws SQLException;

	@SQL("delete from guide_flow_z3")
	public int deleteGuideZ3Content() throws SQLException;
	
	/*
	 * ----------------------------------------------guide usersort DAO----------------------------------------------------------
	 */
	@SQL("SELECT login_record FROM login_record_count WHERE user_id=:1")
	public String getUserLoginRecord(int userId) throws SQLException;
	
	
	
	//查找page 数据
	@SQL("select page_id,page_name,university_id, university_name,recommend_info,page_type,remarks from guide_recommend_page where university_id=:1 and page_type=0")
	public PagePojo selectGuideRecommendPage(int universityId) throws SQLException;

	//插入 page数据
	@SQL("insert into guide_recommend_page (page_id,page_name,university_id, university_name,recommend_info,page_type,remarks) values (:1, :2, :3, :4, :5, :6, :7)")
	public int insertPageContent(int pageId,String pageName,int universityId,String universityName,String recommendInfo,int pageType,String remarks) throws SQLException;
    //删除page数据
	@SQL("delete from  guide_recommend_page")
	public int deletePageContent() throws SQLException;
	
	//插入用户新鲜事条数
	@SQL("insert into guide_user_feedcount (user_id,feedcount,tags) values (:1, :2, :3)")
	public int insertGuideUserFeedCount(int user_id, int feedcount, String tags) throws SQLException;
	//查询用户新鲜事条数
	@SQL("select feedcount from guide_user_feedcount where user_id=:1")
	public List<Integer> selectGuideUserFeedCount(int user_id) throws SQLException;
	//更新新鲜事条数
	@SQL("update guide_user_feedcount set feedcount=:1 where user_id=:2")
	public int updateGuideUserFeedCount(int feedcount,int user_id) throws SQLException;
	
}
