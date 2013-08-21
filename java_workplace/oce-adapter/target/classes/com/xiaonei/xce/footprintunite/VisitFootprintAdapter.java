package com.xiaonei.xce.footprintunite;

import java.sql.Date;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;
import java.util.Map.Entry;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.model.FootprintNewView;
import mop.hi.oce.domain.model.FootprintResult;

import com.xiaonei.services.FriendInput.FriendInputManagerPrx;
import com.xiaonei.services.FriendInput.FriendInputManagerPrxHelper;
import com.xiaonei.xce.globalid.GlobalId;

import xce.footprint.FootprintFacadeManagerPrx;
import xce.footprint.FootprintFacadeManagerPrxHelper;
import xce.footprint.FootprintInfo;
import xce.footprint.HomeFootprintCachePrx;
import xce.footprint.HomeFootprintCachePrxHelper;
import xce.footprint.HomeInfo;
import xce.footprint.UgcFootprintCachePrx;
import xce.footprint.UgcFootprintCachePrxHelper;
import xce.footprint.UgcInfo;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.util.channel.Channel;
import xce.util.service.ServiceAdapter;

import com.xiaonei.xce.footprint.FootprintNewAdapter;
import com.xiaonei.xce.footprintunite.HomeFootprintResult;

import com.xiaonei.xce.log.Oce;

public class VisitFootprintAdapter extends ReplicatedClusterAdapter {

	private FootprintFacadeAdapter facade;
	private static final String ctr_endpoints = "ControllerVisitFootprint";
	private static int _interval = 120;
	private int _cluster = 10;

	/***
	 * 默认构造函数，获取一个VisitFootprintAdapter的一个实例
	 */
	public VisitFootprintAdapter() {
		super(ctr_endpoints, _interval);
		facade = new FootprintFacadeAdapter();
	}
	
	/***
	 * 获取一个用户最近访问过的6个用户信息，不限时间
	 * @param owner， 需要获取那个用户最近访问过的用户信息
	 * @return 一个VisitFootprintResult对象，包含owner用户最近访问过的用户的id, 时间等信息
	 */
	public VisitFootprintResult getAll(int owner) {
		return getAll(owner, 0, 6, 0);
	}

	/***
	 * 获取一个用户最近访问过那些用户的信息，用begin, limit进行限制
	 * @param owner， 需要获取那个用户最近访问过的用户信息
	 * @param begin， 从那个index开始，一个用户最近访问过的其他用户从0开始编号，编号越小，访问的时间越晚
	 * @param limit， 最多获取多少个owner访问过的其他用户信息
	 * @return 一个VisitFootprintResult对象，包含owner用户最近访问过的用户的id, 时间等信息
	 */
	public VisitFootprintResult getAll(int owner, int begin, int limit) {
		return getAll(owner, begin, limit, 0);
	}

	/***
	 * 获取一个用户最近访问过那些用户的信息，用begin, limit, nLimit进行限制
	 * @param owner， 需要获取那个用户最近访问过的用户信息
	 * @param begin， 从那个index开始，一个用户最近访问过的其他用户从0开始编号，编号越小，访问的时间越晚
	 * @param nLimit，最多获取多少个owner访问过的其他用户信息
	 * @param tLimit，时间限制,owner访问的用户的时间必须在tLimit之后 
	 * @return 一个VisitFootprintResult对象，包含owner用户最近访问过的用户的id, 时间等信息
	 */
	public VisitFootprintResult getAll(int owner, int begin, int nLimit,
			int tLimit) {
		VisitFootprintResult res = null;
		UgcFootprintCachePrx prx = null;
		try {
			prx = getVisitFootprintManager(owner);
			UgcInfo info = prx.getAll(owner, begin,
					nLimit, tLimit);
			res = new VisitFootprintResult(owner, info.steps);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getAll [" + prx.toString() + "]" + owner
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getAll [" + prx.toString() + "]" + owner, e);
		}

		return res;
	}

	/***
	 * 获取系统中现在保存了owner用户访问了多少个其他用户的信息。系统中保存的一个用户访问过多少个其他用户
	 * 信息的数量是有限制的
	 * @param owner， 需要获取信息的用户id
	 * @return 系统中有多少个owner访问过的其他用户的信息
	 */
	public int getSize(int owner) {
		int size = 0;
		UgcFootprintCachePrx prx = null;
		try {
			prx = getVisitFootprintManager(owner);
			size = prx.getSize(owner);
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getSize [" + prx.toString() + "]" + owner
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getSize [" + prx.toString() + "]" + owner, e);
		}
		return size;
	}

