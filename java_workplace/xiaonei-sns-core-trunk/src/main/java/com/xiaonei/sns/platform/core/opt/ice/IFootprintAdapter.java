/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;

import mop.hi.svc.fpt.FootprintException;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.xce.footprintunite.FootprintUniteAdapter;
import com.xiaonei.xce.footprintunite.FootprintView;
import com.xiaonei.xce.footprintunite.HomeFootprintResult;

/**
 * 脚印服务
 * 
 * @author lookis (comic.liu@gmail.com)
 * @author xylz (xylz@live.cn)
 * @author tai.wang@opi-corp.com May 31, 2010 - 6:49:17 PM
 */
public interface IFootprintAdapter extends IRegisterable {

    /**
     * 获取指定数量的脚印
     * 
     * @param hostId 用户id
     * @param begin 偏移值
     * @param limit 大小,-1为全部记录
     * @return 带脚印数量的脚印服务
     */
	HomeFootprintResult getAllFootprints(int hostId, int begin, int limit);

    /**
     *@deprecated 2010-01-26
     *@see #getAllFootprints(int, int, int)
     */
    List<FootprintView> getAll(int hostId) throws FootprintException;

    /**
     * @deprecated 2010-01-26
     * @see #getAllFootprints(int, int, int)
     */
    List<FootprintView> getAll(int hostId, int begin, int limit) throws FootprintException;

    /**
     * 获取脚印数量<br />
     * 如果你需要脚印的列表，请使用{@link #getAllFootprints(int, int, int)}
     */
    int getSize(int hostId) throws FootprintException;

    int getUserVisitCount(int hostId);

    /**
     * 直接设置人气数量
     * 
     * @param hostId 用户id
     * @param count 人气数
     * @author xylz (xylz@live.cn)
     * @since 2010-01-26
     */
    void setUserVisitCount(int hostId, int count);

    /**
     * @param visiter 访问者
     * @param visiteeId 被访问者的id
     * @param ref 附加信息
     */
    void visitAppPage(User visiter, int visiteeId, String ref);

    /**
     * @param visiter 访问者
     * @param visiterTinyUrl 访问者的小头像，不带http://前缀
     * @param visiteeId 被访问者的id
     * @param ref 附加信息
     */
    @Deprecated
    void visitAppPage(User visiter, String visiterTinyUrl, int visiteeId, String ref);

    /**
     * @deprecated 2010-01-26
     * @see #visitHomepage(FootprintUniteAdapter, boolean, String)
     */
    void visitHomepage(FootprintView fpt);

    /**
     * @deprecated 2010-01-26
     * @see #visitHomepage(FootprintUniteAdapter, boolean, String)
     */
    void visitHomepage(FootprintView fpt, boolean flag);

    /**
     * 访问个人主页产生人气 <br />
     * 如果需要返回结果，请使用{@link #visitHomepageAndGet(FootprintUniteAdapter, boolean, String)}
     * 
     * @param fpt 脚印信息
     * @param friendFlag 是否是好友
     * @param ref 本次调用的附件信息
     */
    void visitHomepage(FootprintView fpt, boolean friendFlag, String ref);

    /**
     * 留脚印并且返回新的结果
     * 
     * @param fpt 要留的脚印
     * @param friendflag 是否是好友
     * @param ref 引用？？来源？？
     * @return 新的脚印列表
     * @author xylz (xylz@live.cn)
     * @since 2010-01-26
     */
    HomeFootprintResult visitHomepageAndGet(FootprintView fpt, boolean friendflag, String ref);

	void removeFootprint(FootprintView fpt);

}
