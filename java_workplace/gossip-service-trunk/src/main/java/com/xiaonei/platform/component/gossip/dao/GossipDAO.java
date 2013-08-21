package com.xiaonei.platform.component.gossip.dao;

import java.util.Date;
import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;
import net.paoding.rose.jade.annotation.ShardBy;

import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.component.gossip.view.GossipView;

/**
 * 
 * @author 吕恩乐 [enle.lv@opi-corp.com]
 * 
 */
@DAO(catalog = Catalogs.GOSSIP_NEW)
//@DAO
public interface GossipDAO {

    //static final String table = " gossip_recent ";

    static final String save = "insert into gossip_recent"
            + " (owner,name,sender,tinyurl,univ_name,id,chat_id)"
            + " values (:g.owner,:g.name,:g.sender,:g.tinyurl,:g.univ_name,:g.id,:g.chatId)";

    @SQL(save)
    public int save(@SQLParam("g") final Gossip gossip, @ShardBy @SQLParam("owner") int owner);

    static final String saveBody = "insert into gossip_body_recent " + " (id, body, time,owner) "
            + " values (:id,:body,:time,:owner)";

    @SQL(saveBody)
    public int saveBody(@ShardBy @SQLParam("owner") int owner, @SQLParam("id") long id,
            @SQLParam("body") String body, @SQLParam("time") Date time);

    public static final String delete = "delete from gossip_recent"
            + " where id = :id and owner= :owner";

    @SQL(delete)
    int delete(@SQLParam("id") long id, @ShardBy @SQLParam("owner") int owner);

    public static final String deleteBody = "delete from gossip_body_recent  where id = :id ";

    @SQL(deleteBody)
    public int deleteBody(@SQLParam("id") long id, @ShardBy @SQLParam("owner") int owner);

    static final String getAllList = "select gr.id,gr.owner,gr.chat_id chatId,gr.sender guestId,gr.name guestName,gr.univ_name guestUniv,gr.tinyurl,gbr.body,gbr.time "
            + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr " +
            //测试代码,上面是线上代码
            //FIXME
            //"from gossip_body_recent gbr , gossip_recent gr " +
            "where gr.owner=:owner  #if(:host!=:owner){ and gr.chat_id=0 } and gr.id=gbr.id order by gr.id desc limit :offset,:limit";

    @SQL(getAllList)
    public List<GossipView> getAllList(@ShardBy @SQLParam("owner") long owner,//NL
            @SQLParam("offset") int offset//NL
            , @SQLParam("limit") int limit, @SQLParam("host") int host);//NL

    static final String getPublicList = "select gr.id,gr.owner,gr.chat_id chatId,gr.sender guestId,gr.name guestName,gr.univ_name guestUniv,gr.tinyurl,gbr.body,gbr.time "
            + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr " +
            //测试代码,上面是线上代码
            //FIXME
            //"from gossip_body_recent gbr , gossip_recent gr " +
            "where gr.owner=:owner and gr.chat_id=0 and gr.id=gbr.id order by gr.id desc limit :offset,:limit";

    @SQL(getPublicList)
    public List<GossipView> getPublicList(@ShardBy @SQLParam("owner") long owner,//NL
            @SQLParam("offset") int offset//NL
            , @SQLParam("limit") int limit, @SQLParam("host") int host);//NL
    
    /**
     * 获取非悄悄话留言数量
     * BY wenzhiwei
     */
    static final String getPubliccount = "select count(1) "
            + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr " +
            //测试代码,上面是线上代码
            //FIXME
            //"from gossip_body_recent gbr , gossip_recent gr " +
            "where gr.owner=:owner and gr.chat_id=0 and gr.id=gbr.id ";
    @SQL(getPubliccount)
    public int getPublicCount (@ShardBy @SQLParam("owner") long owner);


    static final String getGossipCount = "select count(1) from gossip_recent  where owner=:owner";

    /**
     * 获取非聊天留言的个数
     * 
     * @param owner
     * @return
     */
    @SQL(getGossipCount)
    public int getCount(@ShardBy @SQLParam("owner") long owner);

    static final String getConversationCount = "select count(1) from gossip_recent where owner=:owner and sender=:sender";

    @SQL(getConversationCount)
    public int getConversationCount(@ShardBy @SQLParam("owner") long owner,
            @SQLParam("sender") int sender);

    static final String getConversation = "select gr.id,gr.owner,gr.chat_id chatId, gr.sender guestId, gr.name guestName,gr.tinyurl,gr.univ_name guestUniv, gbr.body,gbr.time "
            + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr "
            +
            //FIXME
            //"from gossip_body_recent gbr , gossip_recent gr " +
            "where gr.owner=:owner and gr.sender=:sender and gr.id=gbr.id "
            + "#if(:gid>0){ #if(:limit>0){ and gr.id < :gid } #else { and gr.id > :gid } }  "
            + "#if(!:boundary && :limit<0 || :boundary && :limit>0 ){} #else {  order by gr.id desc  }  "
            + " limit :limit";

