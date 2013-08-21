/**
 * 
 */
package com.xiaonei.platform.component.gossip.dao;

import java.sql.SQLException;
import java.util.Map;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;
import net.paoding.rose.jade.annotation.ShardParam;

import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.component.gossip.model.GossipExtension;

/**
 *
 * @author 吕恩乐 [enle.lv@opi-corp.com]
 *
 */
@DAO(catalog=Catalogs.GOSSIP_NEW)
//@DAO
public interface GossipExtensionDAO{
	
	static final String save= "insert into gossip_ext (id, head_url,large_url,owner) " +
			"values (:gid, :head,:large,:owner) ";
    /**
     * 保存图片信息 图片地址是不完整的，不能包含域名信息。例如img.renren.com
     * 比如类似于：20060729/1450/head41484.jpg
     * 
     * @param gossipID
     *            留言ID
     * @param headUrl
     *            100定宽的图片地址
     * @param largeUrl
     *            最大尺寸的图片地址
     * @return
     * @throws SQLException
     */
	@SQL(save)
	@ShardParam(":owner")
    public int save(@SQLParam("gid")final long gossipID,@SQLParam("owner")final int owner, @SQLParam("head")final String headUrl, @SQLParam("large")final String largeUrl);
    /**
     * 删除留言的图片信息。 在删除留言的时候需要同时删除留言的图片信息
     * 
     * @param id
     * @param owner
     *            留言所在主页用户id
     * @return
     * @throws SQLException
     */
	final static String remove = "delete from gossip_ext where id = :id";
	@SQL(remove)
	@ShardParam(":owner")
    public int remove(@SQLParam("id")final long id,@SQLParam("owner")int owner);
	
	final static String get = "select id, head_url ,large_url from gossip_ext " +
			"where id>= :minId and id<= :maxId and owner = :owner order by id desc";

	@SQL(get)
	//­@KeyColumnOfMap("id")
	@ShardParam(":owner")
    public Map<String,GossipExtension> getGossipExtension(@SQLParam("owner")final int host,@SQLParam("minId")final long lowGId,@SQLParam("maxId")final long heightGId);
}
