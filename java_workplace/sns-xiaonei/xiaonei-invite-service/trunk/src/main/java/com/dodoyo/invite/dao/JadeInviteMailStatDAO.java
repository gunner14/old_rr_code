package com.dodoyo.invite.dao;

import java.util.Set;

import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.dodoyo.invite.core.entity.InviteMailStat;
import com.xiaonei.platform.framework.exception.DataAccessException;

/**
 * @author dream
 *         邀请邮件统计dao,产品张魏
 */
public interface JadeInviteMailStatDAO
{
    @SQL("insert ignore into invite_detail_stat (userid, email_from, email_to, app_id, " +
    		"source_id, action_type, mail_suffix, mail_suffix_from, mail_suffix_to, enter_type," +
    		"invite_type,s1,s2,s3,s4,time) " +
    		"values(:stat.userid,:stat.email_from,:stat.email_to,:stat.app_id,:stat.source_id," +
    		":stat.action_type,:stat.mail_suffix,:stat.main_suffix_from,:stat.mail_suffix_to,:stat.enter_type," +
    		":stat.invite_type,:stat.s1,:stat.s2,:stat.s3,:stat.s4,now())")
    public int insertRecorders(@SQLParam("stat") Set<InviteMailStat> stats) throws DataAccessException;
}
