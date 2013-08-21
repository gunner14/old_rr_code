/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.svc.fpt.FootprintException;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.sns.platform.core.opt.ice.IFootprintAdapter;
import com.xiaonei.xce.footprintunite.FootprintUniteAdapter;
import com.xiaonei.xce.footprintunite.FootprintView;
import com.xiaonei.xce.footprintunite.HomeFootprintResult;
import com.xiaonei.xce.globalid.GlobalId;

/**
 * 脚印服务
 * 
 * @author xylz (xylz@live.cn)
 * @author tai.wang@opi-corp.com May 31, 2010 - 6:34:27 PM
 */
public class WFootprintAdapter extends BaseUsersAdapter<FootprintUniteAdapter>
		implements IFootprintAdapter {

	/**
	 * 获取单例
	 * 
	 * @deprecated
	 * @see {@linkplain SnsAdapterFactory#getFootprintAdapter()}
	 */
	public static IFootprintAdapter getInstance() {
		return SnsAdapterFactory.getFootprintAdapter();
	}

	protected WFootprintAdapter() {
	}

	@Override
	protected FootprintUniteAdapter makeObject() {
		return AdapterFactory.getHomeFootprintAdapter();
	}

	public List<FootprintView> getAll(int hostId) throws FootprintException {
		return getAdapter().getAll(hostId).getFootprintViews();
	}

	public List<FootprintView> getAll(int hostId, int begin, int limit)
			throws FootprintException {
		return getAdapter().getAll(hostId, begin, limit).getFootprintViews();
	}

	public int getSize(int hostId) throws FootprintException {
		return getAdapter().getSize(hostId);
	}

	@Deprecated
	public void visitHomepage(FootprintView fpt) {
		getAdapter().visit(fpt);
	}

	public void visitHomepage(FootprintView fpt, boolean friendFlag, String ref) {
		getAdapter().visit(fpt);
	}

	public void visitAppPage(User visiter, int visiteeId, String ref) {
		UserBasic userbasic = SnsAdapterFactory.getUserBasicAdapter().get(
				visiter.getId());
		visitAppPage(visiter, userbasic.getTinyUrl(), visiteeId, ref);
	}

	@SuppressWarnings("deprecation")
	public void visitAppPage(User visiter, String visiterTinyUrl,
			int visiteeId, String ref) {
		GlobalId gid = GlobalId.fromProfile(visiteeId);
		FootprintView fpv = new FootprintView(visiteeId, visiter.getId(),
				(int) (System.currentTimeMillis() / 1000), gid);
		// fpv.setGuest(visiter.getId()); // guest是访问者
		// fpv.setHost(visiteeId); // host是被访问者
		// fpv.setTime(new Date());
		getAdapter().visit(fpv);
	}

	@Deprecated
	public void visitHomepage(FootprintView fpt, boolean flag) {
		getAdapter().visit(fpt);
	}

	@Override
	public int getUserVisitCount(int hostId) {
		return getAdapter().getUserVisitCount(hostId);
	}

	@Override
	public void setUserVisitCount(int hostId, int count) {
		getAdapter().setUserVisitCount(hostId, count);
	}

	@Override
	public HomeFootprintResult visitHomepageAndGet(FootprintView fpt,
			boolean friendflag, String ref) {
		return getAdapter().visitAndGet(fpt);
	}

	@Override
	public HomeFootprintResult getAllFootprints(int hostId, int begin, int limit) {
		return getAdapter().getAll(hostId,begin,limit);
	}
	
	@Override
	public void removeFootprint(FootprintView fpt){
		getAdapter().remove(fpt);
	}

	@Override
	protected String getServiceName() {
		return "footprint";
	}
}
