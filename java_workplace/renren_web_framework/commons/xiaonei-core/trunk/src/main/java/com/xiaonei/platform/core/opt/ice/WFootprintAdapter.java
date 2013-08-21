/**
 * 
 */
package com.xiaonei.platform.core.opt.ice;

import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.svc.fpt.FootprintException;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.xce.footprintunite.FootprintUniteAdapter;
import com.xiaonei.xce.footprintunite.FootprintView;
import com.xiaonei.xce.globalid.GlobalId;

/**
 * <ul>
 * <li>脚印
 * 
 * <pre>
 * 使用的FootprintView的构造方式
 * UserBasic ub = UserBasicDAO.getInstance().get(host.getId());
 * FootprintView footprint = new FootprintView();
 * footprint.setHost(guestId);
 * footprint.setGuest(host.getId());
 * footprint.setGuestName(host.getName());
 * footprint.setGuestUrl(ub.getTinyUrl());
 * footprint.setTime(new Date());
 * 
 * </pre>
 * 
 * @author skllb </li> <li>2009-2-20 下午02:14:57</li> <li></li>
 *         </ul>
 */
public class WFootprintAdapter {

    private FootprintUniteAdapter adapter = AdapterFactory.getHomeFootprintAdapter();

    // ////////////////////////////////////////////////////////////////////
    // 单例部分
    private static WFootprintAdapter instance = new WFootprintAdapter();

    /**
     * 获取单例
     */
    public static WFootprintAdapter getInstance() {
        return instance;
    }

    /**
     * <ul>
     * <li>获得用户的所有脚印</li>
     * </ul>
     * 
     * @param hostId 用户编号
     * @return
     * @throws FootprintException
     */
    public List<FootprintView> getAll(int hostId) throws FootprintException {
        return adapter.getAll(hostId).getFootprintViews();
    }

    /**
     * <ul>
     * <li>分页获取用户的脚印信息</li>
     * </ul>
     * 
     * @param hostId
     * @param begin
     * @param limit
     * @return
     * @throws FootprintException
     */
    public List<FootprintView> getAll(int hostId, int begin, int limit)
            throws FootprintException {
        return adapter.getAll(hostId, begin, limit).getFootprintViews();
    }

    /**
     * <ul>
     * <li>获取脚印数量</li>
     * </ul>
     * 
     * @param hostId
     * @return
     * @throws FootprintException
     */
    public int getSize(int hostId) throws FootprintException {
        return adapter.getSize(hostId);
    }

    /**
     * <ul>
     * <li>保存脚印:::默认表示为非好友来访</li>
     * </ul>
     * 
     * @param fpt
     */
    public void visitHomepage(FootprintView fpt) {
        adapter.visit(fpt);
    }

    /**
     * @param fpt
     * @param friendFlag
     * @param ref 本次调用的附件信息
     */
    public void visitHomepage(FootprintView fpt, boolean friendFlag, String ref) {
        adapter.visit(fpt);
    }

    /**
     * @param visiter 访问者
     * @param visiteeId 被访问者的id
     * @param ref 附加信息
     */
    public void visitAppPage(User visiter, int visiteeId, String ref) {
        UserBasic userbasic = WUserBasicAdapter.getInstance().get(visiter.getId());
        visitAppPage(visiter, userbasic.getTinyUrl(), visiteeId, ref);
    }

    /**
     * @param visiter 访问者
     * @param visiterTinyUrl 访问者的小头像，不带http://前缀
     * @param visiteeId 被访问者的id
     * @param ref 附加信息
     */
	public void visitAppPage(User visiter, String visiterTinyUrl,
			int visiteeId, String ref) {
		FootprintView fpv = new FootprintView(visiteeId, visiter.getId(), 0,
				GlobalId.fromProfile(visiteeId));
		adapter.visit(fpv);
	}

    /**
     * <ul>
     * <li>保存脚印</li>
     * </ul>
     * 
     * @param fpt 脚印对象
     * @param flag 来访者是否是访问人的好友
     */
    public void visitHomepage(FootprintView fpt, boolean flag) {
        adapter.visit(fpt);
    }
}
