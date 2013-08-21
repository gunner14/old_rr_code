package com.dodoyo.invite.dao;

import java.util.Set;

import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.dodoyo.invite.core.entity.InviteUrlParas;
import com.xiaonei.platform.framework.exception.DataAccessException;

/**
 * 邀请连接参数dao
 *
 * @author dream
 */
public interface JadeInviteUrlParasDAO 
{    
    /*@SQL("select * from invite_url_paras where md5 = :md5")
    public 
    List<InviteUrlParas> getInviteUrlParasByMd5(@SQLParam("md5") String md5) 
    throws DataAccessException;
    
    @SQL("selelct * from invite_url_paras where user_id = :userId")
    public 
    List<InviteUrlParas> selectUserByUserId(@SQLParam("userId") int userId) 
    throws DataAccessException;*/
    
    @SQL("insert ignore into invite_url_paras (id, uuid, user_id, email_from, " +
    		"email_to, ss, app_id, app_token, md5, extar_paras, buddy_group, " +
    		"code, rt, time) values(:data.id,:data.uuid,:data.user_id,:data.email_from," +
    		":data.email_to,:data.ss,:data.app_id,:data.app_token,:data.md5,:data.extar_paras," +
    		":data.buddy_group,:data.code,:data.rt,now())")
    public
    void insertIups(@SQLParam("data") Set<InviteUrlParas> midDatas) throws DataAccessException;
    
    @SQL("insert ignore into invite_url_paras (uuid, user_id, email_from, " +
            "email_to, ss, app_id, app_token, md5, extar_paras, buddy_group, " +
            "code, rt, time) values(:data.uuid,:data.user_id,:data.email_from," +
            ":data.email_to,:data.ss,:data.app_id,:data.app_token,:data.md5,:data.extar_paras," +
            ":data.buddy_group,:data.code,:data.rt,now())")
    public
    void insertIup(@SQLParam("data") InviteUrlParas midDatas) throws DataAccessException;
}
