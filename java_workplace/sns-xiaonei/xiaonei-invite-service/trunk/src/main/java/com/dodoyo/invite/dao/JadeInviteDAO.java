package com.dodoyo.invite.dao;

import java.sql.SQLException;
import java.util.Date;
import java.util.List;

import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import org.springframework.dao.DataAccessException;

import com.dodoyo.invite.model.InviteInfo;

public interface JadeInviteDAO {

    //保存邀请
    @SQL("insert ignore into invite_list (owner_id, owner_name, account, flag, tinyurl, from_account, headurl)"
            + " values (:owner_id, :owner_name, :account, :flag, :tinyurl, :from_account, :headurl)")
    public int save(@SQLParam("owner_id") int owner_id, // NL
            @SQLParam("owner_name") String owner_name, // NL
            @SQLParam("account") String account, // NL
            @SQLParam("flag") int flag, // NL
            @SQLParam("tinyurl") String tinyurl, // NL
            @SQLParam("from_account") String from_account, // NL
            @SQLParam("headurl") String headurl) throws DataAccessException;

    //保存邀请 add by 康伟 适应新的需求：记录是邀请或者查找 是MSN或者WEBMAIL或者其他方式，具体情况参见类注释
    @SQL("insert ignore into invite_list (owner_id, owner_name, account, flag, tinyurl, from_account, headurl,action_type,app_id, nickname)"
            + " values (:owner_id,:owner_name,:account,:flag,:tinyurl,:from_account,:headurl,:action_type,:appId,:nickname)")
    public int saveWithActionType(@SQLParam("owner_id") int owner_id, // NL
            @SQLParam("owner_name") String owner_name, // NL
            @SQLParam("account") String account, // NL
            @SQLParam("flag") int flag, // NL
            @SQLParam("tinyurl") String tinyurl, // NL
            @SQLParam("from_account") String from_account, // NL
            @SQLParam("headurl") String headurl, // NL
            @SQLParam("action_type") int action_type, // NL
            @SQLParam("appId") int appId, // NL
            @SQLParam("nickname") String nickname) throws DataAccessException;

    @SQL("UPDATE invite_list SET flag=:status ,nickname=:nickname WHERE id=:id")
    public int UpdateInviteStatusWithNickname(@SQLParam("id") int id, // NL
            @SQLParam("status") int status, // NL
            @SQLParam("nickname") String nickname) throws DataAccessException;

    @SQL("UPDATE invite_list SET `flag`=:status WHERE `id`=:id")
    public int UpdateInviteStatus(@SQLParam("id") int id, // NL
            @SQLParam("status") int status) throws DataAccessException;

    @SQL("update invite_list set flag = :status, invitee_user_id = :invitee_user_id where id = :id")
    public int updateInviteStatusForUserId(@SQLParam("id") int id, // NL
            @SQLParam("invitee_user_id") int invitee_user_id, // NL
            @SQLParam("status") int status) throws DataAccessException;

    /**
     * 查看是否有重复数据 康伟添加
     */
    @SQL("SELECT * FROM `invite_list` WHERE `owner_id`=:owner_id AND `account`=:account LIMIT 0, 1")
    public InviteInfo checkSelect(@SQLParam("owner_id") int owner_id, // NL
            @SQLParam("account") String account) throws DataAccessException;

    @SQL("SELECT * FROM `invite_list` WHERE `owner_id`=:inviteId AND `account`=:invitedEmail AND `flag`=1 ORDER BY id DESC LIMIT 0, 1")
    public InviteInfo getInviteInfo(@SQLParam("inviteId") int inviteId, // NL
            @SQLParam("invitedEmail") String invitedEmail) throws DataAccessException;