	/***
	 * 删除系统中的一条访问信息， 这个方法只能是相关业务部门使用，其他用户不能随便调用
	 * @param fpt，访问信息，包括访问者，被访问者，时间以及访问的资源等信息
	 */
	public void remove(FootprintView fpt) {
		UgcFootprintCachePrx prx = null;
		try {
			prx = getVisitFootprintManager(fpt.getOwner());
			// System.out.println("visitor:"+fpt.getVisitor()
			// +" owner: "+fpt.getOwner());
			prx.remove(fpt.getFootprint());
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".remove [" + prx.toString() + "]" + fpt.getOwner()
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".remove [" + prx.toString() + "]" + fpt.getOwner(), e);
		}
	}

	private void visit(FootprintView fpt) {
		UgcFootprintCachePrx prx = null;
		try {
			prx = getVisitFootprintManager(fpt.getOwner());
			prx.visit(fpt.getFootprint());
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visit [" + prx.toString() + "]" + fpt.getOwner()
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visit [" + prx.toString() + "]" + fpt.getOwner(), e);
		}
	}

	private void visitBatch(ArrayList<FootprintView> fpts) {
		UgcFootprintCachePrx prx = null;
		try {
			HashMap<Integer, ArrayList<FootprintInfo>> bucket = new HashMap<Integer, ArrayList<FootprintInfo>>();
			for (FootprintView fpt : fpts) {

				ArrayList<FootprintInfo> list = bucket.get(fpt.getOwner()
						% _cluster);
				if (list == null) {
					list = new ArrayList<FootprintInfo>();
				}
				list.add(fpt.getFootprint());
			}			
			for (Entry<Integer, ArrayList<FootprintInfo>> entry : bucket
					.entrySet()) {
				prx = getVisitFootprintManager(entry.getKey(), true);
				prx.visitBatch(
						entry.getValue().toArray(
								new FootprintInfo[entry.getValue().size()]));
			}

		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visitBatch [" + prx.toString() + "]"
							+ " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".visitBatch [" + prx.toString() + "]" + e);
		}
	}

	private UgcFootprintCachePrx getVisitFootprintManager(int owner) {
		return getVisitFootprintManager(owner, false);
	}

	private UgcFootprintCachePrx getVisitFootprintManager(int owner,
			boolean isOneWay) {
//		Ice.ObjectPrx prx0 = locateProxy(_managers, "UFC", owner,
//				Channel.Invoke.Twoway, UgcFootprintCachePrxHelper.class, 300);
		Ice.ObjectPrx prx0 = getProxy(owner, 300);

		// System.out.println(prx0.ice_toString());
		if (prx0 != null) {
			UgcFootprintCachePrx prx;
			if (!isOneWay) {
				prx = UgcFootprintCachePrxHelper.uncheckedCast(prx0
						.ice_timeout(300).ice_twoway());
			} else {
				prx = UgcFootprintCachePrxHelper.uncheckedCast(prx0
						.ice_timeout(300).ice_oneway());
			}
			return prx;
		} else {
			return null;
		}
	}

	/*
	 * public void reloadCache(int host) {public FootprintResult
	 * visitHomepageAndGet(FootprintNewView fpt, boolean friendflag, String
	 * ref); int mod = host % _cluster; int category = 0; ObjectCachePrx objPrx
	 * = channel.locate("SC", "@FootprintN"+mod, Channel.Invoke.Twoway,
	 * ObjectCachePrxHelper.class, 300); objPrx.reloadObject(category, host); }
	 */

	public static void main(String[] args) {

		int hostId = 0;
		int owner = 0;
		int begin = 0;
		int limit = 0;
		VisitFootprintAdapter test = AdapterFactory.getVisitFootprintAdapter();
		GlobalId gid = GlobalId.fromAlbum(1008);
		// // GlobalId gid = GlobalId.fromBlog(ugcId);
		// // GlobalId gid = GlobalId.fromAlbum(ugcId);

		int ownerId = 220840375;
		int[] guests = new int[] { 123, 456, 789 };
		ArrayList<FootprintInfo> fpts = new ArrayList<FootprintInfo>();
		for (int guest = 1; guest <= 30; guest++) {
			FootprintInfo fpt = new FootprintInfo();
			fpt.owner = ownerId;
			fpt.gid = ownerId;
			fpt.timestamp = (int) (System.currentTimeMillis() / 1000L);
			fpt.visitor = guest;
			FootprintView view = new FootprintView(fpt.owner, fpt.visitor,
					(int) (System.currentTimeMillis() / 1000L), GlobalId
							.fromBlog(468823778));
			test.visit(view);
			// fpts.add(fpt);
			// facade.visitBatch(fpts);
			//test.remove(view);
		}
		try {
			Thread.sleep(10);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		// test.remove(view);
		// System.out.println(fpt.visitor);
		// break;
		// test.visitVisitpage(fpnv, false, "UNKNOWN");
		// result = test.visitVisitpageAndGet(fpnv, false, "UNKNOWN");

		// System.exit(0);
		//
		// System.out.println("VisitCount of " + ownerId + " : "
		// + test.getUserVisitCount(ownerId));

		// 取脚印
		// VisitFootprintResult result = test.getAll(ownerId, 0, 96);
		// System.out.println("Visit Count of " + ownerId + " is :"
		// + result.visitcount);

		System.out.println(test.getSize(ownerId) + " footprint(s)\n[");
		VisitFootprintResult result = test.getAll(ownerId, 0, 96);
		for (FootprintView view : result.getFootprintViews()) {
			System.out.println(view.getVisitor() + " " + view.getOwner() + " "
					+ view.getTime());
		}
		System.out.println("]\n");
	}
}
