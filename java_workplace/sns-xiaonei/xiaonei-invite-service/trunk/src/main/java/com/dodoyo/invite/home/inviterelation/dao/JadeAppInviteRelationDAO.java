package com.dodoyo.invite.home.inviterelation.dao;

import java.util.Date;
import java.util.List;

import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.dodoyo.invite.home.inviterelation.model.AppInviteRelation;

public interface JadeAppInviteRelationDAO {

    /**
     * 添加邀请关系 appId,inviterId,inviteeId
     * 
     * @param appId 应用id
     * @param inviterId 邀请者id
     * @param inviteeId 被邀请者id
     * @return
     */
    @SQL("INSERT INTO app_invite_relation (inviter_id, invitee_id, app_id) VALUES (:inviter_id, :invitee_id, :app_id)")
    public int addAppInviteRelation(@SQLParam("app_id") int appId,
            @SQLParam("inviter_id") int inviterId, @SQLParam("invitee_id") int inviteeId);

    /**
     * 获得被邀请者列表
     * 
     * @param appId 应用id
     * @param lastId 最后的id
     * @param offset
     * @param limit
     * @return List<AppInviteRelation>
     */
    @SQL("SELECT * FROM app_invite_relation WHERE `inviter_id`=:inviter_id AND `app_id`=:app_id LIMIT :offset,:limit")
    public List<AppInviteRelation> getInviteeList(@SQLParam("app_id") int appId,
            @SQLParam("inviter_id") int inviterId, @SQLParam("offset") int offset,
            @SQLParam("limit") int limit);

    /**
     * 根据appId和时间段获取邀请关系 <br>
     * 请调用者限制maxCount
     * 
     * @param appId 应用id
     * @param beginTime 开始时间
     * @param endTime 结束时间
     * @param offset
     * @param maxCount 最大限制
     * @return List<AppInviteRelation>
     */
    @SQL("SELECT * FROM app_invite_relation WHERE `create_time` BETWEEN :beginTime AND :endTime AND `app_id`=:app_id LIMIT :offset,:maxCount")
    public List<AppInviteRelation> getInviterList(@SQLParam("app_id") int appId,
            @SQLParam("beginTime") Date beginTime, @SQLParam("endTime") Date endTime,
            @SQLParam("offset") int offset, @SQLParam("maxCount") int maxCount);

    /**
     * 根据被邀请者id获取邀请关系
     * 
     * @param inviteeId 被邀请者id
     * @param appId 应用的id
     * @return List<AppInviteRelation>
     */
    @SQL("SELECT * FROM app_invite_relation WHERE `invitee_id`=:invitee_id and `app_id`=:app_id")
    public List<AppInviteRelation> getInviterList(@SQLParam("invitee_id") int inviteeId,
            @SQLParam("app_id") int appId);

    /**
     * 根据appId和最后的id获取一定数量的邀请关系 <br>
     * 请调用者限制count的数量
     * 
     * @param appId 应用id
     * @param lastId 最后的id
     * @param count 条数
     * @return List<AppInviteRelation>
     */
    @SQL("SELECT * FROM app_invite_relation WHERE `id`>:lastId AND `app_id`=:app_id LIMIT 0,:count")
    public List<AppInviteRelation> getInviterList(@SQLParam("app_id") int appId,
            @SQLParam("lastId") int lastId, @SQLParam("count") int count);
}