    @SQL(getConversation)
    public List<GossipView> getConversation(@ShardBy @SQLParam("owner") long owner,
            @SQLParam("sender") int sender, @SQLParam("gid") long gossipId,
            @SQLParam("limit") int limit, @SQLParam("boundary") boolean boundary);

    static final String getGossipByOwner = "select gr.id,gr.owner,gr.chat_id chatId, gr.sender guestId, gr.name guestName,gr.tinyurl,gr.univ_name guestUniv, gbr.body,gbr.time "
            + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr " +
            //FIXME
            //"from gossip_body_recent gbr , gossip_recent gr " +
            "where gr.owner=:owner and gr.sender=:owner and gr.id=gbr.id  order by gr.id desc limit :limit, :offset";

    @SQL(getGossipByOwner)
    public List<GossipView> getGossipByOwner(@ShardBy @SQLParam("owner") long owner,
            @SQLParam("limit") int limit, @SQLParam("offset") int offset);

    static final String getGossipById = "select gr.id,gr.owner,gr.chat_id chatId, gr.sender guestId, gr.name guestName,gr.tinyurl,gr.univ_name guestUniv, gbr.body,gbr.time "
            + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr " +
            //FIXME
            //"from gossip_body_recent gbr , gossip_recent gr " +
            "where gr.owner=:owner and gr.id=:id and gr.id=gbr.id ";

    @SQL(getGossipById)
    public GossipView getGossipById(@ShardBy @SQLParam("owner") int owner, @SQLParam("id") long id);

    static final String getGossipByTime = "select gr.id,gr.owner,gr.chat_id chatId, gr.sender guestId, gr.name guestName,gr.tinyurl,gr.univ_name guestUniv, gbr.body,gbr.time "
            + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr " +
            //FIXME
            //"from gossip_body_recent gbr , gossip_recent gr " +
            "where gr.owner=:owner and gr.sender=:sender and gbr.time=:time and gr.id=gbr.id order by id desc limit 1";

    @SQL(getGossipByTime)
    public GossipView getGossipByTime(@ShardBy @SQLParam("owner") int owner,
            @SQLParam("sender") int sender, @SQLParam("time") Date time);

    static final String getGossipLevelCount = "select count(1) from gossip_recent where owner=:owner and chat_id=0";

    @SQL(getGossipLevelCount)
    public int getGossipLevelCountByUser(@ShardBy @SQLParam("owner") int owner);
    
    /**
     * 按时间区间获取给别人留言的留言列表
     * BY wenzhiwei Email:zhiwei.wen@renren-inc.com
     * 2012-10-23
     */
    static final String getOwnerGossipByTimeInterval = "select gr.id,gr.owner,gr.chat_id chatId, gr.sender guestId, gr.name guestName,gr.tinyurl,gr.univ_name guestUniv, gbr.body,gbr.time "
           + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr " + 
    		 "where gr.owner=:owner and gr.sender=:owner and gbr.time between :startime and :endtime and gr.id=gbr.id order by gr.id desc limit :limit,:offset";
    
    @SQL(getOwnerGossipByTimeInterval)
    public List<GossipView> getOwnerGossipByTimeInterval(@ShardBy @SQLParam("owner") int owner,
    		@SQLParam("startime") Date startime, @SQLParam("endtime") Date endtime,@SQLParam("limit") int limit,@SQLParam("offset") int offset);
    
    /**
     * 按时间区间获取别人给我留言的留言列表
     * BY wenzhiwei Email:zhiwei.wen@renren-inc.com
     * 2012-10-23
     */
    static final String getOtherGossipByTimeInterval = "select gr.id,gr.owner,gr.chat_id chatId, gr.sender guestId, gr.name guestName,gr.tinyurl,gr.univ_name guestUniv, gbr.body,gbr.time "
           + "from gossip_body_recent_##(:owner % 100) gbr , gossip_recent gr " + 
    		 "where gr.owner=:owner and gr.sender!=:owner and gbr.time between :startime and :endtime and gr.id=gbr.id order by gr.id desc limit :limit,:offset";
    
    @SQL(getOtherGossipByTimeInterval)
    public List<GossipView> getOtherGossipByTimeInterval(@ShardBy @SQLParam("owner") int owner,
    		@SQLParam("startime") Date startime, @SQLParam("endtime") Date endtime,@SQLParam("limit") int limit,@SQLParam("offset") int offset);
    
    
}
