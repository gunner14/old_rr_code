package com.renren.sns.wiki.dao.adminwiki;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.admin.SuperAdmin;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-4
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface SuperAdminDAO {

    public static final String TABLE_NAME = "wiki_super_admin";

    public static final String DB_FIELDS = "user_id, name, remark, type";

    public static final String BEAN_FIELDS = ":superAdmin.userId, :superAdmin.name, :superAdmin.remark, :superAdmin.type";

    public static final String INSERT_DB_FIELDS = DB_FIELDS;

    public static final String INSERT_BEAN_FIELDS = BEAN_FIELDS;

    public static final String SELECT_DB_FIELDS = "id, insert_time, " + DB_FIELDS;

    /**
     * 添加管理员
     */
    @SQL("insert into " + TABLE_NAME + "(" + INSERT_DB_FIELDS + ") values (" + INSERT_BEAN_FIELDS
            + ")")
    public int addSuperAdmin(@SQLParam("superAdmin") SuperAdmin superAdmin);

    /**
     * 根据userId获取管理员
     * 
     * @param userId
     * @return
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME + " where user_id=:userId")
    public SuperAdmin getSuperAdminByUserId(@SQLParam("userId") int userId);

    /**
     * 根据userId获取管理员
     * 
     * @param userId
     * @return
     */
    @SQL("select user_id from " + TABLE_NAME + " where user_id=:userId limit 1")
    public Integer getSuperAdminUserIdByUserId(@SQLParam("userId") int userId);

    /**
     * 获取超管列表
     * 
     * @return
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME)
    public List<SuperAdmin> getSuperAdminList();

    /**
     * 获取超管人人ID列表
     * 
     * @return
     */
    @SQL("select user_id from " + TABLE_NAME)
    public List<Integer> getSuperAdminIdList();

    /**
     * 移除超管
     * 
     * @param userId
     * @return
     */
    @SQL("delete from " + TABLE_NAME + " where user_id=:userId")
    public int removeSuperAdmin(@SQLParam("userId") int userId);

}