    /**
     * 根据发送邀请者的ID读取flag=1的被邀请的用户
     * 
     * @owner_Id 邀请者的ID
     * @offset 查询数据库记录的起始未知
     * @limit 查询数据库的结束
     * @author seraph
     */
    @SQL("SELECT * FROM `invite_list` WHERE `owner_id`=:owner_id AND `flag`=:flag LIMIT ##(:offset),##(:limit)")
    public List<InviteInfo> getInviteInfoByOwnerId(@SQLParam("owner_id") int owner_id, // NL
            @SQLParam("flag") int flag, // NL
            @SQLParam("offset") int offset, // NL
            @SQLParam("limit") int limit) throws DataAccessException;

    @SQL("SELECT * FROM `invite_list` WHERE `id`=:id AND `flag`=1 ORDER BY id DESC LIMIT 0, 1")
    public InviteInfo getInviteInfoById(@SQLParam("id") int id) throws DataAccessException;

    @SQL("SELECT COUNT(id) FROM `invite_list` WHERE `account`=:email AND `flag`=1")
    public int getInviteCount(@SQLParam("email") String email) throws DataAccessException;

    /**
     * 根据被邀请的Email读取数据库表的记录
     * 
     * @email 被邀请的Email
     */
    @SQL("SELECT * FROM `invite_list` WHERE `account`=:email AND `flag`=1")
    public List<InviteInfo> getInviteUsers(@SQLParam("email") String email)
            throws DataAccessException;

    @SQL("SELECT * FROM `invite_list` WHERE `account`=:email AND `flag`=0")
    public List<InviteInfo> getNotInviteUsers(@SQLParam("email") String email)
            throws DataAccessException;

    //	private final static String sqlGetMSNFriendsByEmail = "SELECT * FROM `invite_list` WHERE `owner_id`=? AND `flag`=0 LIMIT 0, 10"; //康伟注释
    @SQL("SELECT * FROM `invite_list` WHERE `owner_id`=:userId LIMIT 0, 10")
    public List<InviteInfo> getMSNFriends(@SQLParam("userId") int userId)
            throws DataAccessException;

    /**
     * 下面的描述将以"寻找好友"这个名称统称"寻找好友"以及"邀请好友"两个功能，就是说，当提及"寻找好友"的时候，其实是在说这两个功能.
     * 因为它们只是在概念上有区别
     * <p/>
     * <b>本接口实现这样一个功能：查询谁通过寻找好友寻找过我</b>
     * <p/>
     * 假设有这样一个好友关系：A<b>用户</b>的MSN里有一个<b>好友</b>B,B并没有注册在系统中
     * 当A用户使用了寻找好友功能，并且允许系统记录了他的好友关系，系统就会知道B的EMAIL地址。当B以后注册到系统的时候，通过本接口可以将
     * <b>用户</b>A推荐给<b>用户</b>B
     * 
     * @param email B用户的电子邮件地址
     * @return List<InviteInfo> 曾经使用寻找好友功能寻找过B的用户信息集合
     * @throws SQLException
     */
    @SQL("SELECT * FROM `invite_list` WHERE `account`=:email")
    public List<InviteInfo> invitedGetInvitoer(@SQLParam("email") String email)
            throws DataAccessException;

    /**
     * 为好友分组添加的保存信息
     * 
     * @param hostId 用户的id
     * @param groupId 分组的id
     * @param friendMail 被邀请者的email
     * @return 数据库插入结果
     * @throws SQLException
     */
    @SQL("insert into invite_list_group (hostid, groupid, guestemail, groupname) values (:hostId,:groupid,:friendMail,:groupname)")
    public int saveFriendGroup(@SQLParam("hostId") int hostId, // NL
            @SQLParam("groupid") int groupid, // NL
            @SQLParam("friendMail") String friendMail, // NL
            @SQLParam("groupname") String groupname) throws DataAccessException;

    /**
     * 查询invite_time
     * 
     * @param rs
     * @return
     * @throws SQLException
     */
    @SQL("select invite_time from invite_list_2 where owner_id = :id")
    public Date getInvite_time(@SQLParam("id") int id) throws DataAccessException;
}
