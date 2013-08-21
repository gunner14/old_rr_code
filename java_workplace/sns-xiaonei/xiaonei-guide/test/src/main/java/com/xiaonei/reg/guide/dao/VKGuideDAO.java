package com.xiaonei.reg.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.register.pojo.VKCoralFormInfo;
import com.xiaonei.reg.register.pojo.VKFriendInfo;
import com.xiaonei.xce.XceAdapter;
import com.xiaonei.reg.guide.flows.xfive.formbeans.FormZhongxue;
import com.xiaonei.reg.guide.flows.xfive.formbeans.VKFormDaxue;
import com.xiaonei.reg.guide.flows.xfivecoral.formbeans.CoralFormInfo;

/**
 * @author 董刚锋 gangfeng.dong@renren-inc.com
 * vk流程用的DAO
 *
 */

@DAO(catalog="reg_invite")
public interface VKGuideDAO {
   
	
    
    @SQL("insert into user_basic_info (userid,stage,sex,birthday) values  (:1, :2, :3, :4)")
    public int insertUserBasicInfo(String userid,String stage,int sex,String birthday) throws SQLException; 
    
    //查询好友列表
    @SQL("select friendlist,fromsource from addfriend_info where userid=:1 limit 55")
    public List<VKFriendInfo> selectUseridByUserid(String userid) throws SQLException; 
    
    @SQL("select userid from user_basic_info where real_uid=:1")
    public String selectUseridByRealUserid(int userid) throws SQLException; 
    
    //查询中学
    @SQL("select stage,schooltype,highschoolname,highschoolyear,highschoolcode," +
    		"highschoolclass1,highschoolclass2,highschoolclass3,techschoolname,techschoolyear,techschoolcode," +
    		"juniorhighschoolname,juniorhighschoolyear,juniorhighschoolcode," +
    		"elementaryschoolname,elementaryschoolyear,elementaryschoolcode" +
    		" from formzhongxue_info where userid=:1 limit 1")
    public FormZhongxue selectMid(String userid) throws SQLException; 
    
    //查询上大学
    @SQL("select stage,universityname,universityyear,universitycode," +
    		"universitydepartment,coursetype,highschoolname,highschoolyear,highschoolcode," +
    		"highschoolclass1,highschoolclass2,highschoolclass3 " +
    		"from formdaxue_info where userid=:1 limit 1")
    public VKFormDaxue selectUniv(String userid) throws SQLException; 
    
    //查询工作
    @SQL("select companyname,currentprovince,currentcityname," +
    		"currentcitycode,universityname,universityyear,universitycode," +
    		"universitydepartment,coursetype,highschoolname,highschoolyear,highschoolcode," +
    		"highschoolclass1,highschoolclass2,highschoolclass3," +
    		"juniorhighschoolname,juniorhighschoolyear,juniorhighschoolcode," +
    		"techschoolname,techschoolyear,techschoolcode," +
    		"elementaryschoolname,elementaryschoolyear,elementaryschoolcode from formbailing_info where userid=:1 limit 1")
    public CoralFormInfo selectWork(String userid) throws SQLException; 
    
    //查询其它
    @SQL("select " +
    		"universityname,universityyear,universitycode," +
    		"universitydepartment,coursetype,highschoolname,highschoolyear,highschoolcode," +
    		"highschoolclass1,highschoolclass2,highschoolclass3," +
    		"juniorhighschoolname,juniorhighschoolyear,juniorhighschoolcode," +
    		"techschoolname,techschoolyear,techschoolcode," +
    		"elementaryschoolname,elementaryschoolyear,elementaryschoolcode from formqita_info where userid=:1 limit 1")
    public VKCoralFormInfo selectNone(String userid) throws SQLException; 
    
    @SQL("insert into addfriend_info (userid,friendlist,fromsource) values (:1, :2, :3)")
    public int insertFriendList(String userid,String friend,String fromeSource) throws SQLException;
    
    @SQL("update addfriend_info set friendlist=:1 where userid=:2")
    public int updateFriendList(String friendlist,String userid) throws SQLException;
    
    
    @SQL("update user_basic_info set real_uid=:1 where userid=:2")
    public int updateUserRealId(int real_uid,String userid) throws SQLException;
  
}
